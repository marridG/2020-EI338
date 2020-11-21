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


/*!
 * Get Command from Input or History
 * @param command               variable to store the command, also the last command
 * @return                      1 = successful, 0 = unsuccessful
 */
int get_input(char *command) {
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
    //     printf("%s", command);           // keep the command unchanged and print it
    //     return 1;
    // }

    // update the command
    strcpy(command, input_buffer);

    return 1;
}


/*!
 * Parse Input and Store Arguments
 * @param args                  the array to store the parsed arguments
 * @param original_command      the input command
 * @return                      the number of arguments
 */
size_t parse_input(char *args[], char *original_command) {
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


/*!
 * Print the Help Messages
 */
void print_help_msg() {
    printf("A Simple UNIX Shell\n\n"
           "usage: <command> [<args>]\n\n"
           "These are common commands used in various situations:\n\n"
           "Control Commands\n"
           "  ?, help, -help, --help             Show the help messages\n"
           "  exit                               Exit the simple UNIX shell\n"
           "\n"
           "Some other UNIX commands are also supported, you may try yourself\n");
}


/*!
 *   Check whether an Ampersand (&) is in the End of Arguments. If so,
 *      (1) remove '&' from the last arg
 *      (2) remove the whole last arg if it becomes empty (actually should be)
 *   @param: args           the array to check
 *   @param: size           the pointer to array size
 *   @return                whether an ampersand is in the end: 1=yes, 0=no
 */
int check_ampersand(char **args, size_t *size) {

    size_t len = strlen(args[*size - 1]);

    // check whether & is in the last argument
    if (args[*size - 1][len - 1] != '&') {
        return 0;
    }

    // remove the whole last arg if arg=='&' exactly
    if (len == 1) {
        free(args[*size - 1]);
        args[*size - 1] = NULL;
        --(*size);                      // reduce its size
    }
        // remove only the '&' in the last command
    else {
        args[*size - 1][len - 1] = '\0';
    }


    return 1;
}


/*!
 * Run the Input Command, via Parsed Arguments
 * @param args                  arguments list
 * @param args_num              number of arguments
 * @return                      success or not
 */
int run_command(char **args, size_t args_num) {
    // [CONCURRENT] Detect '&' to determine whether to run concurrently
    int run_concurrently = check_ampersand(args, &args_num);

    // [PIPE]
    char **args2;
    size_t args_num2 = 0;
    // detect_pipe(args, &args_num, &args2, &args_num2);

    // [EXECUTE] Create a child process
    pid_t pid = fork();

    // fork failed
    if (pid < 0) {
        fprintf(stderr, "Failed to fork!\n");
        return 0;
    }
        // [PROCESS] child process
    else if (pid == 0) {
        if (args_num2 != 0) {    // pipe
            // /* Create pipe */
            // int fd[2];
            // pipe(fd);
            // /* Fork into another two processes */
            // pid_t pid2 = fork();
            // if (pid2 > 0) {  // child process for the second command
            //     /* Redirect I/O */
            //     char *input_file, *output_file;
            //     int input_desc, output_desc;
            //     unsigned io_flag = check_redirection(args2, &args_num2, &input_file,
            //                                          &output_file);    // bit 1 for output, bit 0 for input
            //     io_flag &= 2;   // disable input redirection
            //     if (redirect_io(io_flag, input_file, output_file, &input_desc, &output_desc) == 0) {
            //         return 0;
            //     }
            //     close(fd[1]);
            //     dup2(fd[0], STDIN_FILENO);
            //     wait(NULL);     // wait for the first command to finish
            //     execvp(args2[0], args2);
            //     close_file(io_flag, input_desc, output_desc);
            //     close(fd[0]);
            //     fflush(stdin);
            // }
            // else if (pid2 == 0) {  // grandchild process for the first command
            //     /* Redirect I/O */
            //     char *input_file, *output_file;
            //     int input_desc, output_desc;
            //     unsigned io_flag = check_redirection(args, &args_num, &input_file,
            //                                          &output_file);    // bit 1 for output, bit 0 for input
            //     io_flag &= 1;   // disable output redirection
            //     if (redirect_io(io_flag, input_file, output_file, &input_desc, &output_desc) == 0) {
            //         return 0;
            //     }
            //     close(fd[0]);
            //     dup2(fd[1], STDOUT_FILENO);
            //     execvp(args[0], args);
            //     close_file(io_flag, input_desc, output_desc);
            //     close(fd[1]);
            //     fflush(stdin);
            // }
        }
        else {    // no pipe
            // /* Redirect I/O */
            // char *input_file, *output_file;
            // int input_desc, output_desc;
            // unsigned io_flag = check_redirection(args, &args_num, &input_file,
            //                                      &output_file);    // bit 1 for output, bit 0 for input
            // if (redirect_io(io_flag, input_file, output_file, &input_desc, &output_desc) == 0) {
            //     return 0;
            // }
            size_t exe_result = execvp(args[0], args);
#ifdef DEBUG
            printf("%zu\n", exe_result);
            fflush(stdout);
#endif
            // close_file(io_flag, input_desc, output_desc);
            fflush(stdin);
        }
    }

        // [PROCESS] parent process
    else {
        // parent waits for child's exit, i.e, not concurrently
        if (!run_concurrently) {
            wait(NULL);
        }
    }

    return 1;
}


int main(void) {
    char *args[MAX_LINE / 2 + 1];       // command line arguments
    char command[MAX_LINE + 1];

    // [INIT] Initialize the arguments, set as NULL
    for (size_t i = 0; i <= MAX_LINE / 2; i++) {
        args[i] = NULL;
    }
    // [INIT] Initialize the command, set as an empty string
    strcpy(command, "");


    while (1) {
        printf("osh>");
        fflush(stdout);                 // flush the stream buffer

        fflush(stdin);                  // flush the stream buffer
        refresh_args(args);             // empty args before parsing

        // [INPUT] Get and store the input (notice that "empty" command ENTER is a success)
        if (!get_input(command)) {
            continue;
        }
#ifdef DEBUG                            // print the stored input command
        printf("[DEBUG] The input command is: \"%s\"\n", command);
#endif

        // [INPUT] Parse the input command
        size_t args_num = parse_input(args, command);
        if (0 == args_num) {
            continue;
        }
#ifdef DEBUG                            // print the stored parsed arguments of the nonempty input command
        printf("[DEBUG] The parsed %zu arguments are:\n", args_num);
        for (size_t i = 0; i <= args_num - 1; i++)
            printf("\t\"%s\"\n", args[i]);
#endif

        // [INPUT] [Extra] Enable "exit" command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        // [INPUT] [Extra] Enable "?", "help", "-help", "--help" command
        if (strcmp(args[0], "?") == 0 || strcmp(args[0], "help") == 0 ||
            strcmp(args[0], "-help") == 0 || strcmp(args[0], "--help") == 0) {
            print_help_msg();
            continue;
        }


        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) parent will invoke wait() unless command included &
        */
    }
    return 0;
}