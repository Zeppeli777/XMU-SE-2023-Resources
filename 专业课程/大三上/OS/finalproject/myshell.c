/*
 * myshell.c - A simple command line shell
 * 
 * This program implements a basic command line interpreter with support for:
 * - Internal commands (cd, clr, dir, environ, echo, help, pause, quit)
 * - External program execution
 * - I/O redirection (<, >, >>)
 * - Background execution (&)
 * - Batch file processing
 * 
 * Author: Tengkai Pan(37220232203786)
 * Date: 2025-12-10
 */

#include "myshell.h"

/* Global variables */
char *batchfile = NULL;  /* Batch file name if running in batch mode */
FILE *input_stream = NULL;  /* Input stream (stdin or batch file) */

/*
 * Main function - Entry point of the shell
 */
int main(int argc, char *argv[]) {
    char command_line[MAX_LINE];
    char *args[MAX_ARGS];
    char *input_file = NULL;
    char *output_file = NULL;
    int append_mode = 0;
    int background = 0;
    
    /* Set shell environment variable to full path of executable */
    set_shell_env(argv[0]);
    
    /* Check if running in batch mode */
    if (argc == 2) {
        batchfile = argv[1];
        input_stream = fopen(batchfile, "r");
        if (input_stream == NULL) {
            fprintf(stderr, "myshell: cannot open batch file: %s\n", batchfile);
            exit(EXIT_FAILURE);
        }
    } else {
        input_stream = stdin;
    }
    
    /* Main shell loop */
    while (1) {
        int i;
        
        /* Display prompt only in interactive mode */
        if (input_stream == stdin) {
            display_prompt();
        }
        
        /* Read command line */
        if (fgets(command_line, MAX_LINE, input_stream) == NULL) {
            break;  /* EOF or error */
        }
        
        /* Remove newline and carriage return characters */
        command_line[strcspn(command_line, "\r\n")] = '\0';
        
        /* Skip empty lines */
        if (strlen(command_line) == 0) {
            continue;
        }
        
        /* Echo command in batch mode */
        if (input_stream != stdin) {
            printf("%s\n", command_line);
        }
        
        /* Initialize args array */
        for (i = 0; i < MAX_ARGS; i++) {
            args[i] = NULL;
        }
        
        /* Parse command line */
        int arg_count = parse_command(command_line, args, &input_file, 
                                     &output_file, &append_mode, &background);
        
        if (arg_count == 0) {
            continue;
        }
        
        /* Execute command */
        execute_command(args, input_file, output_file, append_mode, background);
        
        /* Free allocated memory from parse_command */
        for (i = 0; i < MAX_ARGS && args[i] != NULL; i++) {
            free(args[i]);
            args[i] = NULL;
        }
        if (input_file != NULL) {
            free(input_file);
            input_file = NULL;
        }
        if (output_file != NULL) {
            free(output_file);
            output_file = NULL;
        }
        
        /* Reset variables for next iteration */
        append_mode = 0;
        background = 0;
    }
    
    /* Cleanup */
    if (input_stream != stdin) {
        fclose(input_stream);
    }
    
    return 0;
}

/*
 * Display command prompt with current working directory
 */
void display_prompt(void) {
    char cwd[MAX_PATH];
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s> ", cwd);
    } else {
        printf("myshell> ");
    }
    fflush(stdout);
}

/*
 * Set shell environment variable to full path
 */
void set_shell_env(char *argv0) {
    char path[MAX_PATH];
    
    /* Get absolute path of shell executable */
    if (realpath(argv0, path) != NULL) {
        char env_var[MAX_PATH + 10];
        snprintf(env_var, sizeof(env_var), "shell=%s", path);
        putenv(strdup(env_var));
    }
}

/*
 * Execute internal or external commands
 */
void execute_command(char **args, char *input_file, char *output_file, 
                    int append_mode, int background) {
    /* Check for internal commands */
    if (strcmp(args[0], "cd") == 0) {
        builtin_cd(args);
    } else if (strcmp(args[0], "clr") == 0) {
        builtin_clr(args);
    } else if (strcmp(args[0], "dir") == 0) {
        builtin_dir(args, output_file, append_mode);
    } else if (strcmp(args[0], "environ") == 0) {
        builtin_environ(args);
    } else if (strcmp(args[0], "echo") == 0) {
        builtin_echo(args, output_file, append_mode);
    } else if (strcmp(args[0], "help") == 0) {
        builtin_help(args);
    } else if (strcmp(args[0], "pause") == 0) {
        builtin_pause(args);
    } else if (strcmp(args[0], "quit") == 0) {
        builtin_quit(args);
    } else {
        /* External command */
        execute_external(args, input_file, output_file, append_mode, background);
    }
}

/*
 * Execute external program
 */
void execute_external(char **args, char *input_file, char *output_file,
                      int append_mode, int background) {
    pid_t pid;
    int status;
    
    pid = fork();
    
    if (pid < 0) {
        perror("myshell: fork failed");
        return;
    } else if (pid == 0) {
        /* Child process */
        
        /* Handle input redirection */
        if (input_file != NULL) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("myshell: input redirection failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        
        /* Handle output redirection */
        if (output_file != NULL) {
            int fd;
            if (append_mode) {
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (fd < 0) {
                perror("myshell: output redirection failed");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        /* Execute the program */
        execvp(args[0], args);
        
        /* If execvp returns, an error occurred */
        fprintf(stderr, "myshell: %s: command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else {
        /* Parent process */
        if (!background) {
            /* Wait for child to complete if not background */
            waitpid(pid, &status, 0);
        } else {
            /* Background process - don't wait */
            printf("[Background] PID: %d\n", pid);
        }
    }
}
