/*
 * Main.c
 * 
 * Copyright 2024 ensea <ensea@StudentLab>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

#define WELCOME "$ ./enseash\nWelcome to ENSEA Tiny Shell.\nType 'exit' to quit.\nenseash % "
#define ENSEASH "enseash % "
#define ERRORFORK "Error (fork failed)"
#define ERROREXEC "Error (execvp failed)"
#define BYE "\nBye bye...\n$"
#define EXITSTATUE
#define ENSEASHEXIT "enseash [exit:%d|%lldms] %% "
#define ENSEASHSIGN "enseash [sign:%d|%lldms] %% "
int lastexit = -1;
long long lasttime = 0;

void welcome(){	
	// Display the welcome message
	write(STDOUT_FILENO, WELCOME, strlen(WELCOME) );
}

void input(char *buffer){
    // The user command length
	ssize_t usercommand;	
    // Read the user command
	usercommand = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	// Check if <Ctrl>+D is pressed
    if (usercommand == 0) {
        // Set buffer to exit
        strcpy(buffer,"exit");
        return;
    }
    // Remove newline and null-terminate
    buffer[usercommand - 1] = '\0';
}

int enter(const char *buffer){
	// If only ENTER is pressed do nothing
    if (strlen(buffer) == 0) {
		// Re-display the prompt
        write(STDOUT_FILENO, ENSEASH, strlen(ENSEASH) );
        return 1;
    }
    return 0;
}

void process(const char *buffer){
	// New process to execute the command
	pid_t pid = fork();
	if (pid == -1) {
		// Error handling for fork()
		perror(ERRORFORK);
		lastexit = -1;
		return;
	} else if (pid == 0) {
		// Child process: execute the user command
		// Create a modifiable buffer to tokenize
        char buffer2[1024];  // buffer to hold the command and arguments
        strncpy(buffer2, buffer, sizeof(buffer2));  // Copy the input into buf
        buffer2[sizeof(buffer2) - 1] = '\0';  // Ensure null-termination

		// Tokenize the command into arguments
        char *args[128]; // assuming a maximum of 128 arguments
        char *token = strtok(buffer2, " ");
        int i = 0;
        int input_redirect = -1;
        int output_redirect = -1;
        char *input_file = NULL;
        char *output_file = NULL;
        while (token != NULL) {
            if (strcmp(token, "<") == 0) {
                input_redirect = 1;
                input_file = strtok(NULL, " ");
            } else if (strcmp(token, ">") == 0) {
                output_redirect = 1;
                output_file = strtok(NULL, " ");
            } else {
                args[i++] = token;
            }
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Null-terminate the arguments array
        // Handle input redirection
        if (input_redirect != -1) {
            if (freopen(input_file, "r", stdin) == NULL) {
                perror("Error opening input file");
                exit(1);
            }
        }
        // Handle output redirection
        if (output_redirect != -1) {
            if (freopen(output_file, "w", stdout) == NULL) {
                perror("Error opening output file");
                exit(1);
            }
        }
        execvp(args[0], args); // Execute the command
		// If execvp fails, print an error and exit the child process
        perror(ERROREXEC);
        lastexit = -1;
		return;
	} else {
        // Parent process: wait for the child to finish
        int status;
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start); // Start time
        waitpid(pid, &status, 0);
        clock_gettime(CLOCK_MONOTONIC, &end); // End time
        long long elapsedtime = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        lasttime = elapsedtime / 1000000;
        // Check if the process terminated normally or by a signal
        if (WIFEXITED(status)) {
            lastexit = WEXITSTATUS(status);  // Normal exit
        } else if (WIFSIGNALED(status)) {
            lastexit = WTERMSIG(status);  // Terminated by a signal
        }
    }
}

void enseash(){
	if (lastexit == -1) {
        // No previous command: enseash
        write(STDOUT_FILENO, ENSEASH, strlen(ENSEASH));
    } else if (WIFEXITED(lastexit)) {
        // Previous command exited normally: enseash[exit]
        char buffer[64];
        snprintf(buffer, sizeof(buffer), ENSEASHEXIT, lastexit, lasttime);
        write(STDOUT_FILENO, buffer, strlen(buffer));
    } else if (WIFSIGNALED(lastexit)) {
        // Previous command terminated with signal: enseash[sign]
        char buffer[64];
        snprintf(buffer, sizeof(buffer), ENSEASHSIGN, lastexit, lasttime);
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
}

int output(const char *buffer){
	// Management of the shell output
    if (strcmp(buffer, "exit") == 0) {
        write(STDOUT_FILENO, BYE, strlen(BYE));
        return 1;
    }
	return 0;
}

int main(){
	
	welcome(); // QUESTION 1
	
	// Loop that read the user command, execute it and ask again for user command.
	while(1){
		// The user command variable
		char buffer[1024];
		
		input(buffer); // QUESTION 2
		
		if (enter(buffer)){ continue; }
				
		if (output(buffer)){ break; } // QUESTION 3
				
		process(buffer); // QUESTION 2 & 5 & 6
		
		enseash(); // QUESTION 2 & 4 & 5

	}

	return 0;
}
