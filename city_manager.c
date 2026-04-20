#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include<fcntl.h>
#include <unistd.h>
#include<string.h>

#define MAX_LENGTH 100

typedef struct {
    float latitude;
    float longitude;
}coordinates;

typedef struct {
    int ID;
    char inspector_name[MAX_LENGTH];
    coordinates GPS;
    char issue_category[MAX_LENGTH];
    int severity_level;
    time_t timestamp;
    char description_text[MAX_LENGTH];
}report;

int main(int argc, char **argv) {

    int input_file_handler = open("reports.dat", O_WRONLY, S_IWUSR);

    if (input_file_handler == -1) {

        perror("Error opening input file");
        close(input_file_handler);
        exit(1);
    }

    // to do: orice ordine !!
    /*
     * for (int i = 1; i <= argc; i++){
     *
     */
    // roles
    // manager
    if (strcmp(argv[3], "manager") == 0) {

    }
    // inspector
    else if (strcmp(argv[3], "inspector") == 0) {

    }
    else {
        perror("incorrect role (3rd command argument)");
    }

    close(input_file_handler);
    return 0;
}