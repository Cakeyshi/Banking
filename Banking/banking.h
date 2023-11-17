#ifndef BANKING_H
#define BANKING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Sizes
#define MAX_USERS 100
#define DEFAULT_DATA_SIZE 100
#define FILENAME "Accounts.txt"

// Size for fgets
extern char buffer[DEFAULT_DATA_SIZE];

// Sign up and Login
typedef struct S_SignUp {
    char *firstName;
    char *lastName;
    char *middleName;
    int birth_year, birth_month, birth_day;
    int mobileNumber;
    char *nationality;
    char *accountNumber;
    char pinNumber[7];
} signUpDetails;

// Function declarations
// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to read an integer using fgets
void readInt(int *num) {
    char inputBuffer[100];
    do {
        fgets(inputBuffer, sizeof(inputBuffer), stdin);
        if (sscanf(inputBuffer, "%d", num) == 1) {
            break;
        } else {
            printf("Invalid Input, please enter an integer!\n");
        }
    } while (1);
}

// Function to read a string using fgets
void readString(char *str, size_t size) {
    do {
        fgets(str, size, stdin);
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        }
        if (len == 0) {
            printf("Empty input, please try again\n");
        }
    } while (strlen(str) == 0);
}

// Main Screen (mainly for sign up and Log in)
void f_Main_Screen() {
    int choice;

    do {
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer(); // Clear the input buffer to prevent infinite loop
            getchar();         // Wait for Enter before re-entering the loop
            continue;
        }
        clearInputBuffer(); // Clear the input buffer after successfully reading the choice

        switch (choice) {
            case 1:
                f_SignUp();
                break;
            case 2:
                f_Login();
                break;
            case 3:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 3);
}

// Age validator function
int ageValidator(signUpDetails *basicAccount) {
    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);

    int isValidInput = 0;

    do {
        // Using fgets and parsing it into an integer using sscanf with getchar fixes the newline problem
        // Input the birthdate
        printf("Enter birth year: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, " %d", &basicAccount->birth_year) != 1 || basicAccount->birth_year < 1900 ||
            basicAccount->birth_year > current_time->tm_year + 1900) {
            if (basicAccount->birth_year > current_time->tm_year + 1900) {
                printf("Invalid input. Birth year cannot be in the future.\n");
            } else {
                printf("Invalid input. Please enter a valid year between 1900 and %d.\n",
                       current_time->tm_year + 1900);
            }
            continue; // Repeat the loop to ask again
        }

        // Input the birth month
        printf("Enter birth month: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, " %d", &basicAccount->birth_month) != 1 || basicAccount->birth_month < 1 ||
            basicAccount->birth_month > 12) {
            printf("Invalid input. Please enter a valid month (1-12).\n");
            continue; // Repeat the loop to ask again
        }

        // Input the birth day
        printf("Enter birth day: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, " %d", &basicAccount->birth_day) != 1 || basicAccount->birth_day < 1 ||
            basicAccount->birth_day > 31 ||
            (basicAccount->birth_day > 30 &&
             (basicAccount->birth_month == 4 || basicAccount->birth_month == 6 || basicAccount->birth_month == 9 ||
              basicAccount->birth_month == 11)) ||
            (basicAccount->birth_day > 28 && basicAccount->birth_month == 2 &&
             !((basicAccount->birth_year % 4 == 0 && basicAccount->birth_year % 100 != 0) ||
               (basicAccount->birth_year % 400 == 0)))) {
            printf("Invalid input. Please enter a valid day.\n");
            continue; // Repeat the loop to ask again
        }

        // If we reach here, input is valid
        isValidInput = 1;

    } while (!isValidInput);

    // Compare birthdate with current date
    int age = current_time->tm_year + 1900 - basicAccount->birth_year;

    // Adjust age based on birth month and day
    if (current_time->tm_mon + 1 < basicAccount->birth_month ||
        (current_time->tm_mon + 1 == basicAccount->birth_month &&
         current_time->tm_mday < basicAccount->birth_day)) {
        age--;
    }

    // Display age (you might want to use this information)
    return age;
}

// Memory allocator
void allocateMemory(signUpDetails *account) {
    account->firstName = (char *)malloc(DEFAULT_DATA_SIZE * sizeof(char));
    account->lastName = (char *)malloc(DEFAULT_DATA_SIZE * sizeof(char));
    account->middleName = (char *)malloc(DEFAULT_DATA_SIZE * sizeof(char));
    account->nationality = (char *)malloc(DEFAULT_DATA_SIZE * sizeof(char));
    account->accountNumber = (char *)malloc(13 * sizeof(char)); // 12 digits + '\0'
}

void freeMemory(signUpDetails *account) {
    free(account->firstName);
    free(account->lastName);
    free(account->middleName);
    free(account->nationality);
    free(account->accountNumber);
}

