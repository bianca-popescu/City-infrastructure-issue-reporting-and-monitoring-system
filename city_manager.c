#include<stdio.h>
#include <stdlib.h>
#include<time.h>
#include<fcntl.h>
#include <unistd.h>
#include<string.h>
#include<getopt.h>
#include<sys/stat.h>

#define MAX_LENGTH 256

typedef struct {
    float latitude;
    float longitude;
}coordinates;

// structura report
typedef struct {
    int ID;
    char inspector_name[MAX_LENGTH];
    coordinates GPS;
    char issue_category[MAX_LENGTH];
    int severity_level;
    time_t timestamp;
    char description_text[MAX_LENGTH];
}report;

// optiuni operatii
typedef enum {
    OP_NONE,
    OP_ADD,
    OP_LIST,
    OP_VIEW,
    OP_REMOVE,
    OP_THRESHOLD,
    OP_FILTER

}operation;

int check_access(const char *path, const char *role, int requested_bit) {

    struct stat st;

    if (stat(path, &st) != 0) {
        return 1; // fisierul nu exista
    }

    if (strcmp(role, "manager") == 0) {

        if (requested_bit == R_OK && !(st.st_mode & S_IRUSR)) return 0;
        if (requested_bit == W_OK && !(st.st_mode & S_IWUSR)) return 0;
    }

    if (strcmp(role, "inspector") == 0) {

        if (requested_bit == R_OK && !(st.st_mode & S_IRGRP)) return 0;
        if (requested_bit == W_OK && !(st.st_mode & S_IWGRP)) return 0;
    }

    return 1;
}

int main(int argc, char **argv) {

    char *role = NULL;
    char *user = NULL;
    char *district = NULL;

    operation op = OP_NONE; // initializare operatie

    // optiuni argumente in linia de comanda
    static struct option long_options[] = {

        {"role", required_argument, 0, 'r'}, // required_argument - urmatorul parametru din comanda este valoarea rolului
        {"user", required_argument, 0, 'u'},
        {"add", required_argument, 0, 'a'},
        {"list", required_argument, 0, 'l'},
        {"view", required_argument, 0, 'v'},
        {"remove_report", required_argument, 0, 'd'},
        {"update_threshold", required_argument, 0, 't'},
        {"filter", required_argument, 0, 'f'},

        {0,0,0,0}
    };

    int option;
    int option_index = 0;

    // getopt_long accepta optiuni care incep cu --
    while ((option = getopt_long(argc, argv, "r:u:a:l:v:d:t:f:", long_options, &option_index)) != -1) {

        switch (option) {
            case 'r':
                role = optarg; // role pointeaza la adresa cuvantului "manager"/"inspector" in optarg
                break;

            case 'u':
                user = optarg;
                break;

            case 'a':
                op = OP_ADD;
                district = optarg;
                break;

            case 'l':
                op = OP_LIST;
                district = optarg;
                break;

            case 'v':
                op = OP_VIEW;
                district = optarg;
                break;

            case 'd':
                op = OP_REMOVE;
                district = optarg;
                break;

            case 't':
                op = OP_THRESHOLD;
                district = optarg;
                break;

            case 'f':
                op = OP_FILTER;
                district = optarg; break;

            default: exit(1);
        }
    }

    return 0;
}