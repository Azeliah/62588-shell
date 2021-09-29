#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
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
            exit(1);
        }

        // Remove newline from end of input string
        input_buffer[strlen(input_buffer) - 1] = '\0';

        // If the input is "exit", quit the infinite loop
        if (strcmp(input_buffer, "exit") == 0)
            break;

        // Dynamically allocate array of correct size for commands seperated by pipes
        char *pipe_tokens[token_array_size(input_buffer, '|')];

        // Tokenize input string based on pipe char in case of piping
        tokenize(input_buffer, "|", pipe_tokens);

        // Tokenize commands on either side of pipe char
        char *tokens_left_of_pipe[token_array_size(pipe_tokens[0], ' ')];
        tokenize(pipe_tokens[0], " ", tokens_left_of_pipe);

        char *tokens_right_of_pipe[token_array_size(pipe_tokens[1], ' ')];
        tokenize(pipe_tokens[1], " ", tokens_right_of_pipe);

        // Try to fork, report error and exit if it fails
        int rc = fork();
        if (rc < 0) {
            perror("fork");
            exit(1);
        }

        // If we are in child process, execute command
        if (rc == 0) {

            // In case the command involves piping, tokens_right_of_pipe[0] is not NULL
            if (tokens_right_of_pipe[0]) {

                // Create pipe using  array of file descriptors, report error and exit if it fails
                int pipe_file_descriptors[2];
                if (pipe(pipe_file_descriptors) < 0) {
                    perror("pipe");
                    exit(1);
                }

                // Try to fork, report error and exit if it fails
                rc = fork();
                if (rc < 0) {
                    perror("fork");
                    exit(1);
                }

                // If we are in child process, write to pipe
                if (rc == 0) {

                    // Close unused read end of pipe
                    close(pipe_file_descriptors[0]);

                    // Call dup2 so the child writes to stdout
                    dup2(pipe_file_descriptors[1], STDOUT_FILENO);

                    // Execute command, report error if it fails
                    if (execvp(tokens_left_of_pipe[0], tokens_left_of_pipe) < 0) {
                        fprintf(stderr, "execvp: %s: %s\n", tokens_left_of_pipe[0], strerror(errno));
                        exit(1);
                    }

                } else { // If we are in parent process, read from pipe

                    // Close unused write end of pipe
                    close(pipe_file_descriptors[1]);

                    // Call dup2 so the parent reads from stdin
                    dup2(pipe_file_descriptors[0], STDIN_FILENO);

                    // Execute command, report error if it fails
                    if (execvp(tokens_right_of_pipe[0], tokens_right_of_pipe) < 0) {
                        fprintf(stderr, "execvp: %s: %s\n", tokens_right_of_pipe[0], strerror(errno));
                        exit(1);
                    }
                }

            } else { // The command includes no piping

                // Execute command, report error if it fails
                if (execvp(tokens_left_of_pipe[0], tokens_left_of_pipe) < 0) {
                    fprintf(stderr, "execvp: %s: %s\n", tokens_left_of_pipe[0], strerror(errno));
                    exit(1);
                }
            }

        } else { // If we are in parent process, wait for child process to finish and continue with loop
            wait(NULL);
        }
    }

    return 0;
}
