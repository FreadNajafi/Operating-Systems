/* 
 * File:   main.c
 * Author: a4najafi
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_ARGS 64
#define MAX_CMD_LENGTH 1024

int main() {
    char line[MAX_CMD_LENGTH];
    char *args[MAX_ARGS];

    if (fgets(line, sizeof(line), stdin) == NULL) {
        perror("fgets failed");
        exit(EXIT_FAILURE);
    }

    char *token;
    token = strtok(line, " \n");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;

    // Find the position of the pipe symbol
    int pipe_pos = -1;
    for (int j = 0; args[j] != NULL; j++) {
        if (strcmp(args[j], "|") == 0) {
            pipe_pos = j;
            break;
        }
    }

    if (pipe_pos == -1 || args[pipe_pos + 1] == NULL) {
        fprintf(stderr, "Invalid command\n");
        exit(EXIT_FAILURE);
    }

    // Create a pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
    
        close(pipe_fd[1]); 

        // Redirect stdin to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);

        // Execute the command after the pipe symbol
        execvp(args[pipe_pos + 1], &args[pipe_pos + 1]);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
       
        close(pipe_fd[0]);

        dup2(pipe_fd[1], STDOUT_FILENO);

        // Execute the command before the pipe symbol
        args[pipe_pos] = NULL; 
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
