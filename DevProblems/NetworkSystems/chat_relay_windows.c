/*
 * chat_relay_win.c
 *
 * Windows / Winsock port of chat_relay.c.
 *
 * Same behaviour and same single-threaded, non-blocking select() design
 * as the POSIX version. Usage is identical:
 *
 *     chat_relay_win.exe <port>
 *
 * ---------------------------------------------------------------------
 * WHY THIS ISN'T A STRAIGHT RECOMPILE
 *
 * The original allowed-function list (socket/bind/listen/accept/select/
 * recv/send/close/fcntl/signal/...) is POSIX. Winsock keeps most of the
 * *names* (socket, bind, listen, accept, select, recv, send, setsockopt,
 * htons, htonl) but a handful of things are different enough that a
 * couple of substitutions are unavoidable:
 *
 *   - fcntl() does not exist on Windows. There is no O_NONBLOCK you can
 *     flip with it. The Winsock equivalent is ioctlsocket() with the
 *     FIONBIO command.
 *   - close() closes CRT file descriptors, not SOCKET handles. Winsock
 *     sockets must be closed with closesocket().
 *   - A POSIX fd is a small int; a Winsock SOCKET is an opaque handle
 *     (UINT_PTR under the hood) and comparisons against -1 are wrong -
 *     you compare against INVALID_SOCKET, and socket calls that "fail"
 *     return SOCKET_ERROR rather than -1.
 *   - errno/EAGAIN/EWOULDBLOCK are not populated by Winsock calls. You
 *     read the error with WSAGetLastError() and check WSAEWOULDBLOCK.
 *   - Winsock must be explicitly initialised/torn down with
 *     WSAStartup()/WSACleanup() - there's no equivalent step on POSIX.
 *   - Windows sockets never raise SIGPIPE (POSIX does, which is why the
 *     original code ignores it) - the #ifdef simply skips that call
 *     here since the macro doesn't exist to ignore.
 *   - write(2, ...) for reporting a fatal error to stderr assumed a
 *     POSIX low-level fd. That header (unistd.h) isn't available, so
 *     the two fatal-error paths use fprintf(stderr, ...) from stdio.h
 *     instead (stdio.h was already in use for snprintf).
 *   - Winsock's fd_set is an *array* of handles capped by FD_SETSIZE
 *     (default 64), not a bitmask indexed by fd number as on POSIX. It
 *     must be redefined (bigger) before including winsock2.h to accept
 *     more concurrent clients, which is done below.
 *
 * Everything else - the accept/read/write loop structure, the growable
 * per-client read/write buffers, the line-splitting logic, id
 * assignment, join/leave broadcasts - is unchanged from chat_relay.c.
 *
 * Build (MinGW-w64):
 *     gcc -Wall -Wextra -std=c11 -O2 -o chat_relay_win.exe chat_relay_win.c -lws2_32
 *
 * Build (MSVC):
 *     cl /W4 chat_relay_win.c ws2_32.lib
 * ---------------------------------------------------------------------
 */

#define WIN32_LEAN_AND_MEAN
#define FD_SETSIZE 1024      /* raise Winsock's default 64-socket select() cap */

#include <winsock2.h>   /* socket, bind, listen, accept, select, send, recv,
                           setsockopt, htons, htonl, closesocket, ioctlsocket,
                           WSAStartup, WSACleanup, WSAGetLastError            */
#include <winsock.h>    /* pulled in for completeness/older constants; on
                           modern MinGW/MSVC winsock2.h alone is sufficient
                           and should be included first, as done above       */
#include <sys/types.h>  /* size_t and friends                                */

#include <stdio.h>      /* snprintf, fprintf (fatal-error reporting)         */
#include <stdlib.h>     /* malloc, realloc, free, exit, atoi                 */
#include <string.h>     /* memset, memcpy, memmove, strlen                   */
#include <signal.h>     /* signal, SIGINT, SIGTERM                           */

#pragma comment(lib, "ws2_32.lib") /* MSVC: link Winsock; ignored by MinGW   */

