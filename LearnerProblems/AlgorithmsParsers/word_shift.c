#include <unistd.h>
#include <stdlib.h>

void write_char(char c) 
{
    write(1, &c, 1);
}

// Writes buf[start..end) to stdout, collapsing spaces into one
void write_collapsed(char *buf, int start, int end, int *last_was_space) 
{
    int j = start;

    while (j < end) 
    {
        if (buf[j] == ' ') 
        {
            if (!*last_was_space) 
            {
                write_char(' ');
                *last_was_space = 1;
            }
        } 
        else 
        {
            write_char(buf[j]);
            *last_was_space = 0;
        }
        j++;
    }
}

int main(int argc, char **argv) 
{
    if (argc != 2) 
    {
        write(1, "wrong number of arguments\n", 26);
        return 0;
    }

    char *input = argv[1];

    // Manually compute length (no strlen)
    int len = 0;
    while (input[len] != '\0')
        len++;

    // Trim leading spaces
    int start = 0;
    while (start < len && input[start] == ' ')
        start++;

    // Trim trailing spaces
    int end = len;
    while (end > start && input[end - 1] == ' ')
        end--;

    if (start == end)
    {
        // String was all whitespace (or empty)
        write_char('\n');
        return 0;
    }

    // Find the end of the first word within [start, end)
    int i = start;
    while (i < end && input[i] != ' ')
        i++;

    if (i == end) 
    {
        // Only one word, nothing to rotate
        int last_was_space = 0;
        write_collapsed(input, start, end, &last_was_space);
        write_char('\n');
        return 0;
    }

    // Skip space(s) after the first word
    int rest_start = i;
    while (rest_start < end && input[rest_start] == ' ')
        rest_start++;

    // Write the rest of the string, a single space, then the first word
    int last_was_space = 0;
    write_collapsed(input, rest_start, end, &last_was_space);
    write_char(' ');
    write_collapsed(input, start, i, &last_was_space);
    write_char('\n');

    return 0;
}