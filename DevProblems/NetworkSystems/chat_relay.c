/*
 * chat_relay.c
 *
 * A single-threaded, non-blocking TCP chat relay.
 *
 * Usage: ./chat_relay <port>
 *
 * The program binds to 127.0.0.1:<port>, accepts any number of clients,
 * and relays every line a client sends to every OTHER connected client,
 * prefixed with "peer <id> says: ". Join/leave events are also
 * broadcast. Everything is driven from a single select() loop so one
 * slow/stuck client can never block the others: all sockets are
 * non-blocking and all output is queued into per-client buffers that
 * are drained opportunistically.
 *
 * ---------------------------------------------------------------------
 * Functions used, and why:
 *
 *   socket()      - create the listening TCP socket and (implicitly via
 *                   accept()) each client socket.
 *   setsockopt()  - set SO_REUSEADDR on the listening socket so the
 *                   server can restart quickly on the same port.
 *   bind()        - bind the listening socket to 127.0.0.1:<port>.
 *   listen()      - mark the socket as ready to accept connections.
 *   accept()      - pull a queued incoming connection off the listener.
 *   select()      - the single I/O multiplexer: waits until the
 *                   listening socket or any client socket is readable
 *                   or writable, without blocking on any one of them.
 *   fcntl()       - put every socket (listener + clients) into
 *                   non-blocking mode (O_NONBLOCK) so recv/send/accept
 *                   never stall the loop.
 *   recv()        - read bytes from a client into its read buffer.
 *   send()        - write queued bytes out to a client's socket.
 *   read()/write()- write() is used for reporting fatal startup errors
 *                   to stderr (fd 2) without pulling in stdio's
 *                   buffered printf family. read() is not needed for
 *                   sockets (recv() is used instead) but is listed as
 *                   allowed; unused here.
 *   close()       - close a socket when a client disconnects or when
 *                   the server shuts down.
 *   signal()      - ignore SIGPIPE (so a send() to an already-closed
 *                   peer can't kill the process) and catch
 *                   SIGINT/SIGTERM for a clean shutdown that frees
 *                   every buffer and closes every descriptor.
 *   htons()       - convert the port number to network byte order.
 *   htonl()       - convert INADDR_LOOPBACK to network byte order.
 *   malloc()/realloc()/free() - dynamically grow each client's
 *                   per-connection read buffer (partial line storage)
 *                   and write buffer (queued outgoing data), and free
 *                   them when the client disconnects.
 *   memset()      - zero the sockaddr_in struct before filling it in.
 *   memcpy()      - append newly received/queued bytes into buffers.
 *   memmove()     - shift a partial (unterminated) line to the front
 *                   of a client's read buffer after complete lines
 *                   have been extracted (source/dest can overlap).
 *   strlen()      - measure fixed C-string error/status messages
 *                   before writing them out.
 *   atoi()        - parse the port number command-line argument.
 *   snprintf()    - safely format the "peer %d joined/left/says: "
 *                   messages into buffers with known bounds.
 *   exit()        - terminate immediately on usage/setup errors.
 *
 * Not used: strcpy(), sprintf(), poll(), calloc() - listed as allowed
 * but not needed given the design above.
 * ---------------------------------------------------------------------
 */

#include <stdio.h>      /* snprintf */
#include <stdlib.h>     /* malloc, realloc, free, exit, atoi */
#include <string.h>     /* memset, memcpy, memmove, strlen */
#include <unistd.h>     /* close, read, write */
#include <fcntl.h>      /* fcntl, O_NONBLOCK */
#include <errno.h>      /* errno, EAGAIN, EWOULDBLOCK, EINTR */
#include <signal.h>     /* signal, SIGINT, SIGTERM, SIGPIPE */
#include <sys/types.h>
#include <sys/socket.h> /* socket, setsockopt, bind, listen, accept, send, recv */
#include <sys/select.h> /* select, fd_set, FD_ZERO, FD_SET, FD_ISSET, FD_SETSIZE */
#include <netinet/in.h> /* sockaddr_in, htons, htonl, INADDR_LOOPBACK */

#define MAX_CLIENTS   FD_SETSIZE
#define RECV_CHUNK    4096
#define INITIAL_CAP   256

