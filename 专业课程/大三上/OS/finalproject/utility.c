/*
 * utility.c - Utility functions and built-in commands for myshell
 * 
 * This file contains:
 * - Command line parsing function
 * - All built-in command implementations
 * - Helper functions
 */

#include "myshell.h"

/*
 * Parse command line into arguments and detect redirections/background
 * 
 * Returns: number of arguments
 */
int parse_command(char *command_line, char **args, char **input_file,
                 char **output_file, int *append_mode, int *background) {
    int i = 0;
    char *token;
    char line_copy[MAX_LINE];
    
    /* Make a copy of the command line */
    strncpy(line_copy, command_line, MAX_LINE - 1);
    line_copy[MAX_LINE - 1] = '\0';
    
    /* Tokenize the command line */
    token = strtok(line_copy, " \t");
    
    while (token != NULL && i < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            /* Input redirection */
            token = strtok(NULL, " \t");
            if (token != NULL) {
                *input_file = strdup(token);
            }
        } else if (strcmp(token, ">") == 0) {
            /* Output redirection (overwrite) */
            token = strtok(NULL, " \t");
            if (token != NULL) {
                *output_file = strdup(token);
                *append_mode = 0;
            }
        } else if (strcmp(token, ">>") == 0) {
            /* Output redirection (append) */
            token = strtok(NULL, " \t");
            if (token != NULL) {
                *output_file = strdup(token);
                *append_mode = 1;
            }
        } else if (strcmp(token, "&") == 0) {
            /* Background execution */
            *background = 1;
        } else {
            /* Regular argument */
            args[i++] = strdup(token);
        }
        
        token = strtok(NULL, " \t");
    }
    
    args[i] = NULL;
    return i;
}

/*
 * Built-in command: cd
 * Change current directory
 */
void builtin_cd(char **args) {
    char cwd[MAX_PATH];
    
    if (args[1] == NULL) {
        /* No argument - display current directory */
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("myshell: getcwd failed");
        }
    } else {
        /* Change to specified directory */
        if (chdir(args[1]) != 0) {
            perror("myshell: cd failed");
        } else {
            /* Update PWD environment variable */
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                setenv("PWD", cwd, 1);
            }
        }
    }
}

/*
 * Built-in command: clr
 * Clear the screen
 */
void builtin_clr(char **args) {
    (void)args;  /* Unused parameter */
    
    /* Use ANSI escape codes to clear screen */
    printf("\033[2J\033[H");
    fflush(stdout);
}

/*
 * Built-in command: dir
 * List directory contents
 */
void builtin_dir(char **args, char *output_file, int append_mode) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char *dirname = ".";
    FILE *output = stdout;
    
    /* Determine directory to list */
    if (args[1] != NULL) {
        dirname = args[1];
    }
    
    /* Handle output redirection */
    if (output_file != NULL) {
        if (append_mode) {
            output = fopen(output_file, "a");
        } else {
            output = fopen(output_file, "w");
        }
        if (output == NULL) {
            perror("myshell: cannot open output file");
            return;
        }
    }
    
    /* Open directory */
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("myshell: cannot open directory");
        if (output != stdout) fclose(output);
        return;
    }
    
    /* Read and display directory entries */
    fprintf(output, "Directory listing of %s:\n", dirname);
    fprintf(output, "%-30s %10s %s\n", "Name", "Size", "Type");
    fprintf(output, "-----------------------------------------------------------\n");
    
    while ((entry = readdir(dir)) != NULL) {
        char fullpath[MAX_PATH];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirname, entry->d_name);
        
        if (stat(fullpath, &file_stat) == 0) {
            char *type = S_ISDIR(file_stat.st_mode) ? "DIR" : "FILE";
            fprintf(output, "%-30s %10ld %s\n", 
                   entry->d_name, 
                   (long)file_stat.st_size, 
                   type);
        } else {
            fprintf(output, "%-30s %10s %s\n", entry->d_name, "?", "?");
        }
    }
    
    closedir(dir);
    
    if (output != stdout) {
        fclose(output);
    }
}

/*
 * Built-in command: environ
 * List all environment variables
 */
void builtin_environ(char **args) {
    (void)args;  /* Unused parameter */
    
    char **env = environ;
    
    printf("Environment variables:\n");
    printf("-----------------------------------------------------------\n");
    
    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }
}

/*
 * Built-in command: echo
 * Display a line of text
 */
void builtin_echo(char **args, char *output_file, int append_mode) {
    FILE *output = stdout;
    int i = 1;
    
    /* Handle output redirection */
    if (output_file != NULL) {
        if (append_mode) {
            output = fopen(output_file, "a");
        } else {
            output = fopen(output_file, "w");
        }
        if (output == NULL) {
            perror("myshell: cannot open output file");
            return;
        }
    }
    
    /* Print all arguments separated by space */
    while (args[i] != NULL) {
        fprintf(output, "%s", args[i]);
        if (args[i + 1] != NULL) {
            fprintf(output, " ");
        }
        i++;
    }
    fprintf(output, "\n");
    
    if (output != stdout) {
        fclose(output);
    }
}

/*
 * Built-in command: help
 * Display user manual using more filter
 */
void builtin_help(char **args) {
    (void)args;  /* Unused parameter */
    
    /* Try to open readme file and pipe through more */
    FILE *readme = fopen("readme", "r");
    
    if (readme == NULL) {
        fprintf(stderr, "myshell: cannot open readme file\n");
        fprintf(stderr, "Please ensure 'readme' file exists in current directory\n");
        return;
    }
    
    fclose(readme);
    
    /* Use more command to display readme */
    int ret = system("more readme");
    if (ret != 0) {
        /* If more is not available, try less or cat */
        ret = system("less readme");
        if (ret != 0) {
            ret = system("cat readme");
            if (ret != 0) {
                fprintf(stderr, "myshell: failed to display readme file\n");
            }
        }
    }
}

/*
 * Built-in command: pause
 * Pause shell operation until Enter is pressed
 */
void builtin_pause(char **args) {
    (void)args;  /* Unused parameter */
    
    printf("Shell paused. Press Enter to continue...");
    fflush(stdout);
    
    getchar();
}

/*
 * Built-in command: quit
 * Exit the shell
 */
void builtin_quit(char **args) {
    (void)args;  /* Unused parameter */
    
    printf("Exiting myshell...\n");
    exit(0);
}

/*
 * Trim leading and trailing whitespace from a string
 */
void trim_whitespace(char *str) {
    char *start = str;
    char *end;
    
    /* Trim leading whitespace */
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    
    /* All spaces? */
    if (*start == 0) {
        *str = 0;
        return;
    }
    
    /* Trim trailing whitespace */
    end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t')) {
        end--;
    }
    
    /* Write new null terminator */
    *(end + 1) = '\0';
    
    /* Move trimmed string to beginning */
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}
