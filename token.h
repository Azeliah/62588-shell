#ifndef TOKEN_H
#define TOKEN_H

char **token_array(const char *input_string, size_t input_length, char delimiter);

char **tokenize(char *input_string, char delimiter);

#endif // TOKEN_H
