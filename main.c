#include <stdio.h>
#include <string.h>

int main(void){

    char input_buffer[128];
    size_t input_length;

    while(1) {
       fgets(input_buffer, 128, stdin); 

       input_length = strlen(input_buffer);
       input_buffer[--input_length] = '\0';

       printf("%s\n", input_buffer);

       if (strcmp(input_buffer, "exit") == 0)
           break;
    }

    return 0;
}
