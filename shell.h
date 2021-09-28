#ifndef SHELL_H
#define SHELL_H

#define INPUT_BUFFER_LENGTH 128

char **token_array(const char *input_string, size_t input_length, char delimiter);

#endif // SHELL_H
