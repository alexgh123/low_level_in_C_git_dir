// ----------------------------------------------------------------------
// File: shell.c
// ----------------------------------------------------------------------
// Description:
//              A program that acts as a basic shell
//
// Usage:
//              ./shell
//
// Created: 2017-11-20 (A.Hardt)
// ----------------------------------------------------------------------


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


#define SUCCESS 0
#define ALARM_TIME 30
#define MAXARGS 10
#define CHAR_LENGTH 80


//GLOBAL
FILE * filePointer;

// **************************  signal handler  **************************

void signal_handler(int signal)
{
        if(signal == SIGSEGV){
                fprintf(stderr, "A segmentation fault has been detected.\n");
                fprintf(stderr, "Exiting...\n");
                //flush history file
                fflush(filePointer);
                //Close the file
                fclose(filePointer);
                exit(-1);
        }

        if (signal == SIGINT){
                fprintf(stdout, "\nThe interupt signal has been caught\n");
                fprintf(stdout, "Exiting...\n");
                //flush history file
                fflush(filePointer);
                //Close the file
                fclose(filePointer);
                exit(-2);
        }

        if (signal == SIGALRM){
                fprintf(stdout, "The session has expired.\n");
                fprintf(stdout, "Exiting...\n");
                //flush history file
                fflush(filePointer);
                //Close the file
                fclose(filePointer);
                exit(-3);
        }


} // End signal handler


// **********************************************************************
// **************************  M  A  I  N  ******************************
// **********************************************************************
int main()
{

        //reset status,pid
        int status;
        int pid;
        int i = 0;
        int length;
        //allocate space for user input
        char userInput[CHAR_LENGTH];
        // set array length, reinitilize to null so data isn't left over
        char *cmdArray[MAXARGS+1] = {NULL};
        //initialize token
        char *token = NULL;

        //set up signal handling:
        struct sigaction act;
        act.sa_handler=signal_handler;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_RESTART;
        // Set up alarm time
        alarm(ALARM_TIME);
        //indicate which signals to listen to:
        sigaction(SIGSEGV,&act,NULL);
        sigaction(SIGINT, &act,NULL);
        sigaction(SIGALRM, &act,NULL);


        errno = 0;
        //open file for write:
        filePointer = fopen("shell-history", "a");
        if(errno != 0){
                fprintf(stderr, "Error on opening of shell-history.\n");
                exit(-1);
        }

        // set up looping for prompt action
        while(1){

                // querry for input
                printf("prompt>");

                // get the input, put it into userInput
                errno = 0;
                fgets(userInput, sizeof(userInput), stdin);
                if(errno != 0){
                        fprintf(stderr, "Error on taking user input.\n");
                        exit(-1);
                }

                // write user input to shell history file
                errno = 0;
                fwrite(userInput, sizeof(char), strlen(userInput), filePointer);
                if(errno != 0){
                        fprintf(stderr, "Error on writing to history file.\n");
                        exit(-1);
                }

                //remove carriage return from user input
                length = strlen(userInput)-1;
                userInput[length] = '\0';

                // start tokenizing input
                token = strtok(userInput," ");

                //check for user input of "return"
                if(token == NULL){
                        continue;
                }

                // if user wishes to exit, exit(0)
                if(!strcmp(token, "exit")){
                        // fprintf(stdout, "exit triggered\n");
                        fclose(filePointer);
                        exit(0);
                }

                //if user enters 'explode', cause a segmentation fault
                if(!strcmp(token, "explode")){
                        int *bomb = NULL;
                        *bomb = 42;
                }

                // take the user input, put each space seperated cmd/option into an array
                while(token != NULL){
                        cmdArray[i++] = token;
                        token = strtok(NULL, " ");
                }

                // set end of array to null so it isn't read past
                cmdArray[i] = NULL;
                //reset index value
                i = 0;

                //fork a child process
                pid = fork();

                if(pid<0){
                        // bad parent
                        fprintf(stderr, "Error with parent process.\n");
                        fprintf(stderr, "Exiting...\n");
                        fclose(filePointer);
                        exit(-1);
                } else if (pid==0) {
                        // successfully forked child
                        status = execvp(cmdArray[0],cmdArray);
                        fprintf(stderr, "Error with command: (possibly invalid command or file does not exist)\n");
                        fclose(filePointer);
                        exit(-1);

                } else {
                        // wait for child
                        waitpid(pid, &status, 0);
                }

        } //End While

}// end main