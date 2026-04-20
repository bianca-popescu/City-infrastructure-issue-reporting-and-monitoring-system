#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include<fcntl.h>

#define MAX_LENGHT 100

typedef struct {
    float latitude;
    float longitude;
}coordinates;

typedef struct {
    int ID;
    char inspector_name[MAX_LENGHT];
    coordinates GPS;
    char issue_category[MAX_LENGHT];
    int severity_level;
    time_t timestamp;
    char description_text[MAX_LENGHT];
}report;

int main(void) {

    int input_file_handler = open("reports.dat", O_WRONLY, S_IWUSR);

    if (input_file_handler == -1) {

        perror("Error opening input file");
        close(input_file_handler);
        exit(1);
    }



    close(input_file_handler);
    return 0;
}