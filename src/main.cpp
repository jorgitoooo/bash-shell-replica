#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>
#include <vector> 
#include <stack> 
#include "command.h" 
#include "and.h" 
#include "or.h" 
#include "semiColon.h" 
#include "input.h" 
#include "outputOverride.h" 
#include "outputAppend.h" 
#include "pipe.h" 



//COMMENT SIGNIFYING TAG COMMIT

//Add type definition of boost tokenizer 
typedef boost::tokenizer<boost::char_separator<char>> tokenizer; 

std::vector<int> paren_flags;

std::vector<std::string> inputToInfix(std::string str)
{

        //parse string based on quotes, '(', ')', ';', '[', and ']' to get strings of continuous commands/connectors/parentheses
        boost::char_separator<char> sep("", "();\"[]");
        tokenizer tokens(str, sep);

        //Put elements separated by specified delimeters from iterator into vector of strings
        std::vector<std::string> stringVec;

        for (tokenizer::iterator iterator = tokens.begin(); iterator != tokens.end(); iterator++)
        {
                stringVec.push_back(*iterator);
        }

        //Get individual tokens separated by white space 
        std::vector<std::string> tokenVec;
        for (int i = 0; i < stringVec.size(); i++)
        {
                std::string currString = stringVec[i];

                boost::char_separator<char> sep(" ");
                tokenizer tokens(currString, sep);
                for (tokenizer::iterator iterator = tokens.begin(); iterator != tokens.end(); iterator++)
                {
                        tokenVec.push_back(*iterator);
                }
        }


        //Make final vector. Stop adding elements when a "#" appears as a single string or starts a string.
        std::vector<std::string> finalVec;
        for (int i = 0; i < tokenVec.size(); i++)
        {
                if (tokenVec[i] == "#" || tokenVec[i][0] == '#') break;

                finalVec.push_back(tokenVec[i]);
        }

        //Make infix Vector from this
        std::vector<std::string> infixVec;

        for (int i = 0; i < finalVec.size(); i++)
        {
                //If single parentheses, push to infix Vector
                if (finalVec[i] == "(")
                {
                        infixVec.push_back("(");
                }
                else if (finalVec[i] == ")")
                {
                        infixVec.push_back(")");
                }
                //If a connector, push to infix Vector
                else if (finalVec[i] == ";")
                {
                        infixVec.push_back(";");
                }
                else if (finalVec[i] == "&&")
                {
                        infixVec.push_back("&&");
                }
                else if (finalVec[i] == "||")
                {
                        infixVec.push_back("||");
                }

		else if (finalVec[i] == "<") 
		{
			infixVec.push_back("<"); 
		}

		else if (finalVec[i] == ">")
		{
			infixVec.push_back(">"); 
		}

		else if (finalVec[i] == ">>")
		{
			infixVec.push_back(">>"); 
		}

		else if (finalVec[i] == "|")
		{
			infixVec.push_back("|"); 
		}

		//Check for opening bracket "[" or "test" to signify test command.
                else if (finalVec[i] == "[" || finalVec[i] == "test")
                {

                	//Use boolean to see if openning bracket. Need to skip over closing bracket later if true.
                	bool openingBracket;
                	if (finalVec[i] == "[") openingBracket = true;
                	else openingBracket = false;

                	std::string testCommandWithFlags = "test";

                        //Move to next element to check for specified flag. If not specified, use default -e.
                        i++;

                        //If flag type was specified, grab it. Then grab path.
                        if (finalVec[i] == "-e" || finalVec[i] == "-f" || finalVec[i] == "-d")
                        {

                                testCommandWithFlags = testCommandWithFlags + " " + finalVec[i];
                                //Move to next element (will be path if previous flag was specified)
                                i++;
                                testCommandWithFlags = testCommandWithFlags + " " + finalVec[i];

                        }

                        //Default flag "-e" needed. Index i has path instead of flag.
                        else {
                                testCommandWithFlags = testCommandWithFlags + " " + "-e";
                                testCommandWithFlags = testCommandWithFlags + " " + finalVec[i];
                        }

                        infixVec.push_back(testCommandWithFlags);

                //Skip closing bracket if needed.
                if (openingBracket == true) i++;
                }



                //Must be the a command at this index. Create a single string with command and flags.
                else {
                        bool foundClosingQuote = false;

                        //initially just command string
                        std::string commandWithFlags = finalVec[i];
                        i++;

                        ///could be last command with no flags/connectors/parentheses after. Check.
                        if ((i < finalVec.size() == false))
                        {
                                infixVec.push_back(commandWithFlags);
                                break;
                        }

                        //check for opening quote. simply skip over it.
                        if (finalVec[i] == "\"")
                        {
                                i++;
                        }

                        //keep looping grabbing flags until the the next element
                        //is a connector, parentheses, or end of finalVec.
                        while ((i < finalVec.size()) && (finalVec[i] != ")") && (finalVec[i] != "(") && (finalVec[i] != "||")
                        && (finalVec[i] != "&&") && (finalVec[i] != ";") && (finalVec[i] != ">") && (finalVec[i] != ">>")
			&& (finalVec[i] != "<") && (finalVec[i] != "|"))
                        {
                                //check for closing quote.
                                if (finalVec[i] == "\"")
                                {
                                        foundClosingQuote = true;
                                        break;
                                }

                                //concatenate our commandWithFlags string with the next flag
                                commandWithFlags = commandWithFlags + " " + finalVec[i];
                                i++;
                        }

                infixVec.push_back(commandWithFlags);
                //decrement i if we reached here and didn't see closing quote. Prevents skipping an element.

                if (foundClosingQuote == false) i--;
                }
        }
        return infixVec;
}


