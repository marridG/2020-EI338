#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define DEBUG
#define MAX_LINE 80                     // Maximum Command Length
#define DELIMITERS " \t\n\v\f\r"        // Delimiters of Parts in Commands

void refresh_args(char *args[]) {
    /*
     *   Refresh the Content of Arguments, i.e., Free Old Contents and Set to NULL
     *   @param: args           the array to refresh
     */
    while (*args) {
        free(*args);                    // to avoid memory leaks
        *args++ = NULL;
    }
}


int get_input(char *command) {
    /*
     *   Get Command from Input or History
     *   @param: command        variable to store the command, also the last command
     *   @returns:              success or not
     */

    char input_buffer[MAX_LINE + 1];

    // read one line of inputs (length <= MAX_LINE) and store in buffer
    if (fgets(input_buffer, MAX_LINE + 1, stdin) == NULL) {
        fprintf(stderr, "Failed to Read NULL Inputs\n");
        return 0;
    }

    // if (strncmp(input_buffer, "!!", 2) == 0) {
    //     if (strlen(command) == 0) {  // no history yet
    //         fprintf(stderr, "No history available yet!\n");
    //         return 0;
    //     }
    //     printf("%s", command);    // keep the command unchanged and print it
    //     return 1;
    // }

    // update the command
    strcpy(command, input_buffer);

    return 1;
}


size_t parse_input(char *args[], char *original_command) {
    /*
     *   Parse Input and Store Arguments
     *   @param: args           the array to store the parsed arguments
     *   @param: command        the input command
     *   @return:               the number of arguments
     */

    size_t num = 0;

    // make a copy of input command since separation strtok() will change the values
    char command[MAX_LINE + 1];
    strcpy(command, original_command);

    // continuously separate arguments by delimiters until End-of-Line
    char *token = strtok(command, DELIMITERS);
    while (token != NULL) {
        // store the split arguments to the "referred output" argument variable
        args[num] = malloc(strlen(token) + 1);
        strcpy(args[num], token);
        num++;
        token = strtok(NULL, DELIMITERS);
    }


    return num;
}


int main(void) {
    char *args[MAX_LINE / 2 + 1];       // command line arguments
    char command[MAX_LINE + 1];

    //Initialize the arguments, set as NULL
    for (size_t i = 0; i <= MAX_LINE / 2; i++) {
        args[i] = NULL;
    }
    // Initialize the command, set as an empty string
    strcpy(command, "");

    while (1) {
        printf("osh>");
        fflush(stdout);                 // flush the stream buffer

        fflush(stdin);                  // flush the stream buffer
        refresh_args(args);             // empty args before parsing

        // get and store the input (notice that "empty" command ENTER is a success)
        if (!get_input(command)) {
            continue;
        }
#ifdef DEBUG                            // print the stored input command
        printf("[DEBUG] The input command is: \"%s\"", command);
#endif

        // parse the input command
        size_t args_num = parse_input(args, command);
#ifdef DEBUG                            // print the stored parsed arguments of the input command
        printf("[DEBUG] The parsed %zu arguments are:\n", args_num);
        for (size_t i = 0; i <= args_num - 1; i++)
            printf("\t\"%s\"\n", args[i]);
#endif


        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) parent will invoke wait() unless command included &
        */
    }
    return 0;
}