#define MAX_CLIENTS   FD_SETSIZE
#define RECV_CHUNK    4096
#define INITIAL_CAP   256

typedef struct {
    SOCKET fd;         /* INVALID_SOCKET if this slot is unused */
    int    id;          /* permanent, never-reused connection id */
    int    active;       /* 1 if this slot holds a live connection */

    char   *rbuf;        /* pending (not-yet-newline-terminated) input */
    size_t  rlen;
    size_t  rcap;

    char   *wbuf;        /* queued outbound bytes not yet sent */
    size_t  wlen;
    size_t  wcap;
    size_t  woff;
} Client;

static Client clients[MAX_CLIENTS];
static int next_id = 0;
static volatile sig_atomic_t running = 1;

/* ------------------------------------------------------------------ */

static void die_setup(void) {
    fprintf(stderr, "Server setup failed\n");
    WSACleanup();
    exit(1);
}

static void die_args(void) {
    fprintf(stderr, "Invalid argument count\n");
    exit(1);
}

static void on_signal(int sig) {
    (void)sig;
    running = 0;
}

/* Winsock non-blocking mode: ioctlsocket(FIONBIO) is the fcntl(O_NONBLOCK)
 * equivalent - Windows has no fcntl() at all. */
static int set_nonblocking(SOCKET fd) {
    u_long mode = 1; /* nonzero = non-blocking */
    return ioctlsocket(fd, FIONBIO, &mode); /* 0 on success, SOCKET_ERROR on failure */
}

static int buf_append(char **buf, size_t *len, size_t *cap,
                       const char *data, size_t n) {
    if (n == 0) return 0;
    if (*len + n > *cap) {
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

static void queue_write(Client *c, const char *data, size_t len) {
    buf_append(&c->wbuf, &c->wlen, &c->wcap, data, len);
}

static void broadcast(int exclude_id, const char *data, size_t len) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) continue;
        if (clients[i].id == exclude_id) continue;
        queue_write(&clients[i], data, len);
    }
}

static void release_client_slot(Client *c) {
    free(c->rbuf);
    free(c->wbuf);
    c->rbuf = NULL; c->rlen = 0; c->rcap = 0;
    c->wbuf = NULL; c->wlen = 0; c->wcap = 0; c->woff = 0;
    c->active = 0;
    c->fd = INVALID_SOCKET;
    c->id = -1;
}

/* closesocket(), not close() - a Winsock SOCKET is not a CRT file
 * descriptor and close() must not be used on it. */
static void disconnect_client(Client *c) {
    char msg[64];
    int n = snprintf(msg, sizeof msg, "relay: peer %d left\n", c->id);
    if (n > 0) broadcast(c->id, msg, (size_t)n);

    closesocket(c->fd);
    release_client_slot(c);
}

static void process_lines(Client *c) {
    size_t start = 0;

    for (size_t i = 0; i < c->rlen; i++) {
        if (c->rbuf[i] != '\n') continue;

        size_t linelen = i - start;

        char prefix[64];
        int plen = snprintf(prefix, sizeof prefix, "peer %d says: ", c->id);
        if (plen < 0) plen = 0;

        size_t total = (size_t)plen + linelen + 1;
        char *msg = malloc(total);
        if (msg) {
            memcpy(msg, prefix, (size_t)plen);
            if (linelen > 0) memcpy(msg + plen, c->rbuf + start, linelen);
            msg[total - 1] = '\n';
            broadcast(c->id, msg, total);
            free(msg);
        }

        start = i + 1;
    }

    if (start > 0) {
        size_t remaining = c->rlen - start;
        if (remaining > 0) memmove(c->rbuf, c->rbuf + start, remaining);
        c->rlen = remaining;
    }
}

/* ------------------------------------------------------------------ */

