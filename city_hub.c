#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_LENGTH 512
void start_monitor() {

    char buffer[MAX_LENGTH];
    int pfd[2];
    // pfd[0] - read end
    // pfd[1] - write end

    int pid;

    if (pipe(pfd) < 0) {
        perror("pipe error\n");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        perror("fork error\n");
        exit(1);
    }

    if (pid == 0) {
        // proces fiu
        close(pfd[0]); // inchide capatul de citire
        write(pfd[1], buffer, MAX_LENGTH); // scriere in pipe

        close(pfd[1]); // inchide capatul utilizat
        exit(0);
    }

    // proces parinte
    close(pfd[1]); // inchide capatul de scriere
    read(pfd[0], buffer, MAX_LENGTH);

    close(pfd[0]); // inchide capatul utilizat
}

// calculate_scores()