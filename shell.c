#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#include "shell.h"

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

        char *pipe_tokens[2];

        pipe_tokens[0] = strtok(input_buffer, "|");
        pipe_tokens[1] = strtok(NULL, "|");

        char **tokens_left = token_array(pipe_tokens[0], strlen(pipe_tokens[0]), ' ');

        // Fill array with tokens from input string
        char *token = strtok(pipe_tokens[0], " ");
        for(int i = 0; token != NULL; ++i) {
            tokens_left[i] = token;

            token = strtok(NULL, " ");
        }


        char **tokens_right = NULL;

        if (pipe_tokens[1]) {

            tokens_right = token_array(pipe_tokens[1], strlen(pipe_tokens[1]), ' ');

            token = strtok(pipe_tokens[1], " ");

            for (int i = 0; token != NULL; ++i) {
                tokens_right[i] = token;

                token = strtok(NULL, " ");
            }

        }


        // Try to fork, report error and exit if it fails
        int rc = fork();
        if (rc < 0) {
            perror("fork");
            return 1;
        }

        // If we are in child process, execute command
        if (rc == 0) {

            if (tokens_right) {

                int pipefd[2];
                if (pipe(pipefd) < 0) {
                    perror("pipe");
                    return 1;
                }


                rc = fork();
                if (rc < 0) {
                    perror("fork");
                    return 1;
                }

                if (rc == 0) {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[0]);

                    if (execvp(tokens_left[0], tokens_left) < 0) {
                        perror("execvp");
                        return 1;
                    }

                } else {
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[1]);

                    if (execvp(tokens_right[0], tokens_right) < 0) {
                        perror("execvp");
                        return 1;
                    }
                }


            } else {

                // Report error, if any
                if (execvp(tokens_left[0], tokens_left) < 0)
                    perror("execvp");
            }

        } else { // If we are in parent process, wait for child process to execute and continue with loop
            wait(NULL);
        }

        // Free token arrays to prevent memory leak
        free(tokens_left);
        if (tokens_right)
            free(tokens_right);
    }

    return 0;
}

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
