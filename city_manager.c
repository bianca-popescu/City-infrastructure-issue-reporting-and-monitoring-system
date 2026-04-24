#include "city_manager.h"

// verifica daca rolul are permisiunile necesare
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

// conversie
void get_permissions_string(mode_t mode, char *str) {

    //initializare
    strcpy(str, "---------");

    // manager (owner)
    if (mode & S_IRUSR) str[0] = 'r';
    if (mode & S_IWUSR) str[1] = 'w';
    if (mode & S_IXUSR) str[2] = 'x';

    // inspector (group)
    if (mode & S_IRGRP) str[3] = 'r';
    if (mode & S_IWGRP) str[4] = 'w';
    if (mode & S_IXGRP) str[5] = 'x';

    // others
    if (mode & S_IROTH) str[6] = 'r';
    if (mode & S_IWOTH) str[7] = 'w';
    if (mode & S_IXOTH) str[8] = 'x';
}

// 4 = read (r)
// 2 = write (w)
// 1 = execute (x)
//0 = no permission (-)

// report nou
void new_report(report *r) {

    printf("Report ID: ");
    scanf("%d", &(r->ID));

    printf("GPS coordinates: ");
    scanf("%f %f", &(r->GPS.latitude), &(r->GPS.longitude));

    printf("Category: ");
    scanf("%s", r->issue_category);

    printf("Severity (1-3): ");
    scanf("%d", &(r->severity_level));

    getchar();

    printf("Description: ");
    fgets(r->description_text, MAX_LENGTH, stdin);
    r->description_text[strcspn(r->description_text, "\n")] = 0; // sterge newline
}

// operatii
void add(const char *district, const char *user_name, const char *role) {

    char path[MAX_LENGTH];
    snprintf(path, MAX_LENGTH, "%s/reports.dat", district);

    if (!check_access(path, role, W_OK)) {
        printf("access denied: role %s cannot add reports", role);
        return;
    }

    mkdir(district, 0750);
    chmod(district, 0750);

    report r;
    memset(&r, 0, sizeof(report));

    new_report(&r);

    strncpy(r.inspector_name, user_name, MAX_LENGTH);
    r.timestamp = time(NULL);

    int file_descriptor = open(path, O_WRONLY | O_CREAT | O_APPEND, 0664);

    if (file_descriptor < 0) {

        perror("open failed");
        return;
    }

    write(file_descriptor, &r, sizeof(report));
    chmod(path, 0664);

    close(file_descriptor);

    // symlink: active_reports-<district> -> <district>/reports.dat
    char link_name[MAX_LENGTH];
    snprintf(link_name, MAX_LENGTH, "active_reports-%s", district);
    unlink(link_name); // delete if exists
    symlink(path, link_name);

}

void list(const char *district, const char *role) {

    char path[MAX_LENGTH];
    snprintf(path, MAX_LENGTH, "%s/reports.dat", district);

    if (!check_access(path, role, R_OK)) {
        printf("access denied: role '%s' cannot list reports.\n", role);
        return;
    }

    struct stat st;
    if (stat(path, &st) != 0) {

        printf("district %s has no reports.\n");
        return;
    }

    char permissions[10];
    get_permissions_string(st.st_mode, permissions);

    printf(" file permissions: %s\nsize: %ld bytes \nlast modification time: %s\n",
        permissions, st.st_size, ctime(&st.st_mtime));

    int file_descriptor = open(path, O_RDONLY);
    report r;

    while (read(file_descriptor, &r, sizeof(report)) == sizeof(report)) {

        printf("ID: %d\nissue category: %s\nseverity level: %d\ninspector name: %s\n",
            r.ID, r.issue_category, r.severity_level, r.inspector_name);
    }

    close(file_descriptor);
}