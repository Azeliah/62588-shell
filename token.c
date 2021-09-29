#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "token.h"

void tokenize(char *input_string, char *delimiter, char **token_array) {
    if (!token_array)
        return;

    // If the string is NULL, set the first element in the array to NULL, to indicate no tokens
    if (!input_string) {
        token_array[0] = NULL;
        return;
    }

    // Fill array with tokens from input string
    token_array[0] = strtok(input_string, delimiter);
    for (int i = 0; token_array[i];)
        token_array[++i] = strtok(NULL, delimiter);
}

int token_array_size(const char *input_string, char delimiter) {

    // If the string is NULL or of length 0, the array needs to have only one element (NULL terminator)
    if (!input_string || input_string[0] == '\0')
        return 1;

    // Skip all delimiters at the end of the string
    int end = strlen(input_string) - 1;
    while (input_string[end] == delimiter && end > 0)
        --end;

    // Skip all delimiters at the beginning of the string
    int beginning = 0;
    while (input_string[beginning] == delimiter && beginning > end)
        ++beginning;
 
    // size starts at 2, because there needs to be space for the NULL terminator,
    // and because the last token does not have delimiter.
    int size = 2;
    for (int i = beginning; i < end; ++i) {

        if (input_string[i] == delimiter) {
            ++size;

            // When a delimiter is found, ignore any potential duplicates
            do {
                ++i;
            } while (input_string[i] == delimiter && i < end);
        }
    }

    return size;
}
