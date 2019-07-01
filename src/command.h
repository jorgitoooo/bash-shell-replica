#pragma once 

#include "base.hpp" 
#include <string> 
#include <vector> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

class Command : public Base {
public: 

std::string commandString; 
std::vector<std::string> argVector;   


//constructor taking in a string that specifies the command, and the vector of strings for each possible argument 
Command(std::string commandString, std::vector<std::string> argVector); 

bool evaluate(); 

std::string stringify(); 

std::string getClassType(); 
}; 
