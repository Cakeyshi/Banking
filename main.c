#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

// Sizes
#define MAX_USERS 100
#define DEFAULT_DATA_SIZE 100
#define FILENAME "Accounts.txt"

//size for fgets
char buffer[DEFAULT_DATA_SIZE];

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
void f_Main_Screen();
void allocateMemory(signUpDetails *account);
void freeMemory(signUpDetails *account);
void f_SignUp();
void f_Login();
char *generateAccountNumber();
int ageValidator(signUpDetails *basicAccount);

int main() {
    f_Main_Screen();
    return 0;
}

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
//Login and Sign up Options
//TODO: Put more features maybe??
//TODO: Fix ui issue pls check
void f_Main_Screen()
{
    int choice;

    do {
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        // Read user input using fgets and sscanf
        scanf(" %d", &choice);
        getchar();


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




int ageValidator(signUpDetails *basicAccount) {
    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);

    char buffer[DEFAULT_DATA_SIZE];

    //Using fgets and parsing it into integer using sscanf with getchar fixes newline problem
    // Input the birthdate
    printf("Enter birth year: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, " %d", &basicAccount->birth_year);


    // Input the birth month
    printf("Enter birth month: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, " %d", &basicAccount->birth_month);


    // Input the birth day
    printf("Enter birth day: ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, " %d", &basicAccount->birth_day);


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


//Memory allocator
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

// TODO: Have all necessary input details here
void f_SignUp() {
    FILE *file;
    file = fopen(FILENAME, "a");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    signUpDetails basicAccountSignUp;
    allocateMemory(&basicAccountSignUp);

    // Name Input
    printf("Enter your First Name: ");
    fgets(basicAccountSignUp.firstName, DEFAULT_DATA_SIZE, stdin);
    basicAccountSignUp.firstName[strcspn(basicAccountSignUp.firstName, "\n")] = '\0';

    printf("Enter your Last Name: ");
    fgets(basicAccountSignUp.lastName, DEFAULT_DATA_SIZE, stdin);
    basicAccountSignUp.lastName[strcspn(basicAccountSignUp.lastName, "\n")] = '\0';

    printf("Enter your Middle Name: ");
    fgets(basicAccountSignUp.middleName, DEFAULT_DATA_SIZE, stdin);
    basicAccountSignUp.middleName[strcspn(basicAccountSignUp.middleName, "\n")] = '\0';

    // Birthday Input
    int age = ageValidator(&basicAccountSignUp);

    // Nationality Input
    printf("Enter your Nationality: ");
    fgets(basicAccountSignUp.nationality, DEFAULT_DATA_SIZE, stdin);
    basicAccountSignUp.nationality[strcspn(basicAccountSignUp.nationality, "\n")] = '\0';

    // Phone number input
    printf("Enter your Phone Number: ");
    scanf("%d", &basicAccountSignUp.mobileNumber);


    // Generate and assign the account number
    basicAccountSignUp.accountNumber = generateAccountNumber();

    // Account Pin
    printf("Create 6 Digit Pin Number: ");
    scanf("%s", basicAccountSignUp.pinNumber);

    // Congrats User for making an account
    fprintf(file, "Account Number: %s\n", basicAccountSignUp.accountNumber);
    fprintf(file, "Pin Number: %s\n", basicAccountSignUp.pinNumber);
    fprintf(file, "Full Name: %s %s %s\n", basicAccountSignUp.firstName, basicAccountSignUp.middleName, basicAccountSignUp.lastName);
    fprintf(file, "Birthdate: %d-%d-%d\n", basicAccountSignUp.birth_year, basicAccountSignUp.birth_month, basicAccountSignUp.birth_day);
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

    file = fopen(FILENAME, "r");

    signUpDetails basicAccountLogin;
    allocateMemory(&basicAccountLogin);

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Enter Account Number: ");
    scanf("%s", basicAccountLogin.accountNumber);

    printf("Enter pin number: ");
    scanf("%s", basicAccountLogin.pinNumber);

    while (fscanf(file, " Account Number: %s Pin Number: %s", basicAccountLogin.accountNumber, basicAccountLogin.pinNumber) == 2) {
    if (strcmp(basicAccountLogin.accountNumber, basicAccountLogin.accountNumber) == 0 &&
        strcmp(basicAccountLogin.pinNumber, basicAccountLogin.pinNumber) == 0) {
        found = 1;
        break;
    }
}


    fclose(file);

    if (found) {
        printf("Login successful!\n");
    } else {
        printf("Invalid account number or pin. Please try again.\n");
    }

    freeMemory(&basicAccountLogin);
}