int main(int argc, char *argv[]) {
    if (argc != 2) die_args();

    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].fd = INVALID_SOCKET;
        clients[i].active = 0;
    }

    /* No SIGPIPE on Windows - Winsock reports a failed send() through
     * its return value/WSAGetLastError() instead of a signal, so there
     * is nothing to ignore here. SIGINT still works for a clean exit;
     * SIGTERM is accepted by the C runtime but Windows has no real
     * process-level SIGTERM delivery, so treat that as best-effort. */
    signal(SIGINT, on_signal);
    signal(SIGTERM, on_signal);

    /* Winsock must be initialised before any socket calls. */
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Server setup failed\n");
        exit(1);
    }

    SOCKET listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == INVALID_SOCKET) die_setup();

    /* On Winsock, setsockopt's optval is a const char*, not const void*. */
    BOOL opt = TRUE;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,
                    (const char *)&opt, sizeof opt) == SOCKET_ERROR)
        die_setup();

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* 127.0.0.1 */

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof addr) == SOCKET_ERROR)
        die_setup();

    if (listen(listen_fd, 128) == SOCKET_ERROR)
        die_setup();

    if (set_nonblocking(listen_fd) == SOCKET_ERROR)
        die_setup();

    while (running) {
        fd_set readfds, writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(listen_fd, &readfds);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].active) continue;
            FD_SET(clients[i].fd, &readfds);
            if (clients[i].wlen > clients[i].woff)
                FD_SET(clients[i].fd, &writefds);
        }

        /* Winsock ignores the nfds argument (kept for source
         * compatibility) - pass 0. */
        int ready = select(0, &readfds, &writefds, NULL, NULL);
        if (ready == SOCKET_ERROR) {
            /* Winsock select() isn't interrupted by signals the way
             * POSIX select() can be (no EINTR concept), so just check
             * whether our handler cleared `running` and stop otherwise. */
            if (running) continue;
            break;
        }

        if (FD_ISSET(listen_fd, &readfds)) {
            for (;;) {
                SOCKET fd = accept(listen_fd, NULL, NULL);
                if (fd == INVALID_SOCKET) break; /* WSAEWOULDBLOCK: none left */

                if (set_nonblocking(fd) == SOCKET_ERROR) {
                    closesocket(fd);
                    continue;
                }

                int slot = -1;
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!clients[i].active) { slot = i; break; }
                }
                if (slot < 0) {
                    closesocket(fd);
                    continue;
                }

                Client *c = &clients[slot];
                c->fd = fd;
                c->id = next_id++;
                c->active = 1;
                c->rbuf = NULL; c->rlen = 0; c->rcap = 0;
                c->wbuf = NULL; c->wlen = 0; c->wcap = 0; c->woff = 0;

                char msg[64];
                int n = snprintf(msg, sizeof msg,
                                  "relay: peer %d joined\n", c->id);
                if (n > 0) broadcast(c->id, msg, (size_t)n);
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (!clients[i].active) continue;
            Client *c = &clients[i];

            if (FD_ISSET(c->fd, &writefds) && c->wlen > c->woff) {
                int n = send(c->fd, c->wbuf + c->woff,
                             (int)(c->wlen - c->woff), 0);
                if (n > 0) {
                    c->woff += (size_t)n;
                    if (c->woff == c->wlen) {
                        c->wlen = 0;
                        c->woff = 0;
                    }
                } else if (n == SOCKET_ERROR &&
                           WSAGetLastError() != WSAEWOULDBLOCK) {
                    disconnect_client(c);
                    continue;
                }
            }

            if (!c->active) continue;

            if (FD_ISSET(c->fd, &readfds)) {
                char buf[RECV_CHUNK];
                int n = recv(c->fd, buf, sizeof buf, 0);
                if (n > 0) {
                    if (buf_append(&c->rbuf, &c->rlen, &c->rcap,
                                    buf, (size_t)n) < 0) {
                        disconnect_client(c);
                        continue;
                    }
                    process_lines(c);
                } else if (n == 0) {
                    disconnect_client(c);
                    continue;
                } else if (WSAGetLastError() != WSAEWOULDBLOCK) {
                    disconnect_client(c);
                    continue;
                }
            }
        }
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (!clients[i].active) continue;
        closesocket(clients[i].fd);
        release_client_slot(&clients[i]);
    }
    closesocket(listen_fd);
    WSACleanup(); /* undo WSAStartup() - no POSIX equivalent needed */

    return 0;
}