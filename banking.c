#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// store sign up details to database
void signUp(char *firstName, char *lastName, char *middleName, int birthDate, int birthMonth, int birthYear, char *country, char* phoneNumber,  char *accountNumberStr, char* pinNumber) {
    FILE *file = fopen("Account.txt", "a");
    if (file != NULL) {


        fprintf(file, "Name: %s  %s  %s\n", firstName, middleName, lastName);
        fprintf(file, "Birthday: %i - %i - %i\n", birthYear, birthDate, birthYear);
        fprintf(file, "Country: %s\n", country);
        fprintf(file, "Phone Number: %s\n", phoneNumber);
        fprintf(file, "Account Number: %s\n", accountNumberStr);
        fprintf(file, "Pin: %s\n", pinNumber);
        fclose(file);
    }
}


// account number generator
/*unsigned long long int generateRandom11DigitNumber() {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random 11-digit number
    return rand() % 90000000000ULL + 10000000000ULL;
}*/