//Function needed for precedence 
int precedence(std::string currOperator)
{

	//All connectors have the same precedence
	if (currOperator == "&&" || currOperator == "||" || currOperator == ";" || currOperator == "<"
	|| currOperator == ">" || currOperator == ">>" || currOperator == "|")
	{
		return 1; 
	}
	else {
	return -1; 
	}
}

//Function for infix to postfix conversion 
std::vector<std::string> infixToPostfix(std::vector<std::string> infixVector) 
{
	std::stack<std::string> stack; 
	std::vector<std::string> postfixVector; 

	for (int i = 0; i < infixVector.size(); i++)
        {

                std::string currElement = infixVector[i];


                //Check if current element is connector.
                if ((currElement == "&&") || (currElement == "||") || (currElement == ";")
                || (currElement== ">") || (currElement == ">>") || (currElement == "<") || (currElement == "|"))
                {
                        //Keep popping off stack while precedence is less than or equal to the top

                        if (stack.size() != 0)
                        {
                                while (precedence(currElement) <= precedence(stack.top()) && (stack.size() != 0))
                                {
                                        std::string currStackTop = stack.top();
                                        postfixVector.push_back(currStackTop);
                                        stack.pop();

                                        if (stack.size() == 0) break;
                                }
                        }

                //Push our currElement
                stack.push(currElement);
                }

                //Closing parentheses. Pop elements from stack until closing parentheses
                else if (currElement == ")")
                {
                        if (stack.size() == 0)
                        {
                                std::cout << "Error involving closing parentheses in infix to postfix" << std::endl;
                        }
                        else {
                                while ((stack.top() != "(") && (stack.size() != 0))
                                {
                                std::string currStackTop = stack.top();
                                postfixVector.push_back(currStackTop);
                                stack.pop();
                                }
//*****************
				paren_flags.push_back(postfixVector.size() - 1);
//********************
                        }

                        if (stack.size() == 0)
                        {
                                std::cout << "Error involving closing parentheses in infix to postfix" << std::endl;
                        }

                        //Remove closing parentheses
                        stack.pop();
                }

                //Push opening parentheses
                else if (currElement == "(")
                {
                        stack.push(currElement);
                }

                //Else we have command
                else {
                        postfixVector.push_back(currElement);
                }
        }

        //Put all remaining elements into postfix vector
        if (stack.size() != 0)
        {
                for (std::string currStackTop = stack.top(); stack.size() != 0; stack.pop())
                {
                        if (currStackTop == ")" || currStackTop == "(")
                        {
                                std::cout << "Error involving putting remaining stack elements to postfix" << std::endl;
                        }

                        postfixVector.push_back(currStackTop);
                }
        }


	return postfixVector; 
}


