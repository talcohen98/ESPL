#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//parsed args
char *lsArgs[] = {"ls", "-l", NULL};
char *tailArgs[] = {"tail", "-n", "2", NULL};

int main(int argc, char **argv) {
    int fd[2];
    pid_t pid1, pid2;

    if (pipe(fd) == -1) {
        perror("Failed to pipe");
        exit(1);
    }

    fprintf(stderr, "%s", "(parent_process>forking…)\n");
    pid1 = fork();// first fork

    if (pid1 == 0) { // child 1 process
        close(STDOUT_FILENO); // close stdout
        fprintf(stderr, "%s", "(child1>redirecting stdout to the write end of the pipe…)\n");
        dup(fd[1]); // duplicate write-end of pipe
        close(fd[1]); // close original fd
        fprintf(stderr, "%s", "(child1>going to execute cmd: …)\n");
        execvp(lsArgs[0], lsArgs);
        
    } else { // parent process
        fprintf(stderr, "%s %d\n", "(parent_process>created process with id: )", pid1);
        fprintf(stderr, "%s", "(parent_process>closing the write end of the pipe…)\n");
        close(fd[1]);
        fprintf(stderr, "%s", "(parent_process>forking…)\n");

        pid2 = fork(); // second fork

        if (pid2 == 0) { // child 2 process
            close(STDIN_FILENO);
            fprintf(stderr, "%s", "(child2>redirecting stdout to the write end of the pipe…)\n");
            dup(fd[0]);
            close(fd[0]);
            fprintf(stderr, "%s", "(child2>going to execute cmd: …)\n");
            execvp(tailArgs[0], tailArgs);
        } else {
            fprintf(stderr, "%s %d\n", "(parent_process>created process with id: )", pid2);
            fprintf(stderr, "%s", "(parent_process>closing the read end of the pipe…)\n");
            close(fd[0]);
            fprintf(stderr, "%s", "(parent_process>waiting for child processes to terminate…)\n");
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            fprintf(stderr, "%s", "(parent_process>exiting…)\n");
        }
    }
    
    
}