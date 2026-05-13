#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "city_manager.h"

#define MAX_SCORES 500 // numar maxim de scoruri

typedef struct {
    char name[MAX_LENGTH];
    int total_severity;
}Score;

// returneaza indexul inspectorului daca acesta exista deja in array (a mai fost intalnit in lista de reports)
// sau -1 altfel
int inspector_found(Score *scores, int count, const char *inspector_name) {

    for (int i = 0; i < count; i++) {

        if (strcmp(scores[i].name, inspector_name) == 0) {
            return i; // indexul inspectorului
        }
    }

    return -1; // nu s-a gasit numele inspectorului cautat
}

int main(int argc, char **argv) {

    if (argc < 2) {
        return 1;
    }

    char *district = argv[1];
    char path[MAX_LENGTH];

    snprintf(path, MAX_LENGTH, "%s/reports.dat", argv[1]);

    int file_descriptor = open(path, O_RDONLY);

    if (file_descriptor < 0) {

        perror("district is empty or missing\n");
        return 0;
    }

    Score scores[MAX_SCORES];
    int count = 0;
    report r;

    while (read(file_descriptor, &r, sizeof(report)) > 0) {

        int index = inspector_found(scores, count, r.inspector_name);

        if (index >= 0) { // inspectorul exista deja in lista

            scores[index].total_severity += r.severity_level;
            // se adauga severitatea report-ului curent la severitatea totala
        }
        else if (count < MAX_SCORES) {// numele inspectorului nu a fost gasit, se adauga in lista

            strcpy(scores[count].name, r.inspector_name);
            scores[count].total_severity = r.severity_level;
            count++;
        }
    }

    return 0;
}