typedef struct {
    int fd;          /* socket fd, or -1 if this slot is unused */
    int id;           /* permanent, never-reused connection id */
    int active;       /* 1 if this slot holds a live connection */

    char   *rbuf;     /* pending (not-yet-newline-terminated) input */
    size_t  rlen;
    size_t  rcap;

    char   *wbuf;     /* queued outbound bytes not yet sent */
    size_t  wlen;      /* total bytes currently queued           */
    size_t  wcap;
    size_t  woff;      /* how much of wbuf has already been sent */
} Client;

static Client clients[MAX_CLIENTS];
static int next_id = 0;
static volatile sig_atomic_t running = 1;

/* ------------------------------------------------------------------ */
/* Small helpers                                                       */
/* ------------------------------------------------------------------ */

static void die_setup(void) 
{
    const char *msg = "Server setup failed\n";
    write(2, msg, strlen(msg));
    exit(1);
}

static void die_args(void) 
{
    const char *msg = "Invalid argument count\n";
    write(2, msg, strlen(msg));
    exit(1);
}

static void on_signal(int sig) 
{
    (void)sig;
    running = 0;
}

static int set_nonblocking(int fd) 
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* Append n bytes of data onto a growable buffer, growing (doubling)
 * its capacity with realloc() as needed. Returns 0 on success, -1 on
 * allocation failure. */
static int buf_append(char **buf, size_t *len, size_t *cap, const char *data, size_t n) 
{
    if (n == 0) return 0;
    if (*len + n > *cap) 
    {
        size_t newcap = (*cap == 0) ? INITIAL_CAP : *cap;
        while (newcap < *len + n) newcap *= 2;
        char *tmp = realloc(*buf, newcap);
        if (!tmp) return -1;
        *buf = tmp;
        *cap = newcap;
    }
    memcpy(*buf + *len, data, n);
    *len += n;
    return 0;
}

/* Queue raw bytes to be sent out to a single client. */
static void queue_write(Client *c, const char *data, size_t len) 
{
    buf_append(&c->wbuf, &c->wlen, &c->wcap, data, len);
}

/* Send data to every active client except the one with id exclude_id.
 * Pass exclude_id = -1 to send to everyone. */
static void broadcast(int exclude_id, const char *data, size_t len) 
{
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (!clients[i].active) continue;
        if (clients[i].id == exclude_id) continue;
        queue_write(&clients[i], data, len);
    }
}

/* Free a client's buffers and mark its slot free. Does not touch the
 * socket (caller closes it) and does not broadcast anything - callers
 * decide whether a "left" notice is appropriate. */
static void release_client_slot(Client *c) 
{
    free(c->rbuf);
    free(c->wbuf);
    c->rbuf = NULL; c->rlen = 0; c->rcap = 0;
    c->wbuf = NULL; c->wlen = 0; c->wcap = 0; c->woff = 0;
    c->active = 0;
    c->fd = -1;
    c->id = -1;
}

/* Disconnect a client: notify the others, close its socket, free its
 * buffers, and free its slot. */
static void disconnect_client(Client *c) 
{
    char msg[64];
    int n = snprintf(msg, sizeof msg, "relay: peer %d left\n", c->id);
    if (n > 0) broadcast(c->id, msg, (size_t)n);

    close(c->fd);
    release_client_slot(c);
}

/* Scan a client's read buffer for complete ('\n'-terminated) lines,
 * forward each one (with its "peer %d says: " prefix) to every other
 * client, then compact any trailing partial line to the front of the
 * buffer so it can be completed by a later recv(). */
static void process_lines(Client *c) 
{
    size_t start = 0;

    for (size_t i = 0; i < c->rlen; i++) 
    {
        if (c->rbuf[i] != '\n') continue;

        size_t linelen = i - start;

        char prefix[64];
        int plen = snprintf(prefix, sizeof prefix, "peer %d says: ", c->id);
        if (plen < 0) plen = 0;

        size_t total = (size_t)plen + linelen + 1; /* +1 for trailing \n */
        char *msg = malloc(total);
        if (msg) 
        {
            memcpy(msg, prefix, (size_t)plen);
            if (linelen > 0) memcpy(msg + plen, c->rbuf + start, linelen);
            msg[total - 1] = '\n';
            broadcast(c->id, msg, total);
            free(msg);
        }

        start = i + 1;
    }

    if (start > 0) 
    {
        size_t remaining = c->rlen - start;
        if (remaining > 0) memmove(c->rbuf, c->rbuf + start, remaining);
        c->rlen = remaining;
    }
}

