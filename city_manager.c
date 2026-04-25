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

    printf("new report: \n");

    printf("report ID: ");
    scanf("%d", &(r->ID));

    printf("GPS coordinates: ");
    scanf("%f %f", &(r->GPS.latitude), &(r->GPS.longitude));

    printf("issue category: ");
    scanf("%s", r->issue_category);

    printf("severity level (1-3): ");
    scanf("%d", &(r->severity_level));

    getchar();

    printf("description: ");
    fgets(r->description_text, MAX_LENGTH, stdin);
    r->description_text[strcspn(r->description_text, "\n")] = 0; // sterge newline
}

// operatii
void add(const char *district, const char *user_name, const char *role) {

    char path[MAX_LENGTH];
    snprintf(path, MAX_LENGTH, "%s/reports.dat", district);

    if (!check_access(path, role, W_OK)) {
        printf("access denied: role %s can't add reports", role);
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

        perror("open failed\n");
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

    printf("report %d added to %s\n", r.ID, district);

}

void list(const char *district, const char *role) {

    char path[MAX_LENGTH];
    snprintf(path, MAX_LENGTH, "%s/reports.dat", district);

    if (!check_access(path, role, R_OK)) {

        printf("access denied: role %s can't list reports\n", role);
        return;
    }

    struct stat st_dir;
    struct stat st_file;

    // verifica daca exista districtul
    if (stat(district, &st_dir) != 0) {

        printf("district %s doesn't exist\n", district);
        return;
    }

    // verifica daca district este director
    if (!S_ISDIR(st_dir.st_mode)) {

        printf("district %s is not a valid directory\n", district);
        return;
    }

    // verifica daca exista report-uri in district
    if (stat(path, &st_file) != 0) {

        printf("district %s has no reports\n");
        return;
    }

    char permissions[10];
    get_permissions_string(st_file.st_mode, permissions);

    printf(" file permissions: %s\nsize: %ld bytes \nlast modification time: %s\n",
        permissions, st_file.st_size, ctime(&st_file.st_mtime));

    int file_descriptor = open(path, O_RDONLY);
    report r;

    while (read(file_descriptor, &r, sizeof(report)) == sizeof(report)) {

        printf("ID: %d\nissue category: %s\nseverity level: %d\ninspector name: %s\n",
            r.ID, r.issue_category, r.severity_level, r.inspector_name);
    }

    close(file_descriptor);
}

void print_report_details(report r) {

    printf("report ID: %d", r.ID);
    printf("inspector name: %s\n", r.inspector_name);
    printf("coordinates: %.2f, %.2f\n", r.GPS.latitude, r.GPS.longitude);
    printf("issue category: %s\n", r.issue_category);
    printf("severity level: %d\n", r.severity_level);
    printf("timestamp: %s\n", ctime(&r.timestamp));
    printf("description: %s\n", r.description_text);

}

void view(const char *district, int id, const char *role) {

    char path[MAX_LENGTH];
    snprintf(path, MAX_LENGTH, "%s/reports.dat", district);

    if (!check_access(path, role, R_OK)) {

        printf("acces denied: role %s can't view reports/\n", role);
        return;
    }

    int file_descriptor = open(path, O_RDONLY);

    if (file_descriptor < 0) {

        perror("open failed\n");
        return;
    }

    report r;
    int found = 0;

    while (read(file_descriptor, &r, sizeof(report)) == sizeof(report) && found == 0) {

        if (r.ID == id) { // report gasit
            print_report_details(r);
            found = 1;
        }
    }

    if (found == 0) {
        printf("report %d not found in %s\n", id, district);
        close(file_descriptor);
    }
}
void remove_report(const char *district, int id, const char *role) {

    // verifica rolul
    if (strcmp(role, "manager") != 0) {

        printf("role %s can't remove reports\n", role);
        return;
    }

    char path[MAX_LENGTH];
    snprintf(path, MAX_LENGTH, "%s/reports.dat", district);

    int file_descriptor = open(path, O_RDWR);
    if (file_descriptor < 0) {

        perror("open failed\n");
        return;
    }

    report r;
    int found_index = -1;
    int current_index = 0;

    while (read(file_descriptor, &r, sizeof(report)) == sizeof(report)) {

        if (r.ID == id) { // report gasit
            found_index = current_index;
            break;
        }

        current_index++;
    }

    if (found_index == -1) {

        printf("report %d not found in %s\n", id, district);
        close(file_descriptor);
        return;
    }

    struct stat st;
    fstat(file_descriptor, &st);

    int total_records = st.st_size / sizeof(report);

    for (int i = found_index; i < total_records - 1; i++) {

        report next_report;

        // muta cursor la reportul urmator
        lseek(file_descriptor, (i + 1) * sizeof(report), SEEK_SET);
        read(file_descriptor, &next_report, sizeof(report));

        lseek(file_descriptor, i * sizeof(report), SEEK_SET);
        read(file_descriptor, &next_report, sizeof(report));
    }

    off_t new_size = (total_records - 1) * sizeof(report);

    if (ftruncate(file_descriptor, new_size) == -1) {
        perror("error truncating file\n");

    } else {

        printf("report %d removed successfully\n", id);
    }

    close(file_descriptor);
}

void update_threshold(const char *district, int new_threshold, const char *role) {

    // manager only
    if (strcmp(role, "manager") != 0) {

        printf("acces denied: role %s can't update thresholds\n", role);
        return;
    }

    char path[MAX_LENGTH];
    snprintf(path, MAX_LENGTH, "%s/district.cfg", district);

    // verificare permisiuni
    struct stat st;
    if (stat(path, &st) == 0) {

        if ((st.st_mode & 0777) != 0640) { // biti permisiuni

            printf("incorrect permission");
            return;
        }
    }

    int file_descriptor = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (file_descriptor < 0) {

        perror("open failed\n");
        return;
    }

    // write threshold
    char buffer[MAX_LENGTH];
    int l = snprintf(buffer, MAX_LENGTH, "threshold=%d\n", new_threshold);
    write(file_descriptor, buffer, l);

    chmod(path, 0640);
    close(file_descriptor);

    printf("new threshold for %s : %d\n", district, new_threshold);
}
