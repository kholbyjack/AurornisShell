#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


// function for displaying the message of the day
void motd() {
    printf("\033[0;33m");
    printf("Welcome to Aurornis Shell!\n");
    printf("--------------------------\n")
    printf("\033[0m");
}

int custom_cd(int nargs, char *argscd[]) {
    char *dir = NULL;
    char current_dir[1025];
    
    if(nargs > 2) {
        printf("\033[0;31m");
        fprintf(stderr, "<cd> cd does not allow for more than one directory.\n");
        printf("\033[0m");
        return 1;
    }
    if(nargs == 2) {
        // Check if argument is "-", then its own implementation is needed
        int is_dash = strcmp(argscd[1], "-"); 
        if(is_dash == 0) {
            dir = getenv("OLDPWD");
        }
        else {
            dir = argscd[1];
        }
    }
    else {
        // No argument given, change to the home directory
        dir = getenv("HOME");
    }

    if(dir == NULL) {
        printf("\033[0;31m");
        fprintf(stderr, "<cd> Could not get the new directory.\n");
        printf("\033[0m");
        return(1);
    }

    // Updates environment variables and changes directory
    // Update OLDPWD
    if(getcwd(current_dir, sizeof(current_dir)) != NULL) {
        setenv("OLDPWD", current_dir, 1);
    }
    else {
        printf("\033[0;31m");
        fprintf(stderr, "<cd> Could not get the current directory for OLDPWD.\n");
        printf("\033[0m");
        return 1;
    }

    // Change directory
    int is_success = chdir(dir);
    if(is_success != 0) {
        printf("\033[0;31m");
        fprintf(stderr, "<cd> Could not change directory.\n");
        printf("\033[0m");
        return 1;
    }

    // Update PWD
    if(getcwd(current_dir, sizeof(current_dir)) != NULL) {
        setenv("PWD", current_dir, 1);
    }
    else {
        printf("\033[0;31m");
        fprintf(stderr, "<cd> Could not get the current directory for PWD.\n");
        printf("\033[0m");
        return 1;
    }

    return 0;
}

// Try getting rid of these parameters lol
int main()
{
    // Signal to exit
    int exit_program = 0;
    int background = 0; // Background flag, if 1, run in the background

    // Message of the day, only prints once at the beginning of the program
    motd();

    // Main loop
    do{
        // Buffer
        char buff[1025];
        memset(buff, 0x00, sizeof(buff));
        
        // Prompt
        printf("\033[0;33m");
        printf(">>> ");
        printf("\033[0m");

        // Get input
        printf("\033[0;32m");
        fgets(buff, 1024, stdin);
        printf("\033[0m");

        // Get rid of the newline character from the user pressing enter
        char *nl = strchr(buff, '\n');
        if(nl) {
            *nl = '\0';
        }


        // Separate the arguments
        char *args[24];
        int narg = 0;//counter for the loop

        // Using strtok() to separate the arguments, will end with a NULL
        args[narg] = strtok(buff, " ");
        while(args[narg] != NULL) {
            narg++;
            args[narg] = strtok(NULL, " ");
        }

        // Continue if there is no command
        if(args[0] == NULL) {
            continue;
        }

        // Handle &
        // TESTING
        int has_amp = strcmp(args[narg - 1], "&"); 
        if(has_amp == 0) {
            background = 1; //NEW
            args[narg - 1] = NULL;
        }

        // Exit check
        int is_exit = strcmp(args[0], "exit"); 
        if(is_exit == 0) {
            printf("\033[0m");
            printf("Goodbye!");
            exit_program = 1;
            kill(0, SIGINT);
            exit(0);
        } 

        // Fork 
        pid_t pid = fork();

        if(pid < 0) {
            printf("\033[0;31m");
            fprintf(stderr, "Fork failed.");
            printf("\033[0m");
            exit_program = 1;
            exit(-1);
        }
        else if(pid == 0) { // Is a child
            // implementation of cd
            int is_cd = strcmp(args[0], "cd"); 
            if(is_cd == 0) {
                // call cd function
                custom_cd(narg, args);
            }
            else {
                //use exec to perform any tasks that are not cd or exit
                printf("\033[0;33m");
                execvp(args[0], args); 
                printf("\033[0m"); 
            }   
        }
        else { // Is a parent
            // NEED a way to tell that the program is running in the background
            // only do this if the program is not running in the background
            pid_t program_pid = tcgetpgrp(STDOUT_FILENO);
            pid_t parent_pid = getpgrp();

            // || NEW
            if(program_pid == parent_pid || background == 0) {
                // Process is running in the foreground
                printf("\033[0;33m");
                printf("-------Starting Program-------\n");
                wait(NULL);
                printf("-------Program Ended-------\n");
                printf("\033[0m");
                // exit_program = 1;
                // exit(0);
            }
            else {
                waitpid(-1, &status, WNOHANG); //NEW
                // returns immediately if no child has executed
            }

        }

    } while(exit_program == 0);

    // Exit the program
    exit(0);
    
}