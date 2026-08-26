#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* 
On détermine si on peut placer une reine à la position (col, row) sans 
conflit avec les reines déjà posées dans les colonnes précédentes (0 à col - 1). 
*/
static int is_safe(const int *pos, int col, int row)
{
    int c = 0;

    while (c < col) 
    {
        /* Même ligne */
        if (pos[c] == row)
            return 0;
        /* 
        Même diagonale :
        Deux cases (c, pos[c]) et (col, row) sont sur la même diagonale 
        si et seulement si la différence de lignes est égale (en valeur absolue) à la différence de colonnes, cad :
        |pos[c] - row| == |col - c|.
        Ici, col - c est toujours positif car c < col. 
        Mais pos[c] - row peut être positif ou négatif selon que 
        la reine précédente est au-dessus ou en dessous de la ligne envisagée.
        */
        int diff = pos[c] - row;
        if (diff < 0)
            diff = -diff;
        if (diff == col - c)
            return 0;
        c++;
    }
    return 1;
}

static void print_solution(const int *pos, int n)
{
    if (n == 0) 
    {
        write(1, "\n", 1);
        return;
    }

    /*
    Chaque case du tableau pos contient un indice de ligne (un int). 
    Un int sur 32 bits tient sur au plus 10 chiffres, 
    donc 12 par valeur est une marge confortable (10 chiffres + 1 espace + sécurité). 
    Le +2 couvre le \n final et une marge.
    */
    size_t cap = (size_t)n * 12 + 2;
    char *buf = malloc(cap);
    if (!buf)
        return;

    size_t len = 0;
    int i = 0;
    while (i < n) 
    {
        int val = pos[i];
        char tmp[12];
        int t = 0;

        if (val == 0)
            tmp[t++] = '0';
        else 
        {
            while (val > 0) 
            {
                tmp[t++] = (char)('0' + (val % 10));
                val /= 10;
            }
        }
        while (t > 0)
            buf[len++] = tmp[--t];

        if (i != n - 1)
            buf[len++] = ' ';
        i++;
    }
    buf[len++] = '\n';

    write(1, buf, len);
    free(buf);
}

static void solve(int *pos, int col, int n)
{
    /* On a rempli notre pos sans souci [0, n), victoire ! */
    if (col == n) 
    {
        print_solution(pos, n);
        return;
    }
    /* On voit sur quelle ligne poser une reine sur la colonne col*/
    int row = 0;
    while (row < n) 
    {
        if (is_safe(pos, col, row)) 
        {
            pos[col] = row;
            solve(pos, col + 1, n);
        }
        row++;
    }
}

/* Pour print joliement le résultat */
static void print_board(const int *pos, int n)
{
    if (n <= 0)
        return;

    /* Une ligne de separation : "+---" repete n fois, puis "+\n" */
    size_t sep_len = (size_t)n * 4 + 2;
    char *sep = malloc(sep_len);
    if (!sep)
        return;

    size_t p = 0;
    for (int c = 0; c < n; c++) {
        sep[p++] = '+';
        sep[p++] = '-';
        sep[p++] = '-';
        sep[p++] = '-';
    }
    sep[p++] = '+';
    sep[p++] = '\n';

    /* Une ligne de contenu : "|   " ou "| Q " repete n fois, puis "|\n" */
    size_t row_len = (size_t)n * 4 + 2;
    char *row_buf = malloc(row_len);
    if (!row_buf) {
        free(sep);
        return;
    }

    for (int r = 0; r < n; r++) {
        write(1, sep, sep_len);

        size_t q = 0;
        for (int c = 0; c < n; c++) {
            row_buf[q++] = '|';
            row_buf[q++] = ' ';
            row_buf[q++] = (pos[c] == r) ? 'Q' : ' ';
            row_buf[q++] = ' ';
        }
        row_buf[q++] = '|';
        row_buf[q++] = '\n';

        write(1, row_buf, row_len);
    }

    write(1, sep, sep_len);

    free(row_buf);
    free(sep);
}

int main(int argc, char **argv)
{
    if (argc != 2) 
    {
        /*
        NB :
        printf(...) est en réalité équivalent à fprintf(stdout, ...) : 
        printf écrit toujours sur la sortie standard, 
        alors que fprintf permet de choisir n'importe quel flux 
        (ex : la sortie standard (stdout), la sortie d'erreur (stderr), ou même un fichier ouvert avec fopen).
        */
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    if (n < 0)
        return 0;

    if (n == 0) 
    {
        print_solution(NULL, 0);
        return 0;
    }

    int *pos = malloc(sizeof(int) * (size_t)n);
    if (!pos)
        return 1;

    solve(pos, 0, n);

    free(pos);
    return 0;
}