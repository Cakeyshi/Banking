#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_SIZE 100

typedef struct S_SignUp {
    char* firstName;
    char* secondName;
    int birthday;
    int mobileNumber;
    char* nationality;
} signUpDetails;

// Function to allocate memory for dynamic members of signUpDetails
void allocateMemory(signUpDetails* account) {
    account->firstName = (char*)malloc(DEFAULT_SIZE * sizeof(char));
    account->nationality = (char*)malloc(DEFAULT_SIZE * sizeof(char));
}

// Function to free the allocated memory
void freeMemory(signUpDetails* account) {
    free(account->firstName);
    free(account->nationality);
}
//make sure to free memory
void f_SignUp() {
    signUpDetails basicAccount;

    printf("Enter your First Name: ");
    allocateMemory(&basicAccount);
    scanf("%s", basicAccount.firstName);
    printf("Enter Surname: ");
    scanf("%s", basicAccount.)
    // Your other input and processing logic here...

    // Don't forget to free the allocated memory when you're done
    freeMemory(&basicAccount);
}

int main() {
    f_SignUp();

    return 0;
}