char *generateAccountNumber() {
    char *accountNumber = (char *)malloc(13 * sizeof(char)); // 12 digits + '\0'
    srand(time(NULL));

    // Generate a random 12-digit number
    for (int i = 0; i < 12; i++) {
        accountNumber[i] = '0' + rand() % 10;
    }

    accountNumber[12] = '\0'; // Null-terminate the string

    return accountNumber;
}

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
        } else {
            break;
        }
    } while (1);
}

void printS(const char* printline, char* container, size_t size)
{
    size_t len;
    do {
        printf(printline);
        fgets(container, size, stdin);

        len = strcspn(container, "\n");
        if(len > 0 && container[len - 1] == '\n') {
            container[len - 1] = '\0';
        }

        if(len == 0) {
            printf("Empty input, please try again\n");
        } else if(!isString(container)) {
            printf("Invalid input, please try again\n");
        }

    } while(!isString(container) || len == 0);
}

void f_SignUp() {
    FILE *file;
    file = fopen("MyAccounts.txt", "a");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    signUpDetails basicAccountSignUp;
    allocateMemory(&basicAccountSignUp);

    // Name Input
    printf("Enter First Name: ");
    readString(basicAccountSignUp.firstName, DEFAULT_DATA_SIZE);

    printf("Enter Middle Name: ");
    readString(basicAccountSignUp.middleName, DEFAULT_DATA_SIZE);

    printf("Enter Last Name: ");
    readString(basicAccountSignUp.lastName, DEFAULT_DATA_SIZE);

    // Birthday Input
    int age;
    do {
        age = ageValidator(&basicAccountSignUp);
        if (age == -1) {
            printf("Invalid input. Please try signing up again.\n");
            freeMemory(&basicAccountSignUp);
            fclose(file);
            return;
        }
    } while (age == -1);

    // Nationality Input
    printf("Enter your Nationality: ");
    readString(basicAccountSignUp.nationality, DEFAULT_DATA_SIZE);

    // Phone number input
    printInt("Enter your Phone Number: ", &basicAccountSignUp.mobileNumber, sizeof(basicAccountSignUp.mobileNumber));

    // Generate and assign the account number
    basicAccountSignUp.accountNumber = generateAccountNumber();
    strcpy(basicAccountSignUp.accountNumber, generateAccountNumber());

    // Account Pin
    printInt("Create 6 Digit Pin Number: ", (unsigned long long int*)basicAccountSignUp.pinNumber, sizeof(basicAccountSignUp.pinNumber));

    // Congrats User for making an account
    fprintf(file, "Account Number: %s\n", basicAccountSignUp.accountNumber);
    fprintf(file, "Pin Number: %s\n", basicAccountSignUp.pinNumber);
    fprintf(file, "Full Name: %s %s %s\n", basicAccountSignUp.firstName, basicAccountSignUp.middleName,
            basicAccountSignUp.lastName);
    fprintf(file, "Birthdate: %d-%d-%d\n", basicAccountSignUp.birth_year, basicAccountSignUp.birth_month,
            basicAccountSignUp.birth_day);
    fprintf(file, "Age: %d\n", age);
    fprintf(file, "Nationality: %s\n", basicAccountSignUp.nationality);
    fprintf(file, "Phone Address: +63%d\n", basicAccountSignUp.mobileNumber);
    fclose(file);

    printf("User signed up successfully! \n Your Account Number is: %s\n", basicAccountSignUp.accountNumber);

    freeMemory(&basicAccountSignUp);
}

void f_Login() {
    FILE *file;
    int found = 0;

    file = fopen("MyAccounts.txt", "r");

    signUpDetails basicAccountLogin;
    allocateMemory(&basicAccountLogin);

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Account Number Input
    printInt("Enter Account Number: ", (unsigned long long int*)&basicAccountLogin.mobileNumber, sizeof(basicAccountLogin.mobileNumber));

    // Pin Number Input
    printInt("Enter pin number: ", (unsigned long long int*)basicAccountLogin.pinNumber, sizeof(basicAccountLogin.pinNumber));

    char fileAccountNumber[13];
    char filePinNumber[7];

    while (fscanf(file, "Account Number: %s\nPin Number: %s", fileAccountNumber, filePinNumber) == 2) {
        if (strcmp(basicAccountLogin.accountNumber, fileAccountNumber) == 0 &&
            strcmp(basicAccountLogin.pinNumber, filePinNumber) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Login successful!\n");
    } else {
        printf("Invalid account number or pin. Please try again.\n");
        clearInputBuffer(); // Clear input buffer before returning to the main screen
    }

    freeMemory(&basicAccountLogin);
}



#endif // FUNCTIONS_H
