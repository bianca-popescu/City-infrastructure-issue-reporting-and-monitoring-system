#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAX_LENGTH 512
void start_monitor() {

    pid_t hub_mon = fork(); // proces fiu

    if (hub_mon == 0) {

        int pfd[2];
        if (pipe(pfd) < 0) {

            perror("pipe error\n");
            exit(1);
        }

        pid_t monitor_pid = fork(); // proces nepot
        if (monitor_pid < 0) {

            perror("fork monitor failed\n");
            exit(1);
        }

        if (monitor_pid == 0) {

            dup2(pfd[1], STDOUT_FILENO); // redirectare la pipe

            close(pfd[0]);
            close(pfd[1]);

            execl("./monitor_reports", "monitor_reports", (char *)NULL); // inlocuieste procesul cu unul nou
            perror("execl monitor failed\n");
            exit(1);
        }

        close(pfd[1]); // inchidem scrierea

        char buffer[MAX_LENGTH];
        int bytes_read;

        while ((bytes_read = read(pfd[0], buffer, MAX_LENGTH - 1)) > 0) {

            buffer[bytes_read] = '\0';
            printf("%s", buffer);
            fflush(stdout);
        }

        close(pfd[0]);

        int status;
        waitpid(monitor_pid, &status, 0);
        exit(0);
    }

}

void calculate_scores(char **districts, int count) {

   for (int i = 0; i < count; i++) {

       int pfd[2];

       if (pipe(pfd) == 0) {

           pid_t pid = fork();

           if (pid == 0) { // proces copil

               dup2(pfd[1], STDOUT_FILENO);
               close(pfd[0]);
               close(pfd[1]);

               execl("/.scorer", "scorer", districts[i], (char*)NULL);
               exit(1);
           }

           if (pid > 0) { // proces parinte

               close(pfd[1]);

               char buffer[MAX_LENGTH];
               int bytes_read;

               printf("district: %s\n", districts[i]);

               // citire din pipe
               while ((bytes_read = read(pfd[0], buffer, MAX_LENGTH - 1)) > 0) {

                   buffer[bytes_read] = '\0';
                   printf("%s", buffer);
               }

               close(pfd[0]);
               waitpid(pid, NULL, 0);
           }
           else {
               perror("fork failed\n");
               close(pfd[0]);
               close(pfd[1]);
           }
       }
       else {
           perror("pipe failed\n");
       }
   }
}