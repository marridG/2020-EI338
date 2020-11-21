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

/*!
 *   Refresh the Content of Arguments, i.e., Free Old Contents and Set to NULL
 *   @param: args           the array to refresh
 */
void refresh_args(char *args[]) {
    while (*args) {
        free(*args);                    // to avoid memory leaks
        *args++ = NULL;
    }
}


/*!
 * Get Command from Input (with EOL \n, \r removed) or History
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

    // remove EOL \n, \r
    input_buffer[strlen(input_buffer)] = '\0';

    // handle "empty" command, i.e., only the removed ENTER
    if (0 == strlen(input_buffer)) { return 0; }

    // [History] handle history
    if (strncmp(input_buffer, "!!", 2) == 0) {
        // no command history
        if (strlen(command) == 0) {
            fprintf(stderr, "[Error] No Command History\n");
            return 0;
        }
        // print the stored history, keep the command unchanged and exit
        printf("%s", command);
        return 1;
    }

    // [Non-History] update the command by the inputs
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
    if (args[*size - 1][len - 1] != '&') { return 0; }

    // remove the whole last arg if arg=='&' exactly
    if (len == 1) {
        free(args[*size - 1]);
        args[*size - 1] = NULL;
        --(*size);                      // reduce its size
    }
        // remove only the '&' in the last command
    else { args[*size - 1][len - 1] = '\0'; }


    return 1;
}


/*!
 *   Check the Redirection Symbols in the Arguments
 *      (1) remove "<", ">"         (2) remove the in-/out-put file arg
 *   @param: args           arguments list
 *   @param: size           the number of arguments
 *   @param: input_file     file name for input
 *   @param: output_file    file name for output
 *   @return:               IO flag (bit 1 for output, bit 0 for input)
 */
unsigned check_io_redirection(char **args, size_t *size, char **input_file, char **output_file) {
    unsigned flag = 0;
    size_t to_remove[4], remove_cnt = 0;
    for (size_t i = 0; i <= *size - 1; i++) {
        if (remove_cnt >= 4) { break; }

        // [INPUT] input symbol detected
        if (strcmp("<", args[i]) == 0) {
            to_remove[remove_cnt++] = i;
            if (i == (*size) - 1) {
                fprintf(stderr, "[Error] Input from UNKNOWN File\n");
                break;
            }
            flag |= 1;
            *input_file = args[i + 1];
            to_remove[remove_cnt++] = ++i;
        }

            // [OUTPUT] output symbol detected
        else if (strcmp(">", args[i]) == 0) {
            to_remove[remove_cnt++] = i;
            if (i == (*size) - 1) {
                fprintf(stderr, "[Error] Output to UNKNOWN File\n");
                break;
            }
            flag |= 2;
            *output_file = args[i + 1];
            to_remove[remove_cnt++] = ++i;
        }
    }

    // [REMOVE] Remove I/O indicators and filenames from arguments
    for (int i = remove_cnt - 1; i >= 0; --i) {
        size_t pos = to_remove[i];      // the index of arg to remove
        // printf("%lu %s\n", pos, args[pos]);
        while (pos != *size) {
            args[pos] = args[pos + 1];
            ++pos;
        }
        --(*size);
    }

    return flag;
}


/*!
 *   Open Target Files and Redirect I/O
 *   @param: io_flag        the flag for IO redirection (bit 1 for output, bit 0 for input)
 *   @param: input_file     file name for input
 *   @param: output_file    file name for output
 *   @param: input_decs     file descriptor of input file (like the file-opened object in Python)
 *   @param: output_decs    file descriptor of output file (like the file-opened object in Python)
 *   @return:               1 = successful, 0 = unsuccessful
 */
int redirect_io(unsigned io_flag, char *input_file, char *output_file, int *input_desc, int *output_desc) {
#ifdef DEBUG
    printf("[DEBUG] Redirecting I/O - IO flag: %u\n", io_flag);
#endif
    // open() & authorities: https://blog.csdn.net/sdhgood/article/details/39555311

    // [OUTPUT] Redirect output
    if (io_flag & 2) {
        *output_desc = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 644);    // unmask auth
        if (*output_desc < 0) {
            fprintf(stderr, "[Error] Failed to open the output file: \"%s\"\n", output_file);
            return 0;
        }
#ifdef DEBUG
        printf("\"[DEBUG] Redirecting I/O - Output To: \"%s\" (opened as %d)\n", output_file, *output_desc);
#endif
        dup2(*output_desc, STDOUT_FILENO);
    }

    // [OUTPUT] Redirect output
    if (io_flag & 1) { // redirecting input
        *input_desc = open(input_file, O_RDONLY, 644);                          // unmask auth
        if (*input_desc < 0) {
            fprintf(stderr, "[Error] Failed to open the input file: \"%s\"\n", input_file);
            return 0;
        }
#ifdef DEBUG
        printf("[DEBUG] Redirecting I/O - Input from: \"%s\" (opened as %d)\n", input_file, *input_desc);
#endif
        dup2(*input_desc, STDIN_FILENO);
    }


    return 1;
}


/*!
 *   Close Files for I/O Redirection
 *   @param: io_flag        the flag for IO redirection (bit 1 for output, bit 0 for input)
 *   @param: input_decs     file descriptor of input file
 *   @param: output_decs    file descriptor of output file
 */
void close_file(unsigned io_flag, int input_desc, int output_desc) {
    if (io_flag & 2) { close(output_desc); }
    if (io_flag & 1) { close(input_desc); }
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
            // [PIPE] NO pipe
        else {
            /* Redirect I/O */
            char *input_file, *output_file;
            int input_desc, output_desc;
            unsigned io_flag = check_io_redirection(
                    args, &args_num, &input_file, &output_file);    // bit 1 for output, bit 0 for input
            if (0 == redirect_io(io_flag, input_file, output_file, &input_desc, &output_desc)) {
                return 0;
            }
            size_t exe_result = execvp(args[0], args);
#ifdef DEBUG
            printf("[DEBUG] Execution End with Status %zu\n", exe_result);
#endif
            close_file(io_flag, input_desc, output_desc);
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
    for (size_t i = 0; i <= MAX_LINE / 2; i++) { args[i] = NULL; }
    // [INIT] Initialize the command, set as an empty string
    strcpy(command, "");


    while (1) {
        printf("osh>");
        fflush(stdout);                 // flush the stream buffer

        fflush(stdin);                  // flush the stream buffer
        refresh_args(args);             // empty args before parsing

        // [INPUT] Get and store the input (notice that "empty" command ENTER is a success)
        if (!get_input(command)) { continue; }
#ifdef DEBUG                            // print the stored input command
        printf("[DEBUG] The input command is: \"%s\"\n", command);
#endif

        // [INPUT] Parse the input command
        size_t args_num = parse_input(args, command);
        if (0 == args_num) { continue; }
#ifdef DEBUG                            // print the stored parsed arguments of the nonempty input command
        printf("[DEBUG] The parsed %zu arguments are:\n", args_num);
        for (size_t i = 0; i <= args_num - 1; i++) { printf("\t\"%s\"\n", args[i]); }
#endif

        // [INPUT] [Extra] Enable "exit" command
        if (strcmp(args[0], "exit") == 0) { break; }

        // [INPUT] [Extra] Enable "?", "help", "-help", "--help" command
        if (strcmp(args[0], "?") == 0 || strcmp(args[0], "help") == 0 ||
            strcmp(args[0], "-help") == 0 || strcmp(args[0], "--help") == 0) {
            print_help_msg();
            continue;
        }

        // [RUN] Execute command
        run_command(args, args_num);
    }


    return 0;
}