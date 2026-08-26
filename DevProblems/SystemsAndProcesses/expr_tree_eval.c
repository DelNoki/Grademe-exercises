#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Grammar (no spaces, digits + '+' '*' '(' ')'):
 *
 *   expr   := term   ( '+' term   )*
 *   term   := factor ( '*' factor )*
 *   factor := number | '(' expr ')'
 *   number := digit+
 */

typedef enum {
    NODE_NUM,
    NODE_ADD,
    NODE_MUL
} NodeType;

typedef struct Node {
    NodeType     type;
    long         value; /* only used when type == NODE_NUM */
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    const char *s;
    size_t      pos;
} Parser;

/* ---- error state (set once, never overwritten by outer callers) ---- */
typedef enum { ERR_NONE, ERR_INVALID_TOKEN, ERR_UNEXPECTED_END } ErrType;

static ErrType g_err = ERR_NONE;
static char    g_err_char = '\0';

static void set_invalid_token(char c)
{
    if (g_err == ERR_NONE) {
        g_err = ERR_INVALID_TOKEN;
        g_err_char = c;
    }
}

static void set_unexpected_end(void)
{
    if (g_err == ERR_NONE) {
        g_err = ERR_UNEXPECTED_END;
    }
}

static void report_error(void)
{
    if (g_err == ERR_INVALID_TOKEN) {
        printf("Invalid token '%c'\n", g_err_char);
    } else if (g_err == ERR_UNEXPECTED_END) {
        printf("Unexpected end\n");
    }
}

/* ---- small parser helpers ---- */

static char peek(const Parser *p)
{
    return p->s[p->pos];
}

static char advance(Parser *p)
{
    char c = p->s[p->pos];
    if (c != '\0') {
        p->pos++;
    }
    return c;
}

/* ---- tree helpers ---- */

static Node *make_num(long value)
{
    Node *n = malloc(sizeof(Node));
    if (!n)
        return NULL;
    
    n->type = NODE_NUM;
    n->value = value;
    n->left = NULL;
    n->right = NULL;
    return n;
}

static Node *make_binop(NodeType type, Node *left, Node *right)
{
    Node *n = malloc(sizeof(Node));
    if (!n)
        return NULL;

    n->type = type;
    n->value = 0;
    n->left = left;
    n->right = right;
    return n;
}

static void free_tree(Node *n)
{
    if (!n)
        return;

    free_tree(n->left);
    free_tree(n->right);
    free(n);
}

/* ---- 
recursive descent parsing : parse_expr (gère +) 
fait appel à parse_term (gère *)
qui fait appel à parse_term (gère les parenthèses)
qui fait appel à parse_number (gère les nombres) et parse_expr quand il y a parenthèses.
---- */

static Node *parse_expr(Parser *p);

static Node *parse_number(Parser *p)
{
    long value = 0;

    if (!isdigit((unsigned char)peek(p))) 
    {
        if (peek(p) == '\0')
            set_unexpected_end();
        else
            set_invalid_token(peek(p));
        return NULL;
    }

    while (isdigit((unsigned char)peek(p))) 
    {
        char c = advance(p);
        value = value * 10 + (c - '0');
    }

    return make_num(value);
}

/* Priorité aux parenthèses */
static Node *parse_factor(Parser *p)
{
    char c = peek(p);

    if (c == '(') 
    {
        advance(p);
        Node *inner = parse_expr(p);
        if (!inner)
            return NULL;

        if (peek(p) != ')') 
        {
            if (peek(p) == '\0')
                set_unexpected_end();
            else
                set_invalid_token(peek(p));
            
            free_tree(inner);
            return NULL;
        }
        advance(p); /* consume ')' */
        return inner;
    }

    if (isdigit((unsigned char)c))
        return parse_number(p);

    if (c == '\0')
        set_unexpected_end();
    else
        set_invalid_token(c);
    
    return NULL;
}

/* Priorité aux multiplications */
static Node *parse_term(Parser *p)
{
    Node *left = parse_factor(p);
    if (!left)
        return NULL;

    while (peek(p) == '*') 
    {
        advance(p);
        Node *right = parse_factor(p);
        if (!right) 
        {
            free_tree(left);
            return NULL;
        }
        left = make_binop(NODE_MUL, left, right);
        if (!left) 
        {
            free_tree(right);
            return NULL;
        }
    }

    return left;
}

