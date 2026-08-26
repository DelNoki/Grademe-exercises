#include <unistd.h>

/* 
Améliorations : 
- prédéfinir la longueur idéale d'un plateau d'échec : enlever get_row_len et get_rows
- 
*/

static int	ft_strlen(const char *s)
{
	int	l = 0;

	while (s[l] != '\0')
		l++;
	return (l);
}

static void	ft_putstr(int fd, const char *s)
{
	write(fd, s, ft_strlen(s));
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

/* Length of the first row = index of the first '\n' */
static int	get_row_len(const char *board)
{
	int	i = 0;

	while (board[i] != '\0' && board[i] != '\n')
		i++;
	return (i);
}

/* Number of rows */
static int	get_rows(const char *board, int row_len)
{
	int	i = 0;
	int	rows = 0;

	while (board[i] != '\0')
	{
		rows++;
		i += row_len;
		if (board[i] == '\n')
			i++;
	}
	return (rows);
}

/* returns board[r][c] if ok and '\0' if out of bounds */
static char	at(const char *board, int row_len, int rows, int r, int c)
{
	if (r < 0 || c < 0 || r >= rows || c >= row_len)
		return ('\0');
	return (board[r * (row_len + 1) + c]);
}

static int	find_king(const char *board, int row_len, int rows,
		int *kr, int *kc)
{
	int	r = 0;
	int	c;

	while (r < rows)
	{
		c = 0;
		while (c < row_len)
		{
			if (at(board, row_len, rows, r, c) == 'K')
			{
				*kr = r;
				*kc = c;
				return (1);
			}
			c++;
		}
		r++;
	}
	return (0);
}

/* Checks the path strictly between two aligned squares is empty */
static int	is_clear_line(const char *board, int row_len, int rows,
		int r1, int c1, int r2, int c2)
{
	int	dr;
	int	dc;

	dr = (r2 > r1) - (r2 < r1);
	dc = (c2 > c1) - (c2 < c1);
	r1 += dr;
	c1 += dc;
	while (r1 != r2 || c1 != c2)
	{
		if (at(board, row_len, rows, r1, c1) != '.')
			return (0);
		r1 += dr;
		c1 += dc;
	}
	return (1);
}

/* faire des dessins ... */
static int	piece_attacks_king(char piece, int pr, int pc, int kr, int kc,
		const char *board, int row_len, int rows)
{
	int	dr = kr - pr;
	int	dc = kc - pc;

	if (piece == 'R')
		return ((dr == 0 || dc == 0)
			&& is_clear_line(board, row_len, rows, pr, pc, kr, kc));
	if (piece == 'B')
		return ((ft_abs(dr) == ft_abs(dc))
			&& is_clear_line(board, row_len, rows, pr, pc, kr, kc));
	if (piece == 'Q')
		return ((dr == 0 || dc == 0 || ft_abs(dr) == ft_abs(dc))
			&& is_clear_line(board, row_len, rows, pr, pc, kr, kc));
	if (piece == 'N')
		return ((ft_abs(dr) == 2 && ft_abs(dc) == 1)
			|| (ft_abs(dr) == 1 && ft_abs(dc) == 2));
	if (piece == 'P')
		return (dr == -1 && ft_abs(dc) == 1);
	if (piece == 'K')
		return (ft_abs(dr) <= 1 && ft_abs(dc) <= 1 && (dr != 0 || dc != 0));
	return (0);
}

static int	king_in_check(const char *board, int row_len, int rows,
		int kr, int kc)
{
	int		r = 0;
	int		c;
	char	piece;

	while (r < rows)
	{
		c = 0;
		while (c < row_len)
		{
			piece = at(board, row_len, rows, r, c);
			if (piece != '.' && piece != 'K' && piece != '\0'
				&& piece_attacks_king(piece, r, c, kr, kc,
					board, row_len, rows))
				return (1);
			c++;
		}
		r++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int	row_len;
	int	rows;
	int	kr;
	int	kc;

	if (argc != 2)
		return (0);
	row_len = get_row_len(argv[1]);
	rows = get_rows(argv[1], row_len);
	/* NB: dans la ligne qui suit, les valeurs de kr et de kc vont être modifiées si le roi est trouvé sur la grille !!! */
	if (!find_king(argv[1], row_len, rows, &kr, &kc))
		return (0);
	if (king_in_check(argv[1], row_len, rows, kr, kc))
		ft_putstr(1, "Success\n");
	else
		ft_putstr(1, "Fail\n");
	return (0);
}