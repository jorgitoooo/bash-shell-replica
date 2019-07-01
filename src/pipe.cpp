#include "pipe.h"
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



Pipe::Pipe(Base* l_child, Base* r_child) : Decorator(l_child) { this->r_child = r_child; }

bool Pipe::evaluate()
{

	int pipefd[2];

	// pipe
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		printf("errno %d\n", errno);
	}
	
	// fork & execvp
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("Failed to create a child process");
		exit(EXIT_FAILURE);	
	}
	else if (pid == 0)
	{
		// dup2 called in child process to leave parent's stdout open
		int new_fd = dup2(pipefd[1], 1);

		// close read end of pipe
		close(pipefd[0]);

		bool success = this->child->evaluate();

		success ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{

    		int childStatus, childStatus_2;
    		pid_t checkDone = waitpid(-1, &childStatus, 0);
	
		// close write end of pipe
		close(pipefd[1]);

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
			}
		}

		pid_t pid_2 = fork();
		
		if (pid_2 < 0)
		{
			perror("Failed to create a second child process");
			exit(EXIT_FAILURE);	
		}
		else if (pid_2 == 0)
		{
			// dup2 called in second child process to leave parent's stdin open
			int new_fd = dup2(pipefd[0], 0);
			
			bool success = this->r_child->evaluate();

			success ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
		}
    		
		pid_t checkDone_2 = waitpid(-1, &childStatus_2, 0);

		if (checkDone_2 == -1)
		{
			perror("Error on waitpid system call"); 
			exit(EXIT_FAILURE); 
		}

		if (checkDone_2 == pid_2)
		{
			if (WIFEXITED(childStatus_2)) 
			{
				if(WEXITSTATUS(childStatus_2) != 0)
				{
					return false;
				}
				return true;
			}
		}

	}
        return true; 
}

std::string Pipe::stringify()
{

	return this->child->stringify() + " | " + this->r_child->stringify(); 
}

std::string Pipe::getClassType()
{
	return "decorator"; 
}
