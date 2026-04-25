
#ifndef CITY_MANAGER_H
#define CITY_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/stat.h>

#define MAX_LENGTH 256

// structuri de date
typedef struct {
    float latitude;
    float longitude;
} coordinates;

typedef struct {
    int ID;
    char inspector_name[MAX_LENGTH];
    coordinates GPS;
    char issue_category[MAX_LENGTH];
    int severity_level;
    time_t timestamp;
    char description_text[MAX_LENGTH];
} report;

typedef enum {
    OP_NONE, OP_ADD, OP_LIST, OP_VIEW, OP_REMOVE, OP_THRESHOLD, OP_FILTER
} operation;

// functii
int check_access(const char *path, const char *role, int requested_bit);
void get_permissions_string(mode_t mode, char *str);
void new_report(report *r);
void add(const char *district, const char *user_name, const char *role);
void list(const char *district, const char *role);
void print_report_details(report r);
void view(const char *district, int id, const char *role);
void remove_report(const char *district, int id, const char *role);
void update_threshold(const char *district, int new_threshold, const char *role);

#endif