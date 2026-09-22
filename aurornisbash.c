#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// function for displaying the message of the day
void motd() {
    printf("hi:3");
}

// maybe have function for cd and exit, dunno

int main(int argc, char *argv[])
{
    // Signal to exit
    int exit_program = 0;

    // Message of the day, only prints once at the beginning of the program
    motd();

    // main loop
    do{
        // buffer
        char buff[1024];
        
        // Prompt
        printf("$ ");

        // Get input
        fgets(buff, 1024, stdin);

        // Get rid of the newline character from the user pressing enter
        buff[strspn(buff, "\n")] = '\0';

        // Separate the arguments
        char *args[24];
        int narg = 0;//counter for the loop

        // Using strtok() to separate the arguments, will end with a NULL
        args[narg] = strtok(buff, " ");
        while(args[narg != NULL]) {
            narg++;
            args[narg] = strtok(NULL, " ");
        }

        // Checks
        // Continue if there is no command
        if(args[0] == NULL) {
            continue;
        }

        // Exit check
        int is_exit = strcmp(args[0], "exit"); 
        if(is_exit == 0) {
            exit_program = 1;
            //exit(0);
        } 

        // Fork 
        pid_t pid = fork();

        if(pid < 0) {
            fprintf(stderr, "Fork failed."); //NEED make color red
        }
        else if(pid == 0) { // Is a child
            // implementation of cd

            //use exec to perform any tasks that are not cd or exit
            execlp(args[0], args); //NEED make color blue (do this for cd and exit too)
            // WRONG, first arg needs to be the command name and then the second needs to be the whole command with the name again
            
        }
        else { // Is a parent
            // NEED a way to tell that the program is running in the background
            // only do this if the program is not running in the background
            printf("-------Starting Program-------");
            wait(NULL);
            printf("-------Program Ended-------");
            //exit_program = 1;
            // if running in the background, exit
        }

        //^^^^
        // checking if the program is running in the background
        // If it is not running in the background, enter this block 
        // Check the notes on the google doc for this
        // if() {
        //     // only do this if the program is not running in the background
        //     // maybe use pgrep???
        //     printf("-------Starting Program-------");
        //     wait(NULL);
        //     printf("-------Program Ended-------");
        //     exit_program = 1;
        // }


        // Clear the buffer ---NEED


    } while(exit_program == 0);

    // Exit the program
    exit(0);

    return 0;
    
}