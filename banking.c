#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct UserProfile {
    char accountNumber[20];
    char pin[20];
    char firstName[50];
    char middleName[50];
    char lastName[50];
    char birthday[11];
    char age[10];
    char country[50];
    char phoneNumber[20];
    double balance;
};



void signUp(char *firstName, char *lastName, char *middleName, char* birthday, char* age, char *country, char *phoneNumber, char *accountNumberStr, char *pinNumber, double balance) {
    FILE *file = fopen("Account.txt", "a");
    if (file != NULL) {
        fprintf(file, "Account Number: %s\n", accountNumberStr);
        fprintf(file, "Pin: %s\n", pinNumber);
        fprintf(file, "Name: %s %s %s\n", firstName, middleName, lastName);
        fprintf(file, "Birthday: %s\n", birthday);
        fprintf(file, "Age: %s\n", age);
        fprintf(file, "Country: %s\n", country);
        fprintf(file, "Phone Number: %s\n", phoneNumber);
        fprintf(file, "Balance: %lf\n", balance);
        fprintf(file, "\n");
        fclose(file);
    } 
}


struct UserProfile* login_f(const char *accountNumber, const char *pin) {
    struct UserProfile *user = malloc(sizeof(struct UserProfile));
    if (user == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    FILE *file = fopen("Account.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        free(user);
        return NULL;
    }

    char buffer[100];

    // Search for the username and extract the corresponding password
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "Account Number:") != NULL) {
            sscanf(buffer, "%*[^:]: %[^\n]", user->accountNumber);
        } else if (strstr(buffer, "Pin:") != NULL) {
            sscanf(buffer, "%*[^:]: %[^\n]", user->pin);

            // Check if the input username and password match (case-insensitive)
            if (strcasecmp(accountNumber, user->accountNumber) == 0 && strcasecmp(pin, user->pin) == 0) {
                // Extract the entire group of information
                while (fgets(buffer, sizeof(buffer), file) != NULL && strcmp(buffer, "\n") != 0) {
                    if (strstr(buffer, "Name:") != NULL) {
                        sscanf(buffer, "%*[^:]: %s %s %s", user->firstName, user->middleName, user->lastName);
                    } else if (strstr(buffer, "Birthday:") != NULL) {
                        sscanf(buffer, "%*[^:]: %s", user->birthday);
                    } else if (strstr(buffer, "Age:") != NULL) {
                        sscanf(buffer, "%*[^:]: %s", user->age);
                    }else if (strstr(buffer, "Country:") != NULL) {
                        sscanf(buffer, "%*[^:]: %s", user->country);
                    } else if (strstr(buffer, "Phone Number:") != NULL) {
                        sscanf(buffer, "%*[^:]: %s", user->phoneNumber);
                    } else if (strstr(buffer, "Balance:") != NULL) {
                        sscanf(buffer, "%*[^:]: %lf", &user->balance);
                    }
                }
                fclose(file);
                return user;
            }
        }
    }

    fclose(file);
    // Free allocated memory and return NULL if credentials do not match
    free(user);
    return NULL;
}



//This will generate a random number and return a string version of it
char* generateRandomAccountNumber() {
    char accountNumber[20];  
    srand(time(NULL));
    
    unsigned long randomNumber = abs(rand() % 90000000000 + 10000000000);
    
    sprintf(accountNumber, "%ld", randomNumber);

    char* result = malloc(strlen(accountNumber) + 1);
    
    strcpy(result, accountNumber);

    return result;
}

/*void withdraw(char *accountNumber, double amount) {
    FILE *file = fopen("Account.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Create a temporary file for writing modified data
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char buffer[100];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, "Account Number:") != NULL) {
            // Check if the current line matches the target account number
            if (strstr(buffer, accountNumber) != NULL) {
                // Read the current balance
                fgets(buffer, sizeof(buffer), file);
                double currentBalance;
                sscanf(buffer, "Balance: %lf", &currentBalance);

                // Check if the withdrawal amount is valid
                if (amount > 0 && amount <= currentBalance) {
                    // Subtract the amount and write the updated balance to the temporary file
                    currentBalance -= amount;
                    fprintf(tempFile, "Balance: %.6lf\n", currentBalance);
                    fprintf(tempFile, "\n");  // add a newline after updating the balance
                } else {
                    // Invalid withdrawal amount, write the line as is to the temporary file
                    fprintf(tempFile, "%s", buffer);
                }
            } else {
                // Account number does not match, write the line as is to the temporary file
                fprintf(tempFile, "%s", buffer);
            }
        } else {
            // Not a line containing the account number, write the line as is to the temporary file
            fprintf(tempFile, "%s", buffer);
        }
    }

    // Close both files
    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file
    remove("Account.txt");
    rename("temp.txt", "Account.txt");
}

// Function to deposit an amount into an account
void deposit(const char *accountNumber, double amount) {
    FILE *file = fopen("Account.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[100];

    // Copy data to temp file while updating the balance
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Look for the account number
        if (strstr(buffer, "Account Number:") != NULL) {
            char currentAccount[20];
            sscanf(buffer, "Account Number: %s", currentAccount);

            // Check if this is the correct account
            if (strcmp(currentAccount, accountNumber) == 0) {
                // Read the current balance
                fgets(buffer, sizeof(buffer), file);
                double currentBalance;
                sscanf(buffer, "Balance: %lf", &currentBalance);

                // Add the deposit amount and write the updated balance to the temporary file
                currentBalance += amount;
                fprintf(tempFile, "Balance: %.6lf\n", currentBalance);
                fprintf(tempFile, "\n");  // add a newline after updating the balance
                continue;  // Skip writing the original balance to the temp file
            }
        }

        // Write the line to the temp file as is
        fprintf(tempFile, "%s", buffer);
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temp file
    remove("Account.txt");
    rename("temp.txt", "Account.txt");
}
*/
// Example usage:
// deposit("1410073658", 1000.0);  // Deposit 1000 to account number 1410073658


//This will free a string with allocated memory
void freeMemory(char* ptr){
    free(ptr);
}



//for debugging
/*int main()
{
    char accountNumber[20], pin[20];
    printf("Enter Account: ");
    scanf("%s", accountNumber);
    printf("Enter pin: ");
    scanf("%s", pin);

    if(!login(accountNumber, pin)){
        printf("Not Found!");
    }
    
    return 0;
}*/











/*char* printNumberGenerator(){
    char accountNumber[20];  
    char result[20];
    result = generateRandomAccountNumber(accountNumber);
    
    // Print the randomly generated account number
    return result;
}*/