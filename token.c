#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

char **token_array(const char *input_string, size_t input_length, char delimiter) {
    if (!input_string || input_length == 0)
        return NULL;

     /*
     * Find the necessary token array size based on number of delimiters in input string.
     * token_array_size starts at 2 because there needs to be space for the NULL terminator,
     * and because the last token does not have delimiter.
     */
    size_t token_array_size = 2;
    for (int i = 0; i < input_length; ++i) {
        if (input_string[i] == ' ')
            ++token_array_size;
    }

    // Dynamically allocate array of correct size
    char **tokens = malloc(sizeof(char *) * token_array_size);

    // Initialize all elements in the array to NULL to prevent undefined behavior, in case of fucky input
    for (int i = 0; i < token_array_size; ++i)
        tokens[i] = NULL;

    return tokens;
}

char **tokenize(char *input_string, char delimiter) {
    if (!input_string)
        return NULL;

    char **tokens = token_array(input_string, strlen(input_string), delimiter);

    char delimiter_string[2];
    delimiter_string[0] = delimiter;
    delimiter_string[1] = '\0';

    // Fill array with tokens from input string
    char *token = strtok(input_string, delimiter_string);
    for(int i = 0; token != NULL; ++i) {
        tokens[i]  = token;

        token = strtok(NULL, delimiter_string);
    }
    
    return tokens;
}
