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

int main()
{
	// QUESTION 1.
	
	// The welcome message that we want to display
	const char *welcomemessage = "$ ./enseash\nWelcome to ENSEA Tiny Shell.\nType 'exit' to quit.\nenseash % ";
	
	// The number of characters in our message
	int lengthmessage = strlen(welcomemessage);
	
	// Display the welcome message
	write(STDOUT_FILENO, welcomemessage, lengthmessage);
	

	
	// QUESTION 2.

	// Loop that read the user command, execute it and ask again for user command.
	while(1)
	{
	// The maximum user command length
    char buffer[1024];
    
    // The user command length
    ssize_t usercommand;	
	
	// Read the user command
	usercommand = read(STDIN_FILENO, buffer, sizeof(buffer));
	
	// Remove the next line character
    buffer[usercommand - 1] = '\0';

/*
    // New process to execute the command
    pid_t pid = fork();

	if (pid == -1) {
        // Error handling for fork()
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process: execute the user command
        // Use execvp to execute the command
        char *args[] = {buffer, NULL}; // Prepare arguments for execvp
        execvp(args[0], args); // Execute the command
    // If execvp fails, print an error and exit the child process
        perror("execvp failed");
        return 1;
    } else {
        // Parent process: wait for the child to finish
        wait(NULL);
    }

	// The prompt enseash that we want to display
	const char *enseash = "enseash % ";
	
	// The number of characters in our message
	int lengthenseash = strlen(enseash);
	
	// Display the enseash message
	write(STDOUT_FILENO, enseash, lengthenseash);

	}
*/


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
		const char *exitmessage = "Bye bye...\n$";
		int lengthexitmessage = strlen(exitmessage);
        write(STDOUT_FILENO, exitmessage, lengthexitmessage);        
        break;
    }

	    // New process to execute the command
    pid_t pid = fork();

	if (pid == -1) {
        // Error handling for fork()
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process: execute the user command
        // Use execvp to execute the command
        char *args[] = {buffer, NULL}; // Prepare arguments for execvp
        execvp(args[0], args); // Execute the command
    // If execvp fails, print an error and exit the child process
        perror("execvp failed");
        return 1;
    } else {
        // Parent process: wait for the child to finish
        wait(NULL);
    }

	// The prompt enseash that we want to display
	const char *enseash = "enseash % ";
	
	// The number of characters in our message
	int lengthenseash = strlen(enseash);
	
	// Display the enseash message
	write(STDOUT_FILENO, enseash, lengthenseash);

	}


	return 0;
}

