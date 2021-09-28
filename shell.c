#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_BUFFER_LENGTH 128

int main(void) {

    // Buffer of fixed length for input strings
    char input_buffer[INPUT_BUFFER_LENGTH];

    // Go in infinite loop, receiving input from user and executing it
    while(true) {

        // Get user input from stdin, report error and exit if it fails
        if (!fgets(input_buffer, INPUT_BUFFER_LENGTH, stdin)) {
            fprintf(stderr, "fgets failed\n");
            return 1;
        }

        // Remove newline from end of input string
        size_t input_length = strlen(input_buffer);
        input_buffer[--input_length] = '\0';

        /*
         * Find the necessary token array size based on number of delimiters in input string.
         * token_array_size starts at 2 because there needs to be space for the NULL terminator,
         * and because the last token does not have delimiter.
         */
        int token_array_size = 2;
        for (int i = 0; i < input_length; ++i) {
            if (input_buffer[i] == ' ')
                ++token_array_size;
        }

        // Dynamically allocate array of correct size
        char *tokens[token_array_size];

        // Initialize all elements in the array to NULL to prevent undefined behavior, in case of fucky input
        for (int i = 0; i < token_array_size; ++i)
            tokens[i] = NULL;

        // Fill array with tokens from input string
        char *token = strtok(input_buffer, " ");
        for(int i = 0; token != NULL; ++i) {
            tokens[i] = token;

            token = strtok(NULL, " ");
        }

        // Try to fork, report error and exit if it fails
        int rc = fork();
        if (rc < 0) {
            perror("fork");
            return 1;
        }

        // If we are in child process, execute command
        if (rc == 0) {

            // Report error, if any
            if (execvp(tokens[0], tokens) < 0)
                perror("execvp");

        } else { // If we are in parent process, wait for child process to execute and continue with loop
            wait(NULL);
        }
    }

    return 0;
}