/* Les additions */
static Node *parse_expr(Parser *p)
{
    Node *left = parse_term(p);
    if (!left)
        return NULL;

    while (peek(p) == '+') 
    {
        advance(p);
        Node *right = parse_term(p);
        if (!right) 
        {
            free_tree(left);
            return NULL;
        }
        left = make_binop(NODE_ADD, left, right);
        if (!left) 
        {
            free_tree(right);
            return NULL;
        }
    }

    return left;
}

/* ---- evaluation ---- */

static long eval(const Node *n)
{
    switch (n->type) 
    {
        case NODE_NUM:
            return n->value;
        case NODE_ADD:
            return eval(n->left) + eval(n->right);
        case NODE_MUL:
            return eval(n->left) * eval(n->right);
        default:
            return 0;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    Parser p;
    p.s = argv[1];
    p.pos = 0;

    Node *root = parse_expr(&p);

    if (root && peek(&p) != '\0') 
    {
        /* trailing garbage after a syntactically complete expression */
        set_invalid_token(peek(&p));
        free_tree(root);
        root = NULL;
    }

    if (!root) 
    {
        report_error();
        return 1;
    }

    printf("%ld\n", eval(root));
    free_tree(root);

    return 0;
}


/*
FAIL
#include <unistd.h>

static int ft_len(const char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

static int is_valid_char(const char c)
{
	return ((c >= '0' && c <= '9') || c == '+' || c == '*' || c == '(' || c == ')');
}

static int ft_atoi(const char *s)
{
	int i = 0;
	int tot = 0;
	while (s[i] >= '0' && s[i] <= '9')
	{
		tot = tot * 10 + s[i] - '0';
		i++;
	}
	return (tot);
}

static void ft_putnbr(int n)
{
	char c;

	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return;
	}
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	if (n > 9)
		ft_putnbr(n / 10);
	c = n % 10 + '0';
	write(1, &c, 1);
}

static int *get_all_int(const char *s)
{
	int i = 0;
	int *buf;
	int k = 0;
	int count = 0;
	int a;

	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			count++;
			while (s[i] >= '0' && s[i] <= '9')
				i++;
		}
		else
			i++;
	}
	if (count == 0)
	{
		char *msg = "Invalid token \'<char>\'\n";
		write(1, msg, 23);
		return 0;
	}
	i = 0;
	buf = (int *)malloc(sizeof(int) * (count + 1));
	while (s[i] && k < count)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			a = ft_atoi(s + i);
			buf[k++] = a;
			while (s[i] >= '0' && s[i] <= '9')
				i++;
		}
		else
			i++;
	}
	return (buf);
}

static int is_valid_expression(char *s)
{
	int i = 0;
	char buf[1024];
	int k = 0;
	int open_p = 0;
	int l = 0;

	while (s[l])
		l++;

	if (!((s[0] >= '0' && s[0] <= '9') || s[0] == '(') || s[0] == 0)
	{
		char *msg = "Invalid token \'<char>\'\n";
		write(1, msg, 23);
		return 0;
	}
	while (s[i])
	{
		if (!is_valid_char(s[i]))
		{
			char *msg = "Invalid token \'<char>\'\n";
			write(1, msg, 23);
			return 0;
		}
		if (s[i] == '(')
		{
			if (i >= 1 && s[i - 1] == ')')
			{
				char *msg = "Invalid token \'<char>\'\n";
				write(1, msg, 23);
				return 0;
			}
			open_p++;
			buf[k] = s[i];
			k++;
		}
		if (s[i] == ')')
		{
			open_p--;
			if (open_p < 0)
			{
				char *msg = "Invalid token \'<char>\'\n";
				write(1, msg, 23);
				return 0;
			}
		}
		if (i == l - 1 && (s[i] == '+' || s[i] == '*'))
		{
			char *msg = "Invalid token \'<char>\'\n";
			write(1, msg, 23);
			return 0;
		}
		i++;
	}
	if (open_p > 0)
	{
		char *msg = "Unexpected end\n";
		write(1, msg, 15);
		return 0;
	}
	return (l);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	
	char *s = argv[1];
	int l = is_valid_expression(s);
	
	if (l == 0)
		return 1;

	int i = 0;
	int *nbrs = get_all_int(s);
	int k = 0;
	int j;
	int tot = 0;

	return (0);
}
*/