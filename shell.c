#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_BUFFER_LENGTH 128

int main(void) {

    char input_buffer[INPUT_BUFFER_LENGTH];

    while(true) {

        // Get user input from stdin
        if (!fgets(input_buffer, INPUT_BUFFER_LENGTH, stdin)) {
            fprintf(stderr, "fgets failed\n");
            return 1;
        }

        // Remove newline from end of input string
        size_t input_length = strlen(input_buffer);
        input_buffer[--input_length] = '\0';

        // Count how many delimiters are in the input, for token array size
        // tokens_array_size starts at 2 because there needs to be space for the NULL terminator
        int tokens_array_size = 2;
        for (int i = 0; i < input_length; ++i) {
            if (input_buffer[i] == ' ')
                ++tokens_array_size;
        }

        char *tokens[tokens_array_size];

        // Initialize all elements in the array to NULL to prevent undefined behavior
        for (int i = 0; i < tokens_array_size; ++i)
            tokens[i] = NULL;

        char *token = strtok(input_buffer, " ");

        for(int i = 0; token; ++i) {
            tokens[i] = token;

            token = strtok(NULL, " ");
        }

        int rc = fork();
        if (rc < 0) {
            perror("fork");
            return 1;
        }

        if (rc == 0) {
            if (execvp(tokens[0], tokens) < 0)
                perror("execvp");

        } else {
            wait(NULL);
        }
    }

    return 0;
}
