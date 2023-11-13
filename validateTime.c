#include <stdio.h>
#include <time.h>

int main() {
    // Get current time
    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);

    // Input the birthdate
    int birth_year, birth_month, birth_day;
    printf("Enter birth year: ");
    scanf("%d", &birth_year);
    printf("Enter birth month: ");
    scanf("%d", &birth_month);
    printf("Enter birth day: ");
    scanf("%d", &birth_day);

    // Compare birthdate with current date
    int age = current_time->tm_year + 1900 - birth_year;

    // Adjust age based on birth month and day
    if (current_time->tm_mon + 1 < birth_month || 
        (current_time->tm_mon + 1 == birth_month && current_time->tm_mday < birth_day)) {
        age--;
    }

}
