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

#define WELCOME "$ ./enseash\nWelcome to ENSEA Tiny Shell.\nType 'exit' to quit.\nenseash %"
#define ENSEASH "enseash %"

void welcome(){	
	// Display the welcome message
	write(STDOUT_FILENO, WELCOME, strlen(WELCOME) );
}

void input(char *buffer){
    // The user command length
	ssize_t usercommand;	
    // Read the user command
	usercommand = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
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
		perror("fork failed");
		return;
	} else if (pid == 0) {
		// Child process: execute the user command
        char *args[] = {(char *)buffer, NULL}; // Prepare arguments for execvp
        execvp(args[0], args); // Execute the command
		// If execvp fails, print an error and exit the child process
        perror("Incorrect command (execvp failed)");
		return;
	} else {
        // Parent process: wait for the child to finish
        wait(NULL);
    }
}

void enseash(){
	// Display the enseash message
	write(STDOUT_FILENO, ENSEASH, strlen(ENSEASH) );
}

int main(){
	
	welcome(); // QUESTION 1
	
	// Loop that read the user command, execute it and ask again for user command.
	while(1){
		// The user command variable
		char buffer[1024];
		
		input(buffer); // QUESTION 2
		
		if (enter(buffer)){ continue; }
				
		process(buffer); // QUESTION 2
		
		enseash(); // QUESTION 2

	}

	return 0;
}
