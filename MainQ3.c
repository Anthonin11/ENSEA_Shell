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
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void welcome(){	
	// The welcome message that we want to display
	const char *welcomemessage = "$ ./enseash\nWelcome to ENSEA Tiny Shell.\nType 'exit' to quit.\n";
		
	// Display the welcome message
	write(STDOUT_FILENO, welcomemessage, strlen(welcomemessage) );
}	

int main(){
	
	welcome(); // QUESTION 1
	
	// To store the return status or signal of the previous command
	int last_exit_status = -1;
	

	// Loop that read the user command, execute it and ask again for user command.
	while(1){
		
		// The maximum user command length
		char buffer[1024];
    
		// The user command length
		ssize_t usercommand;	
	
		// Print the prompt enseash
        if (last_exit_status == -1) {
            // No previous command executed
            write(STDOUT_FILENO, "enseash % ", 10);
        } else if (WIFEXITED(last_exit_status)) {
            // Command exited normally
            dprintf(STDOUT_FILENO, "enseash [exit:%d|%dms] %% ", last_exit_status, last_exit_status);
        } else if (WIFSIGNALED(last_exit_status)) {
            // Command terminated by a signal
            dprintf(STDOUT_FILENO, "enseash [sign:%d|%dms] %% ", last_exit_status, last_exit_status);
        }
	
		// Read the user command
		usercommand = read(STDIN_FILENO, buffer, sizeof(buffer));
	
		// Remove the next line character
		buffer[usercommand - 1] = '\0';


		// QUESTION 3
	
		// Management of the shell output with the command "exit"
		if (strcmp(buffer, "exit") == 0) {
				const char *exitmessage = "Bye bye...\n$";
				int lengthexitmessage = strlen(exitmessage);
				write(STDOUT_FILENO, exitmessage, lengthexitmessage);
				break;
		}
	
		// Management of the shell output with <ctrl>+d
		if (usercommand == 0) {
			const char *exitmessage = "\nBye bye...\n$";
			int lengthexitmessage = strlen(exitmessage);
			write(STDOUT_FILENO, exitmessage, lengthexitmessage);        
			break;
		}
		
		// Check if the user only pressed Enter (empty command)
        if (buffer[0] == '\0') {
            // Do nothing, just re-display the prompt
            continue;
        }

		 // Measure start time using clock_gettime()
        struct timespec start_time, end_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);

		// New process to execute the command
		pid_t pid = fork();

		if (pid == -1) {
            // Error handling for fork
            perror("fork failed");
            return 1;
        } else if (pid == 0) {
            // Child process: execute the user command
            char *args[] = {buffer, NULL};
            execvp(args[0], args);
            // If execvp fails, print an error and exit the child process
            perror("execvp failed");
            return 1;
        } else {
            // Parent process: wait for the child to finish and get the exit status
            int status;
            waitpid(pid, &status, 0);
			
			// Measure end time after the command finishes
            clock_gettime(CLOCK_MONOTONIC, &end_time);

			// Calculate elapsed time in milliseconds
            long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 + (end_time.tv_nsec - start_time.tv_nsec) / 1000000;
			
			// Check if the child process exited normally or was terminated by a signal
            if (WIFEXITED(status)) {
                last_exit_status = WEXITSTATUS(status);  // Command exited normally
            } else if (WIFSIGNALED(status)) {
                last_exit_status = WTERMSIG(status);  // Command terminated by a signal
            }
        
			// Display the prompt with exit status and elapsed time
            if (WIFEXITED(status)) {
                // Command exited normally
                dprintf(STDOUT_FILENO, "enseash [exit:%d|%ldms] %% ", last_exit_status, elapsed_time);
            } else if (WIFSIGNALED(status)) {
                // Command terminated by a signal
                dprintf(STDOUT_FILENO, "enseash [sign:%d|%ldms] %% ", last_exit_status, elapsed_time);
            }
        }
	}


	return 0;
}


