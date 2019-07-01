#include <iostream> 
#include <string>
#include <bits/stdc++.h> 
#include "command.h" 
#include "and.h" 
#include "or.h" 
#include "semiColon.h" 

int main() 
{
while (1)
{
	std::cout << "$ ";  

	std::string str;
	std::getline(std::cin, str);

	std::vector<std::string> tokenVector;
	std::string s;
     	std::stringstream in(str);
	while(in >> s)
	{
  		tokenVector.push_back(s);
    	}

	//Set up vectors for main parsing/building use.
	std::vector<Command*> commandVector;
        std::vector<char*> connectorVector;

	//Create the connectors vector 
	for (int i = 0; i < tokenVector.size(); i++) 
	{

		int currTokenLastIndex = tokenVector[i].size() - 1; 

		if (tokenVector[i] == "&&") 
		{
			connectorVector.push_back("&&"); 
		}
		else if (tokenVector[i] == "||") 
		{
			connectorVector.push_back("||"); 
		}
		else if (tokenVector[i] == ";")
		{
			connectorVector.push_back(";"); 
		}
		
		else if (tokenVector[i][currTokenLastIndex] == ';') 
		{
			connectorVector.push_back(";"); 
		}

	}
	//Fields for building initial command vector. Will be used as a buffer. 
	std::vector<std::string> commandArgs; 
	
	for (int i = 0; i < tokenVector.size(); i++) 
	{
		bool foundComment = false; 
		bool foundSemiColon = false; 
	 	bool foundFirstQuote = false; 
		bool foundLastQuote = false; 
		
		//clear argument vector on every for-loop run, because we constructed a Command and passed by copy the vector. No longer needed, 
		//and must be ready for next command's arguments. 
		commandArgs.clear();

		//Use this for the 'while' loop later
		std::string commandString = tokenVector[i];

		//Check for a pure '#' comment on each iteration. 
		if (tokenVector[i] == "#") 
		{
			foundComment = true;
			break;
		} 
		//Check for '#' at beginning of a token 
		if (tokenVector[i][0] == '#') 
		{
			foundComment = true; 
			break; 
		}

		int commandLastIndex = tokenVector[i].size() - 1; 
		
		//Check for ';' at end of command
		if (tokenVector[i][commandLastIndex] == ';' && tokenVector[i].size() > 1)
		{
			commandString = tokenVector[i]; 
			commandString.pop_back();
			Command* currCommand = new Command(commandString, commandArgs); 
			commandVector.push_back(currCommand); 
			foundSemiColon = true; 
		}	 		

		else if ((tokenVector[i] != "&&") && (tokenVector[i] != "||") && (tokenVector[i] != ";"))
		{

			commandString = tokenVector[i]; 

			//Loop and get command's flags. End of vector or connector signals end of flags 
			while ((i + 1 != tokenVector.size()) && (tokenVector[i + 1] != "&&") && (tokenVector[i + 1] != "||") && (tokenVector[i + 1] != ";"))
			{

				int stringLastIndex = tokenVector[i + 1].size() - 1;

				
 
				//Check for semi-colon at end of flag. If found, make sure to add flag, build command and break.  
				//THIS IS BROKEN. 
				if (tokenVector[i + 1][stringLastIndex] == ';') 
				{ 
					std::string veryTempString = tokenVector[i + 1]; 
					veryTempString.pop_back();  
					commandArgs.push_back(veryTempString); 
					Command* currCommand = new Command(commandString, commandArgs);
					commandVector.push_back(currCommand);  
					foundSemiColon = true;
					//since we break and we still continue parsing, increment i. 
					i++;  
					break; 	
				}

				//Check for '#' that starts a flag. Build command as is and break out.  
				if (tokenVector[i + 1][0] == '#') 
				{
					Command* currCommand = new Command(commandString, commandArgs); 
					commandVector.push_back(currCommand); 
					foundComment = true; 
					break; 	
				}

				//If we found comment during argument retrieval, build command as is and break out. 
				if (tokenVector[i + 1] == "#") 
				{
					Command* currCommand = new Command(commandString, commandArgs); 
					commandVector.push_back(currCommand); 
					foundComment = true; 
					break; 
				}		

				std::string currArg = tokenVector[i + 1];  

				commandArgs.push_back(currArg); 
				i++; 
			}
			//If we found comment during parsing, break out.
			if (foundComment == true) break; 

			//Create command with parsed inputs, add to vector. Only do if we haven't already in previous cases. 
			if (foundSemiColon == false) 
			{
				Command* currCommand = new Command(commandString, commandArgs); 
				commandVector.push_back(currCommand);  
			}
		}
	}





        //Build the tree now. Setup fields
        Base* leftChild = nullptr;
        Base* rightChild = nullptr;
        Base* root = nullptr;
        bool ranTree = false;

        //Only 2 valid cases for correct shell input line. If not true, don't build tree and return error message.
        //2. Number of commands is 1 more than the number of connectors
        //3. number of commands is equal to number of connectors

        bool vectorSizeEqual = (commandVector.size() == connectorVector.size());
        bool commandVectorOneMore = (commandVector.size() - 1 == connectorVector.size());

        //Check for single command with semi colon
        if (commandVector.size() == 1 && connectorVector.size() == 1 && (connectorVector[0] == ";"))
        {
                commandVector[0]->evaluate();
                ranTree = true;
        }


        //Check for single command and no connectors
        if (commandVector.size() == 1 && connectorVector.size() == 0)
        {
                commandVector[0]->evaluate();
                ranTree = true;
        }

        //If neither case is met, we have an error.
        if ((vectorSizeEqual == false) && (commandVectorOneMore == false))
        {
                std::cout << "Error on input. Check you have valid number of connectors/commands." << std::endl;
                break;
        }

		//Build the tree from our 2 vectors
                for (int i = 0; i < connectorVector.size(); i++)
                {
			//If we already ran single command, break. 
                        if (ranTree == true)
			{
				 break;
			}

                        if(i == 0)
                        {
                                leftChild = commandVector[i];
                        }

                        rightChild = commandVector[i + 1];

                        if (connectorVector[i] == ";")
                        {
                                leftChild = new SemiColon(leftChild,rightChild);
                        }

                        if (connectorVector[i] == "||")
                        {
                                leftChild = new Or(leftChild,rightChild);
                        }

                        if (connectorVector[i] == "&&")

                        {
                                leftChild = new And(leftChild,rightChild);
                        }

                root = leftChild;
                }


	//If we have not ran tree and we have a root, run it!
        if (ranTree == false && root != nullptr) root->evaluate();
	

	
}
return 0; 
}
