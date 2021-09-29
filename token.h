#ifndef TOKEN_H
#define TOKEN_H

void tokenize(char *input_string, char *delimiter, char **token_array);

int token_array_size(const char *input_string, char delimiter);

#endif // TOKEN_H
