#include<stdio.h>
#include<stdlib.h>

void printInt(const char* printline, unsigned long long int* container, size_t size)
{
    char inputBuffer[100];  // Adjust the size accordingly
    do {
        printf(printline);
        fgets(inputBuffer, sizeof(inputBuffer), stdin);

        // Convert input string to unsigned long long int
        char* endptr;
        *container = strtoull(inputBuffer, &endptr, 10);

        // Check if conversion was successful
        if (*endptr != '\n' && *endptr != '\0') {
            printf("Invalid Input, please enter an integer!\n");
        }
        else {
            break;
        }
    } while (1);
}

int main()
{
    unsigned long long int num = 0;
    printInt("Please enter a number: ", &num, sizeof(num));
    printf("You entered the number: %llu\n", num);
    return 0;
}
