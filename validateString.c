#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


int isString(const char* input)
{
    for(int i = 0; i < strlen(input); i++)
    {
        if(!isalpha(input[i]) && !isspace(input[i]))
        {
            return 0;
        }
    }
    return 1;
}

void print(const char* printline, char* container, size_t size)
{
    size_t len;
    do{
        printf(printline);
        fgets(container, size, stdin);

        len = strcspn(container, "\n");
        if(len > 0 && container[len - 1] == '\n')
        {
            container[len - 1] = '\0';
        }

        if(len == 0)
        {
            printf("Empty input, please try again\n");
        }
        else if(!isString(container))
        {
            printf("Invalid input, please try again\n");
        }

    }while(!isString(container) || len == 0);
}

int main()
{
    char* s_string = malloc(100);
    print("Enter a string: ", s_string, 100);
    printf("You entered: %s", s_string);

    return 0;
}
