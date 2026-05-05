#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<string.h>

// functie stergere fisier
void cleanup() {

    if (unlink(".monitor_pid") == 0) {
        printf("successfully removed /monitor_pid file\n");
    }
    else {
        perror("failed removing .monitor_pid file");
    }
}

// functie tratare semnal
void handle_signal(int sig) {

    if (sig == SIGUSR1) { // semnal user defined 1

        const char *message = "a new report has been added\n";
        write(STDOUT_FILENO, message, strlen(message));
    }

    else if (sig == SIGINT) { // semnal interrupt
        cleanup();
    }
}

int main() {

    // creare/ overwrite .monitor_pid
    int file_descriptor = open(".monitor_pid", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (file_descriptor < 0) {

        perror("creating .monitor_pid file failed");
        exit(1);
    }

    char pid_string[16];

    snprintf(pid_string, sizeof(pid_string), "%d", getpid());
    write(file_descriptor, pid_string, strlen(pid_string));

    close(file_descriptor);

    // sigaction pentru SIGUSR1 and SIGINT
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGUSR1, &sa, NULL); // sigaction SIGUSR1
    sigaction(SIGINT, &sa, NULL);   // sigaction SIGINT

    // infinite loop
    while (1) {
        pause();
    }

    return 0;
}