//Used to parse command with flags string. Splits string by whitespace, and makes each separate string an element in vector. 
//Example input: "ls -a -l"
//Example output: vector<string> = {"ls", "-a", "-l"} 
std::vector<std::string> parseCommandWithFlags(std::string commandWithFlagsString) 
{
	std::vector<std::string> commandWithFlagsVector; 

        boost::char_separator<char> sep(" ");
        tokenizer tokens(commandWithFlagsString, sep);
        for (tokenizer::iterator iterator = tokens.begin(); iterator != tokens.end(); iterator++)
        {
                commandWithFlagsVector.push_back(*iterator);
        }
	
	return commandWithFlagsVector; 

}

//Create command from command vector made from parseCommandWithFlags() function
Command* createCommand(std::vector<std::string> commandVector) 
{
	//Vector for command's flags (if any)
	std::vector<std::string> commandFlagVector; 

	//Command will be first string in vector
	std::string commandString = commandVector[0]; 

	//Get any flags for the command. 
	for (int i = 1; i < commandVector.size(); i++) 
	{
		commandFlagVector.push_back(commandVector[i]); 
	}

	//Make command, return it
	Command* command = new Command(commandString, commandFlagVector); 
	return command; 
}

Base* postfixToExpressionTree(std::vector<std::string> postfixVector) 
{
	//Check for single command string with no connectors after
	if (postfixVector.size() == 1)
	{
		//Pass the zero index element, only element in vector  
		std::vector<std::string> singleCommandVector = parseCommandWithFlags(postfixVector[0]); 
		Command* singleCommand = createCommand(singleCommandVector); 
		return singleCommand; 
	}

	//Check for single command string with exactly 1 connector (would indicate command followed by semiColon)
	else if (postfixVector.size() == 2)
	{
		std::vector<std::string> commandVector = parseCommandWithFlags(postfixVector[0]); 
		
		//Double check that the element at index 1 is a semi-colon 
		if (postfixVector[1] != ";") 
		{
			std::cout << "Error on single command with 1 connector" << std::endl;
			exit(EXIT_FAILURE); 		
		}
		
		Command* singleCommand = createCommand(commandVector); 
		
		//Logic in SemiColon class handles right child being nullptr
		SemiColon* semiColon = new SemiColon(singleCommand, nullptr); 
		return semiColon; 
 
	}
	
	//Standard postfix to expression tree conversion 
	else {
		std::stack<Base*> stack; 

		//Scan postfix expression. If we see command, push it to stack. If we see connector, pop two elements off stack (could be a  
		//command or composite connector with two children already). Make this new connector's right and left child the two popped elements. 
		for (int i = 0; i < postfixVector.size(); i++) 
		{
		
        		if (postfixVector[i] == "&&")
        		{
            			Base* rightChild = stack.top();
				stack.pop(); 
            			Base* leftChild = stack.top();
				stack.pop(); 
				
				And* andConnector = new And(leftChild, rightChild); 

            			stack.push(andConnector); 
       			}

			else if (postfixVector[i] == "||")
        		{
            			Base* rightChild = stack.top();
				stack.pop(); 
            			Base* leftChild = stack.top();
				stack.pop(); 
				
				Or* orConnector = new Or(leftChild, rightChild); 

            			stack.push(orConnector); 
       			}

			else if (postfixVector[i] == "<") 
			{
				Base* rightChild = stack.top(); 
				stack.pop(); 
				Base* leftChild = stack.top(); 
				stack.pop(); 

				//Right child is command still, has the path. Get it. 
				std::string path = rightChild->stringify(); 

				//Left child could be command or connector. 
				Base* possibleCommand = leftChild; 

				//If possibleCommand is already a command.
				if (possibleCommand->getClassType() == "command" || possibleCommand->getClassType() == "decorator") 
				{
					Input* input = new Input(possibleCommand);
					input->rightPath = path;  
					stack.push(input); 
				}

				//Command to decorate is right child of the left child Base* connector we popped off stack.
				else {

					Base* parent = possibleCommand; 
					Base* decoratorChild; 

					bool is_within_paren = false;

					for (int j = paren_flags.size() - 1; j >= 0; j--)
					{
						if (paren_flags[j] == (i - 2)) is_within_paren = true;
					}

					decoratorChild = is_within_paren ? parent : parent->rightChild;

					Input* in = new Input(decoratorChild); 
					in->rightPath = path; 

					if (is_within_paren)
					{
						stack.push(in); 
					}
					else
					{
						parent->rightChild = in;
						stack.push(parent);
					}
				}
			}

			else if (postfixVector[i] == ">") 
			{
				Base* rightChild = stack.top(); 
				stack.pop(); 
				Base* leftChild = stack.top(); 
				stack.pop(); 

				//Right child is command still, has the path. Get it. 
				std::string path = rightChild->stringify(); 

				//Left child could be command or connector. 
				Base* possibleCommand = leftChild; 

				//If possibleCommand is already a command.
				if (possibleCommand->getClassType() == "command" || possibleCommand->getClassType() == "decorator") 
				{
					OutputOverride* outputOverride = new OutputOverride(possibleCommand);
					outputOverride->rightPath = path;  
					stack.push(outputOverride); 
				}

				//Command to decorate is right child of the left child Base* connector we popped off stack.
				else {

					Base* parent = possibleCommand; 
					Base* decoratorChild; 

					bool is_within_paren = false;

					for (int j = paren_flags.size() - 1; j >= 0; j--)
					{
						if (paren_flags[j] == (i - 2)) is_within_paren = true;
					}

					decoratorChild = is_within_paren ? parent : parent->rightChild;

					OutputOverride* outOver = new OutputOverride(decoratorChild); 
					outOver->rightPath = path; 

					if (is_within_paren)
					{
						stack.push(outOver); 
					}
					else
					{
						parent->rightChild = outOver;
						stack.push(parent);
					}
				}
			}

			else if (postfixVector[i] == ">>")
                        {
                                Base* rightChild = stack.top();
                                stack.pop();
                                Base* leftChild = stack.top();
                                stack.pop();
				
				//Right child is command still, has the path. Get it. 
				std::string path = rightChild->stringify(); 

				//Left child could be command or connector. 
				Base* possibleCommand = leftChild; 

				//If possibleCommand is already a command
				if (possibleCommand->getClassType() == "command" || possibleCommand->getClassType() == "decorator") 
				{
					OutputAppend* outputAppend = new OutputAppend(possibleCommand);
					outputAppend->rightPath = path;  
					stack.push(outputAppend); 
				}
				
				//Command to decorate is right child of the left child Base* connector we popped off stack.
				else {

					Base* parent = possibleCommand; 
					Base* decoratorChild; 

					bool is_within_paren = false;

					for (int j = paren_flags.size() - 1; j >= 0; j--)
					{
						if (paren_flags[j] == (i - 2)) is_within_paren = true;
					}

					decoratorChild = is_within_paren ? parent : parent->rightChild;

					OutputAppend* outputAppend = new OutputAppend(decoratorChild); 
					outputAppend->rightPath = path; 

					if (is_within_paren)
					{
						stack.push(outputAppend); 
					}
					else
					{
						parent->rightChild = outputAppend;
						stack.push(parent);
					}
				}
			}

			else if (postfixVector[i] == "|")
                        {
                                Base* rightChild = stack.top();
                                stack.pop();
                                Base* leftChild = stack.top();
                                stack.pop();

				//Left child could be command or connector. 
				Base* possibleCommand = leftChild; 

				//If possibleCommand is already a command
				if (possibleCommand->getClassType() == "command" || possibleCommand->getClassType() == "decorator") 
				{
					Pipe* pipe = new Pipe(possibleCommand, rightChild);
					stack.push(pipe); 
				}
				
				//Command to decorate is right child of the left child Base* connector we popped off stack.
				else
				{

					Base* parent = possibleCommand; 
					Base* decoratorChild; 

					bool is_within_paren = false;

					for (int j = paren_flags.size() - 1; j >= 0; j--)
					{
						if (paren_flags[j] == (i - 2)) is_within_paren = true;
					}

					decoratorChild = is_within_paren ? parent : parent->rightChild;

					Pipe* pipe = new Pipe(decoratorChild, rightChild); 

					if (is_within_paren)
					{
						stack.push(pipe); 
					}
					else
					{
						parent->rightChild = pipe;
						stack.push(parent);
					}
				}
                        }

			//Semicolons can be weird case since they might have just 1 child.  
			else if (postfixVector[i] == ";")
        		{
				Base* rightChild = NULL; 
				Base* leftChild = NULL; 

				
				if (stack.size() > 0)
				{				
            			rightChild = stack.top();
				stack.pop(); 
				}
				if (stack.size() > 0)
				{
            			leftChild = stack.top();
				stack.pop(); 
				}

				SemiColon* semiColonConnector = new SemiColon(leftChild, rightChild); 

            			stack.push(semiColonConnector); 
       			}

			//else we found command. Create it and push to stack. 
        		else {
            			std::vector<std::string> commandVector = parseCommandWithFlags(postfixVector[i]); 
				Command* singleCommand = createCommand(commandVector); 
				stack.push(singleCommand); 
			}
		}
	
		//Top of stack at end of postfix expression to expression tree building should be our root. 
		Base* root = stack.top(); 
		return root; 

	}
	return nullptr; 
}


