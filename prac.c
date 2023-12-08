#include <stdio.h>

void save_signup_data(const char *username, const char *password) {
    FILE *file = fopen("user_data.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s,%s\n", username, password);
        fclose(file);
    }
}
