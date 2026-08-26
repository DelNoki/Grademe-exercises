/*
 * mini_scanf.c
 *
 * A reduced formatted-input function reading from standard input.
 *
 * Supported conversions: %d, %s, %c
 * Supported format elements: literal characters, whitespace (matches
 * any amount of input whitespace, including none), and the three
 * conversions above.
 *
 * Allowed library calls only: fgets, ungetc, ferror, feof, isspace,
 * isdigit, va_start, va_arg, va_copy, va_end.
 *
 * Single-byte input is read with fgetc(); ungetc() is used to push a
 * byte back onto the stream so the next fgetc() call sees it again.
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

/* Read exactly one byte from stream. Returns the byte as an unsigned
 * char cast to int, or EOF if no byte is available (end of file or
 * error) -- mini_scanf() only needs to know "no more bytes
 * available", so the two cases aren't distinguished here. */
static int read_byte(FILE *stream)
{
    return fgetc(stream);
}

/* Push a previously read byte back onto stream. */
static void unread_byte(int c, FILE *stream)
{
    if (c != EOF)
        ungetc(c, stream);
}

/* Consume zero or more whitespace bytes from stream, leaving the
 * first non-whitespace byte (or EOF) unread. */
static void skip_whitespace(FILE *stream)
{
    int c;

    while ((c = read_byte(stream)) != EOF && isspace((unsigned char) c)) 
    {
        /* consume */
    }

    if (c != EOF)
        unread_byte(c, stream);
}

/*
 * mini_scanf - minimal formatted input.
 *
 * Returns the number of successful conversions performed. Returns
 * EOF only if input ended before a single conversion could be made
 * (i.e. count is still 0 when EOF is hit).
 */
int mini_scanf(const char *format, ...)
{
    va_list args;
    const char *f = format;
    int count = 0;
    int c;

    va_start(args, format);

    while (*f != '\0') 
    {

        /* --- Whitespace in the format: match any amount (>= 0) --- */
        if (isspace((unsigned char) *f)) 
        {
            skip_whitespace(stdin);
            f++;
            continue;
        }

        /* --- Conversion specifier --- */
        if (*f == '%') 
        {
            char conv;

            f++;
            if (*f == '\0') 
            {
                /* Malformed trailing '%': nothing more to do. */
                break;
            }
            conv = *f;
            f++;

            if (conv == 'd') 
            {
                int *out = va_arg(args, int *);
                long value = 0;
                int digits = 0;
                int negative = 0;

                skip_whitespace(stdin);

                c = read_byte(stdin);
                if (c == '-' || c == '+') 
                {
                    negative = (c == '-');
                    c = read_byte(stdin);
                }

                while (c != EOF && isdigit((unsigned char) c)) 
                {
                    value = value * 10 + (c - '0');
                    digits++;
                    c = read_byte(stdin);
                }
                unread_byte(c, stdin);

                if (digits == 0) 
                {
                    /* No digit could be read: conversion fails. */
                    if (count == 0 && c == EOF) 
                    {
                        va_end(args);
                        return EOF;
                    }
                    va_end(args);
                    return count;
                }

                *out = negative ? -(int) value : (int) value;
                count++;

            } 
            else if (conv == 's') 
            {
                char *out = va_arg(args, char *);
                int n = 0;

                skip_whitespace(stdin);

                c = read_byte(stdin);
                while (c != EOF && !isspace((unsigned char) c)) 
                {
                    out[n++] = (char) c;
                    c = read_byte(stdin);
                }
                unread_byte(c, stdin);
                out[n] = '\0';

                if (n == 0) 
                {
                    if (count == 0 && c == EOF) 
                    {
                        va_end(args);
                        return EOF;
                    }
                    va_end(args);
                    return count;
                }
                
                count++;

            } 
            else if (conv == 'c') 
            {
                char *out = va_arg(args, char *);

                /* %c does NOT skip leading whitespace. */
                c = read_byte(stdin);
                if (c == EOF) 
                {
                    if (count == 0) 
                    {
                        va_end(args);
                        return EOF;
                    }
                    va_end(args);
                    return count;
                }

                *out = (char) c;
                count++;

            } 
            else 
            {
                /* Unsupported conversion: stop parsing. */
                break;
            }

        } 
        else 
        {
            /* --- Literal character: must match next input byte --- */
            c = read_byte(stdin);
            if (c == EOF) 
            {
                if (count == 0) 
                {
                    va_end(args);
                    return EOF;
                }
                va_end(args);
                return count;
            }

            if ((char) c != *f)
            {
                unread_byte(c, stdin);
                va_end(args);
                return count;
            }

            f++;
        }
    }

    va_end(args);
    return count;
}

#ifdef MINI_SCANF_TEST
/* Small manual test harness. Build with:
 *   cc -DMINI_SCANF_TEST -o test mini_scanf.c
 * This is a demo driver only; mini_scanf() itself uses only the
 * functions listed above.
 */
#include <stdio.h>

int main(void)
{
    int a, b;
    char word[64];
    char ch;

    int n = mini_scanf("%d,%d", &a, &b);
    printf("n=%d a=%d b=%d\n", n, a, b);

    n = mini_scanf(" %s", word);
    printf("n=%d word=%s\n", n, word);

    n = mini_scanf("%c", &ch);
    printf("n=%d ch=%c\n", n, ch);

    return 0;
}
#endif