#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Sizes
#define MAX_USERS 100
#define DEFAULT_DATA_SIZE 100

// Sign up and Login
typedef struct S_SignUp {
    char* firstName;
    char* lastName;
    char* middleName;
    int birth_year, birth_month, birth_day;
    int mobileNumber;
    char* nationality;
} signUpDetails;

// Function declarations
void allocateMemory(signUpDetails* account);
void freeMemory(signUpDetails* account);
void f_SignUp();
int ageValidator(signUpDetails* basicAccount);

int main() {
    f_SignUp();
    return 0;
}

int ageValidator(signUpDetails* basicAccount) {
    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);

    // Input the birthdate
    printf("Enter birth year: ");
    scanf("%d", &basicAccount->birth_year);
    printf("Enter birth month: ");
    scanf("%d", &basicAccount->birth_month);
    printf("Enter birth day: ");
    scanf("%d", &basicAccount->birth_day);

    // Compare birthdate with current date
    int age = current_time->tm_year + 1900 - basicAccount->birth_year;

    // Adjust age based on birth month and day
    if (current_time->tm_mon + 1 < basicAccount->birth_month ||
        (current_time->tm_mon + 1 == basicAccount->birth_month && current_time->tm_mday < basicAccount->birth_day)) {
        age--;
    }

    // Display age (you might want to use this information)
    return age;
}

void allocateMemory(signUpDetails* account) {
    account->firstName = (char*)malloc(DEFAULT_DATA_SIZE * sizeof(char));
    account->lastName = (char*)malloc(DEFAULT_DATA_SIZE * sizeof(char));
    account->middleName = (char*)malloc(DEFAULT_DATA_SIZE * sizeof(char));
    account->nationality = (char*)malloc(DEFAULT_DATA_SIZE * sizeof(char));
}

void freeMemory(signUpDetails* account) {
    free(account->firstName);
    free(account->lastName);
    free(account->middleName);
    free(account->nationality);
}

// TODO: Have all necessary input details here
void f_SignUp() {
    FILE* file;
    file = fopen("Accounts.txt", "a");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    signUpDetails basicAccount;
    allocateMemory(&basicAccount);

    // Name Input
    printf("Enter your First Name: ");
    // Use fgets for input to capture spaces
    fgets(basicAccount.firstName, DEFAULT_DATA_SIZE, stdin);
    basicAccount.firstName[strcspn(basicAccount.firstName, "\n")] = '\0'; // Remove the newline character if present

    printf("Enter your Last Name: ");
    scanf("%s", basicAccount.lastName);

    printf("Enter your Middle Name: ");
    scanf("%s", basicAccount.middleName);

    // Birthday Input
    int age = ageValidator(&basicAccount);

    // Nationality Input
    printf("Enter your Nationality: ");
    scanf("%s", basicAccount.nationality);

    // Phone number input
    printf("Enter your Phone Number: ");
    scanf("%d", &basicAccount.mobileNumber); // Assuming mobileNumber is an integer, adjust accordingly

    fprintf(file, "Full Name: %s %s %s\n", basicAccount.firstName, basicAccount.middleName, basicAccount.lastName);
    fprintf(file, "Birthdate: %d-%d-%d\n", basicAccount.birth_year, basicAccount.birth_month, basicAccount.birth_day);
    fprintf(file, "Age: %d\n", age);
    fprintf(file, "Nationality: %s\n", basicAccount.nationality);
    fprintf(file, "Phone Address: %d\n", basicAccount.mobileNumber);
    fclose(file);

    printf("User signed up successfully!\n");

    // Don't forget to free the allocated memory when you're done
    freeMemory(&basicAccount);
}

