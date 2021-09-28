#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_BUFFER_LENGTH 128

int main(void) {

    char input_buffer[INPUT_BUFFER_LENGTH];
    size_t input_length;
    char *token;

    while(true) {
        fgets(input_buffer, INPUT_BUFFER_LENGTH, stdin); 

        // Remove newline from end of input string
        input_length = strlen(input_buffer);
        input_buffer[--input_length] = '\0';

        // Count how many delimiters are in the input, for token array length
        // tokens_array_size starts at 2 because there needs to be space for the NULL terminator
        int tokens_array_size = 2;
        for (int i = 0; i < input_length; ++i) {
            if (input_buffer[i] == ' ')
                ++tokens_array_size;
        }
        printf("Tokens array size: %d\n", tokens_array_size);

        /*
         * FIXME: In case there are no tokens in the input string, 
         *        the first element in the tokens array never gets assigned, 
         *        which can lead to undefined behaviour.
         *        This is why the first element in the array gets assigned to NULL,
         *        Even though its most likely going to be overwritten.
         */
        char *tokens[tokens_array_size];
        tokens[tokens_array_size - 1] = tokens[0] = NULL;

        token = strtok(input_buffer, " ");

        for(int i = 0; token; ++i) {
            tokens[i] = token;

            token = strtok(NULL, " ");
        }

        for (int i = 0; i < tokens_array_size; ++i)
            printf("tokens[%d] = %s\n", i,  tokens[i] ? tokens[i] : "NULL");

        if (strcmp(input_buffer, "exit") == 0)
            break;
    }

    return 0;
}
