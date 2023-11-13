#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define FILENAME "Users.txt"

struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
};

void signUp();
void login();

int main() {
    int choice;

    do {
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 3);

    return 0;
}

void signUp() {
    struct User newUser;
    FILE *file;

    file = fopen(FILENAME, "a");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Enter username: ");
    scanf("%s", newUser.username);

    printf("Enter password: ");
    scanf("%s", newUser.password);

    fprintf(file, "%s %s\n", newUser.username, newUser.password);

    fclose(file);

    printf("User signed up successfully!\n");
}

void login() {
    struct User loginUser;
    FILE *file;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int found = 0;

    file = fopen(FILENAME, "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    while (fscanf(file, " %49s %49s", loginUser.username, loginUser.password) == 2) {
        if (strcmp(username, loginUser.username) == 0 && strcmp(password, loginUser.password) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Login successful!\n");
    } else {
        printf("Invalid username or password. Please try again.\n");
    }
}
