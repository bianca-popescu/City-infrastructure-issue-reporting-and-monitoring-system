#include "city_manager.h"

int main(int argc, char **argv) {

    char *role = NULL;
    char *user = NULL;
    char *district = NULL;

    operation op = OP_NONE; // initializare operatie
    int extra_value = 0;

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

    if (!role || op == OP_NONE || !district) {
        printf("usage error");
        return 1;
    }

    // nu avem parametru --user
    if (user == NULL) {
        user = "unknown";
    }

    switch (op) {

        case OP_ADD:
            add(district, user, role);
            break;

        case OP_LIST:
            list(district, role);
            break;

        case OP_VIEW:
            if (optind < argc) {

                extra_value = atoi(argv[optind]); // ID de cautat
                view(district, extra_value, role);
            }
            else {
                printf("no ID provided\n");
            }
            break;

        case OP_REMOVE:
            if (optind < argc) {

                extra_value = atoi(argv[optind]);
                remove_report(district, extra_value, role);
            }
            else {
                printf("no ID provided\n");
            }
            break;

        case OP_THRESHOLD:

            if (optind < argc) {

                extra_value = atoi(argv[optind]);
                update_threshold(district, extra_value, role);
            }
            else {
                printf("no value provided\n");
            }

        case OP_FILTER:

            if (optind < argc) {

                filter(district, role, argc, argv, optind);
            } else {
                printf("no condition provided\n");
            }
            break;

        default:
            printf("invalid operation");
    }

    return 0;
}
