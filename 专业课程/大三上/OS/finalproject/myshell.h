/*
 * myshell.h - Header file for myshell
 * 
 * Contains function prototypes, constants, and necessary includes
 */

#ifndef MYSHELL_H
#define MYSHELL_H

/* Standard library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

/* Constants */
#define MAX_LINE 1024      /* Maximum command line length */
#define MAX_ARGS 64        /* Maximum number of arguments */
#define MAX_PATH 4096      /* Maximum path length */

/* External environment variable */
extern char **environ;

/* Function prototypes - Main functions (myshell.c) */
void display_prompt(void);
void set_shell_env(char *argv0);
void execute_command(char **args, char *input_file, char *output_file, 
                    int append_mode, int background);
void execute_external(char **args, char *input_file, char *output_file,
                      int append_mode, int background);

/* Function prototypes - Utility functions (utility.c) */
int parse_command(char *command_line, char **args, char **input_file,
                 char **output_file, int *append_mode, int *background);
void trim_whitespace(char *str);

/* Function prototypes - Built-in commands (utility.c) */
void builtin_cd(char **args);
void builtin_clr(char **args);
void builtin_dir(char **args, char *output_file, int append_mode);
void builtin_environ(char **args);
void builtin_echo(char **args, char *output_file, int append_mode);
void builtin_help(char **args);
void builtin_pause(char **args);
void builtin_quit(char **args);

#endif /* MYSHELL_H */
