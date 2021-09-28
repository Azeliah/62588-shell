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
        size_t input_length = strlen(input_buffer);
        input_buffer[--input_length] = '\0';

        // If the input is "exit", quit the infinite loop
        if (strcmp(input_buffer, "exit") == 0)
            break;

        // Tokenize input string based on pipe char in case of piping
        char **pipe_tokens = tokenize(input_buffer, '|');

        // Tokenize commands on either side of pipe char
        char **tokens_left_of_pipe = tokenize(pipe_tokens[0], ' ');
        char **tokens_right_of_pipe = tokenize(pipe_tokens[1], ' ');

        // Try to fork, report error and exit if it fails
        int rc = fork();
        if (rc < 0) {
            perror("fork");
            exit(1);
        }

        // If we are in child process, execute command
        if (rc == 0) {

            // In case the command involves piping, tokens_right_of_pipe is not NULL
            if (tokens_right_of_pipe) {

                // Create pipe using  array of file descriptors, report error and exit if it fails
                int pipe_file_descriptors[2];
                if (pipe(pipe_file_descriptors) < 0) {
                    perror("pipe");
                    exit(1);
                }

                // Try to fork, report error adn rexit if it fails
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

        // Free token arrays to prevent memory leak
        free(pipe_tokens);
        free(tokens_left_of_pipe);
        if (tokens_right_of_pipe)
            free(tokens_right_of_pipe);
    }

    return 0;
}