int main()
{

        while (1)
        {
		std::cout << "$ ";  

                //get input line from user
                std::string str;
                std::getline(std::cin, str);

                std::vector<std::string> infixVector = inputToInfix(str);

		//Check that user entered something at terminal. Only proceed if so. 
		if (infixVector.size() > 0) 
		{


			//Remove ending semi-colon if found. Blows up the program if not removed :( 
			if (infixVector[infixVector.size() - 1] == ";") 
			{
				infixVector.pop_back(); 
			}
		
			//Test infix vector
/*			std::cout << "\n\n#### Infix vector below ####\n\n" << std::endl; 
                	for (int i = 0; i < infixVector.size(); i++)
                	{
                        	std::cout << infixVector[i] << std::endl;
                	}
		
			std::cout << "\n\n#### Postfix vector below ####\n\n" << std::endl; 
*/
			std::vector<std::string> postfixVector; 

			postfixVector = infixToPostfix(infixVector); 

//			for (int i = 0; i < postfixVector.size(); i++) 
//			{
//				std::cout << postfixVector[i] << std::endl; 
//			}

			//Get root of expression tree	
			Base* root = postfixToExpressionTree(postfixVector); 
			if (root == nullptr) 
			{
				std::cout << "root is null" << std::endl; 
				exit(EXIT_FAILURE); 
			}

//			std::cout << "\n\n#### calling stringify() on root now ####\n\n" << std::endl; 

//			std::cout << root->stringify() << std::endl; 
		
//			std::cout << "\n\n#### call of evaulate() on root would be below (not called yet) ####\n\n" << std::endl;
			root->evaluate(); 
			paren_flags.clear();
		}
        }
}
