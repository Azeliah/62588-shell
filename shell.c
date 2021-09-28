#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

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

        char **tokens_left_of_pipe = tokenize(pipe_tokens[0], ' ');
        char **tokens_right_of_pipe = tokenize(pipe_tokens[1], ' ');

        // Try to fork, report error and exit if it fails
        int rc = fork();
        if (rc < 0) {
            perror("fork");
            return 1;
        }

        // If we are in child process, execute command
        if (rc == 0) {

            if (tokens_right_of_pipe) {

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
                    close(pipefd[0]);
                    dup2(pipefd[1], STDOUT_FILENO);

                    if (execvp(tokens_left_of_pipe[0], tokens_left_of_pipe) < 0) {
                       fprintf(stderr, "execvp: %s: %s\n", tokens_left_of_pipe[0], strerror(errno));
                        return 1;
                    }

                } else {
                    close(pipefd[1]);
                    dup2(pipefd[0], STDIN_FILENO);

                    if (execvp(tokens_right_of_pipe[0], tokens_right_of_pipe) < 0) {
                       fprintf(stderr, "execvp: %s: %s\n", tokens_right_of_pipe[0], strerror(errno));
                       return 1;
                    }
                }

            } else {

                // Report error, if any
                if (execvp(tokens_left_of_pipe[0], tokens_left_of_pipe) < 0) {
                    fprintf(stderr, "execvp: %s: %s\n", tokens_left_of_pipe[0], strerror(errno));
                    return 1;
                }
            }

        } else { // If we are in parent process, wait for child process to execute and continue with loop
            wait(NULL);
        }

        // Free token arrays to prevent memory leak
        free(tokens_left_of_pipe);
        if (tokens_right_of_pipe)
            free(tokens_right_of_pipe);
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