/* ------------------------------------------------------------------ */
/* Main                                                                 */
/* ------------------------------------------------------------------ */

int main(int argc, char *argv[]) 
{
    if (argc != 2) die_args();

    /* Initializing clinets */
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        clients[i].fd = -1;
        clients[i].active = 0;
    }

    /* SIGPIPE would otherwise kill us if we send() to a peer that has
     * already closed its end; we detect that via send()'s return value
     * instead. SIGINT/SIGTERM trigger a graceful, buffer-freeing exit. */
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) die_setup();

    int opt = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt) < 0)
        die_setup();

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* 127.0.0.1 */

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof addr) < 0)
        die_setup();

    if (listen(listen_fd, 128) < 0)
        die_setup();

    if (set_nonblocking(listen_fd) < 0)
        die_setup();

    while (running) 
    {
        fd_set readfds;
        fd_set writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(listen_fd, &readfds);
        int maxfd = listen_fd;

        for (int i = 0; i < MAX_CLIENTS; i++) 
        {
            if (!clients[i].active) continue;
            FD_SET(clients[i].fd, &readfds);
            if (clients[i].wlen > clients[i].woff)
                FD_SET(clients[i].fd, &writefds);
            if (clients[i].fd > maxfd) maxfd = clients[i].fd;
        }

        int ready = select(maxfd + 1, &readfds, &writefds, NULL, NULL);
        if (ready < 0) 
        {
            if (errno == EINTR) continue; /* likely our signal handler */
            break;                        /* unrecoverable select error */
        }

        /* Accept every pending connection (loop until EWOULDBLOCK). */
        if (FD_ISSET(listen_fd, &readfds)) 
        {
            for (;;) 
            {
                int fd = accept(listen_fd, NULL, NULL);
                if (fd < 0) break; /* EWOULDBLOCK/EAGAIN: none left */

                if (set_nonblocking(fd) < 0) 
                {
                    close(fd);
                    continue;
                }

                int slot = -1;
                for (int i = 0; i < MAX_CLIENTS; i++) 
                {
                    if (!clients[i].active) { slot = i; break; }
                }
                if (slot < 0) 
                {
                    /* No room for more clients; reject this one. */
                    close(fd);
                    continue;
                }

                Client *c = &clients[slot];
                c->fd = fd;
                c->id = next_id++;
                c->active = 1;
                c->rbuf = NULL; c->rlen = 0; c->rcap = 0;
                c->wbuf = NULL; c->wlen = 0; c->wcap = 0; c->woff = 0;

                char msg[64];
                int n = snprintf(msg, sizeof msg, "relay: peer %d joined\n", c->id);
                if (n > 0) broadcast(c->id, msg, (size_t)n);
            }
        }

        /* Service every active client: drain outgoing data first, then
         * pull in and process any newly arrived incoming data. */
        for (int i = 0; i < MAX_CLIENTS; i++) 
        {
            if (!clients[i].active) continue;
            Client *c = &clients[i];

            if (FD_ISSET(c->fd, &writefds) && c->wlen > c->woff) 
            {
                ssize_t n = send(c->fd, c->wbuf + c->woff, c->wlen - c->woff, 0);
                if (n > 0) 
                {
                    c->woff += (size_t)n;
                    if (c->woff == c->wlen) 
                    {
                        c->wlen = 0;
                        c->woff = 0;
                    }
                } 
                else if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) 
                {
                    disconnect_client(c);
                    continue; /* slot is gone; skip the read check below */
                }
            }

            if (!c->active) continue;

            if (FD_ISSET(c->fd, &readfds)) 
            {
                char buf[RECV_CHUNK];
                ssize_t n = recv(c->fd, buf, sizeof buf, 0);
                if (n > 0) 
                {
                    if (buf_append(&c->rbuf, &c->rlen, &c->rcap, buf, (size_t)n) < 0) 
                    {
                        disconnect_client(c);
                        continue;
                    }
                    process_lines(c);
                } 
                else if (n == 0) 
                {
                    disconnect_client(c);
                    continue;
                } 
                else if (errno != EAGAIN && errno != EWOULDBLOCK) 
                {
                    disconnect_client(c);
                    continue;
                }
            }
        }
    }

    /* Graceful shutdown: release every buffer and close every
     * descriptor we own. */
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        if (!clients[i].active) continue;
        close(clients[i].fd);
        release_client_slot(&clients[i]);
    }
    close(listen_fd);

    return 0;
}