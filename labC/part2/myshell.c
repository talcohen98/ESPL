#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linux/limits.h"
#include "LineParser.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int debug = 0;
//receives a parsed line and invokes the program specified in the cmdLine using the proper system call.
void execute(cmdLine *pCmdLine){
    if(pCmdLine -> inputRedirect){
        fclose(stdin);
        if(fopen(pCmdLine ->inputRedirect,"r") == NULL){
            perror("Execution FAILED");
            _exit(1);
        }
    }

    if(pCmdLine -> outputRedirect){
        fclose(stdout);
        if(fopen(pCmdLine ->outputRedirect,"w") == NULL){
            perror("Execution FAILED");
            _exit(1);
        }
    }
    char *command = (*pCmdLine).arguments[0];
    if(execvp(command,(*pCmdLine).arguments) == -1){ // executing command + display error if execv failed.
        perror("Execution FAILED");
        _exit(1);
    }
}

void cd(cmdLine *pCmdLine){
    chdir(pCmdLine -> arguments[1]);
}

void makePipe(cmdLine *lineToParse){
    int pipefd[2]; // 0 read 1 write
    pid_t pid1, pid2;
    if(lineToParse->outputRedirect || lineToParse->next->inputRedirect){ // redirection
        fprintf(stderr, "Invalid redirection\n");
        return;
    }
    if (pipe(pipefd) == -1) {
        perror("Failed to pipe");
        _exit(-1);
    }

    pid1 = fork();
    if (pid1 == 0) { // child 1 process
        close(STDOUT_FILENO); // close standard output
        dup(pipefd[1]); // duplicate write-end of pipe
        close(pipefd[1]); // close write-end of pipe

        if(lineToParse -> inputRedirect){ // redirection
        fclose(stdin);
            if(fopen(lineToParse ->inputRedirect,"r") == NULL){
                perror("Execution FAILED");
                _exit(1);
            }
        }
        if(execvp(lineToParse->arguments[0],lineToParse->arguments) == -1){ // executing command + display error if execv failed.
            perror("Execution FAILED");
            _exit(1);
        }
    }else { // parent process
        close(pipefd[1]);
        if(lineToParse->next !=NULL){
            pid2 = fork();
            if (pid2 == 0) { // child 2 process
                close(STDIN_FILENO);
                dup(pipefd[0]);
                close(pipefd[0]);

                if(lineToParse->next -> outputRedirect){ // redirection
                    fclose(stdout);
                    if(fopen(lineToParse->next ->outputRedirect,"w") == NULL){
                        perror("Execution FAILED");
                        _exit(1);
                    }
                }
                if(execvp(lineToParse->next->arguments[0],lineToParse->next->arguments) == -1){ // executing command + display error if execv failed.
                    perror("Execution FAILED");
                    _exit(1);
                }

            } else {
                close(pipefd[0]);
                waitpid(pid1, NULL, 0);
                waitpid(pid2, NULL, 0);
            }
        }

    }
}

int main(int argc, char *argv[]){
    char cwd[PATH_MAX];
    char input[2048];
    struct cmdLine *lineToParse;

    for(int i=0; i<argc; i++){
		if(strcmp(argv[i],"-d") == 0)
			debug = 1;
	}

    while(1){
        if(getcwd(cwd, PATH_MAX) == 0) //print cwd
            return 1;
        printf("%s$ ", cwd);

        fgets(input,2048,stdin);
        if(strncmp("quit",input,4) == 0) //end inf loop with "quit"
            exit(0);
        lineToParse = parseCmdLines(input); //parse input
        if(lineToParse->next !=NULL){
            makePipe(lineToParse);
        }
        else if(strcmp(lineToParse -> arguments[0],"cd") ==0){
            cd(lineToParse);        }
        else if(strcmp(lineToParse -> arguments[0],"wake") ==0){
            kill(atoi(lineToParse -> arguments[1]),SIGCONT);
        }
        else if(strcmp(lineToParse -> arguments[0],"kill") ==0) {
            kill(atoi(lineToParse -> arguments[1]),SIGINT);
        }
        else {
            //we create child process before execute
            
            int pid = fork();
            if(pid != 0){// this is a parent
                if((*lineToParse).blocking != 0){ // if no &, we wait for child
                    waitpid(pid, NULL, 0);
                }
            }else{ // this is a child
                execute(lineToParse); // DELETED CALL TO EXECUTE
            }
            if(debug == 1){
                fprintf(stderr,"PID: %d\n",pid);
                fprintf(stderr,"Executing command: %s\n",(*lineToParse).arguments[0]);
            }
            freeCmdLines(lineToParse);
        }
    }
    return 0;
}