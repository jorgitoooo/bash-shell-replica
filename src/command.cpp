#include "command.h" 


Command::Command(std::string commandString, std::vector<std::string> argVector)
{
	this->commandString = commandString; 
	this->argVector = argVector; 
}

bool Command::evaluate() 
{

	if (commandString == "test") 
	{
		bool ret_val = false;

		//There should be more than one argument since argVector will
		//  at least contain '-e' flag
		if(argVector.size() < 2)
		{
			fprintf(stderr, "Not enough arguments passed\n");
			return false;
		}

		struct stat buf;

		int stat_val = stat(argVector[1].c_str(), &buf);

		if(argVector[0] == "-e")
		{
			if(stat_val == -1)
			{
				std::cout << "(FALSE)\n";
				ret_val = false;
			}
			else
			{
				std::cout <<"(TRUE)\n";
				ret_val = true;
			}
		}
		else if(argVector[0] == "-f")
		{
			if(stat_val == -1)
			{
				std::cout << "(FALSE)\n";
				ret_val = false;
			}
			else
			{
				if(S_ISREG(buf.st_mode))
				{
					std::cout << "(TRUE)\n";
					ret_val = true;
				}
				else
				{
					std::cout << "(FALSE)\n";
					ret_val = false;
				}
			}
		}
		else if(argVector[0] == "-d")
		{
			if(stat_val == -1)
			{
				std::cout << "(FALSE)\n";
				ret_val = false;
			}
			else
			{
				if(S_ISDIR(buf.st_mode))
				{
					std::cout << "(TRUE)\n";
					ret_val = true;
				}
				else
				{
					std::cout << "(FALSE)\n";
					ret_val = false;
				}
			}

		}
		else
		{
			if(argVector[0].length() > 1)
			{
				if(argVector[0][0] == '-') fprintf(stderr, "%s: Is not a flag\n", argVector[0].c_str() + 1);
				else fprintf(stderr, "%s: Is not a flag\n", argVector[0].c_str());
			}
			else
			{
				fprintf(stderr, "%s: Is not a flag\n", argVector[0].c_str());
			}
			ret_val = false;
                }

		return ret_val;
	}


	//immediately see if we are running the 'exit' command
	if (commandString == "exit") 
	{
		exit(EXIT_SUCCESS); 
	}

	//extra 2 spots for command being passed in 'execvp' call itself and the NULL for last spot in array
	int execvpArraySize = this->argVector.size() + 2;

	char* execvpArray[execvpArraySize]; 

	execvpArray[0] = (char*)commandString.c_str();  

	//If we have at least 1 argument with the command 	
	if (argVector.size() > 0) 
	{

		for (int i = 0; i < argVector.size(); i++) 
		{
			execvpArray[i + 1] = (char*)argVector[i].c_str(); 
 
			//Need a NULL at the last spot in array 
			if (i == argVector.size() - 1) 
			{
				execvpArray[i + 2] = NULL;  
			}	
		}	
	}

	//If we have no arguments with command
	else {
		execvpArray[1] = NULL; 
	}
	
	int processID = fork();

    	if (processID < 0)
    	{
        	perror("Error on fork system call"); 
		exit(EXIT_FAILURE);
    	}	

    	if (processID == 0)
    	{
    		//Must be in the childprocess right here. Call execvp() and launch the 'ls' command
    	
     		int execvpStatus = execvp(execvpArray[0],execvpArray);
     		
		//check for execvp failure 
		if (execvpStatus == -1) 
		{
//***********************
			std::cout << this->stringify() << std::endl;
//***********************
			perror("Error on execvp system call"); 
			exit(EXIT_FAILURE); 
		}
   	 }

    	//We will be in the parent after this line due to the 'if' statement above
    	int childStatus;
    	int checkDone = waitpid(-1, &childStatus,0);

    	if (checkDone == -1)
    	{
        	perror("Error on waitpid system call"); 
		exit(EXIT_FAILURE); 
    	}
    	//Loop runs while the child process is still doing its work
    	while (checkDone == 0)
    	{
    		checkDone = waitpid(-1, &childStatus, 0);
   	}

	//Check for completed process, see if fail/success
    	if (checkDone == processID)
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
	//Should never reach this
	return true; 
}


std::string Command::stringify() 
{

	std::string stringifyString(commandString); 
	
	for (int i = 0; i < this->argVector.size(); i++) 
	{

		std::string currArgString = argVector[i]; 
		stringifyString = stringifyString + " " +  currArgString; 
	}

	return stringifyString; 


}

std::string Command::getClassType() 
{
	return "command"; 
}
