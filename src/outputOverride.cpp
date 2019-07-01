#include "outputOverride.h"
#include "input.h"
#include <iostream>
#include <string>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // O_*

#include <unistd.h> // dup, dup2
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

OutputOverride::OutputOverride(Base* child) : Decorator(child) {};


bool OutputOverride::evaluate()
{
	int fd = open(
			this->rightPath.c_str(),
			O_RDWR  | O_CREAT | O_TRUNC,
			S_IRWXU | S_IRWXG | S_IRWXO );

	if (fd == -1)
	{
		perror("Unable to open file");
		return false;
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		perror("Failed to create a child process");
		exit(EXIT_FAILURE);	
	}
	else if (pid == 0)
	{
		// dup2 called in child process to leave parent's stdout open
		int new_fd = dup2(fd, 1);

		bool success = this->child->evaluate();

		success ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
    		int childStatus;
    		pid_t checkDone = waitpid(-1, &childStatus,0);
	
		if (checkDone == -1)
		{
			perror("Error on waitpid system call"); 
			exit(EXIT_FAILURE); 
		}
		
		if (checkDone == pid)
		{
			if (WIFEXITED(childStatus)) 
			{
				if(WEXITSTATUS(childStatus) != 0)
				{
					return false;
				}
				return true; 
			}	
		}
	}
        return true;
}

std::string OutputOverride::stringify()
{
	return this->child->stringify(); 
}

std::string OutputOverride::getClassType() 
{
	return "decorator";  
}
