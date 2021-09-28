#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

#include "token.h"

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


