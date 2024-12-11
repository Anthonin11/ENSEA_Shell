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
	const char *welcomemessage = "$ ./enseash\nWelcome to ENSEA Tiny Shell.\nType 'exit' to quit.\nenseash %";
	// Display the welcome message
	write(STDOUT_FILENO, welcomemessage, strlen(welcomemessage) );
}	

int main(){
	
	welcome(); // QUESTION 1
	
	return 0;
}


