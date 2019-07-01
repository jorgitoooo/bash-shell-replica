#include "gtest/gtest.h"
#include "../src/command.h" 
#include "../src/and.h" 
#include "../src/or.h" 
#include "../src/semiColon.h" 
#include "../src/decorator.h" 
#include "../src/input.h" 
#include "../src/outputAppend.h" 
#include "../src/outputOverride.h" 
#include "../src/pipe.h" 


//For commands, successful execution through the 'evaluate()' function will return true. Failure returns false. 
//For connectors, 'evaluate()' will return true if the connector condition is satisfied and the command to right had execution succeed. 
//Otherwise, connector's 'evaluate()' will return false. 

//ASSIGNMENT 4 UNIT TESTS BELOW. Testing to make sure that the new decorator classes decorate commands and other decorators successfully. 
TEST(DecoratorTest, checkFunctionality) {

	//*****IMPORTANT! For the > and >> tests below, check the repository root directory to see the created files 

	//Test the '>>' decorator 	
	std::string commandString = "echo"; 
	std::vector<std::string> commandArgs; 
	commandArgs.push_back("appended"); 
	commandArgs.push_back("to"); 
	commandArgs.push_back("file!"); 
	Command* childCommand = new Command(commandString, commandArgs); 
	OutputAppend* outputAppend = new OutputAppend(childCommand); 
	outputAppend->rightPath = "appendFile.txt"; 
	//Run decorator's evaluate() function
	EXPECT_EQ(true, outputAppend->evaluate()); 

	
	//Test the '>' decorator 
	commandArgs.clear(); 
	commandString = "echo"; 
	commandArgs.push_back("overriding");
	commandArgs.push_back("this"); 
	commandArgs.push_back("file's"); 
	commandArgs.push_back("content!"); 
	childCommand = new Command(commandString, commandArgs);
	OutputOverride* outputOverride = new OutputOverride(childCommand); 
	outputOverride->rightPath = "overridedFile.txt"; 
	EXPECT_EQ(true, outputOverride->evaluate());   

	//Test the '<' decorator 
	commandArgs.clear(); 
	commandString = "cat"; 
	childCommand = new Command(commandString, commandArgs); 
	Input* input = new Input(childCommand); 	
	input->rightPath = "CMakeLists.txt"; 
	//*****IMPORTANT! For the < test below, check that the output to console on ./test is the CMakeLists.txt content
	EXPECT_EQ(true, input->evaluate()); 

	//Test the '|' decorator 
	std::string commandOneString = "ls"; 
	std::string commandTwoString = "more"; 
	std::vector<std::string> commandOneArgs; 
	std::vector<std::string> commandTwoArgs; 
	commandOneArgs.push_back("-a"); 
	Command* commandOne = new Command(commandOneString, commandOneArgs); 
	Command* commandTwo = new Command(commandTwoString, commandTwoArgs); 
	Pipe* pipe = new Pipe(commandOne, commandTwo); 
	//Run the decorator's evaluate() function
	EXPECT_EQ(true, pipe->evaluate()); 

	//Test that the decorators work when stacked on top of one another
	//In this case, one '>>' decorator is stacked on top of another '>>' decorator that has a command as child. 
	commandArgs.clear(); 
	commandOneString = "echo"; 
	commandArgs.push_back("stacked"); 
	commandArgs.push_back("output"); 
	childCommand = new Command(commandOneString, commandArgs); 
	OutputAppend* appendOne = new OutputAppend(childCommand);
	appendOne->rightPath = "stacked.txt"; 
	OutputAppend* appendTwo = new OutputAppend(appendOne); 
	appendTwo->rightPath = "stacked.txt"; 
	EXPECT_EQ(true, appendOne->evaluate()); 
}


//ASSIGNMENT 3 UNIT TESTS BELOW. Only Addition is testing the 'test' command functionality inside the Command class. 
//Only tests relative paths due to difference once the directory is cloned. Works with full paths. 
TEST(TestCommandTest, CheckFunctionality) {
	std::string testCommandString = "test"; 

	//Test that the 'e' flag testcommand returns true when path is valid 
	std::vector<std::string> testCommandArgs; 
	testCommandArgs.push_back("-e"); 
	testCommandArgs.push_back("./CMakeLists.txt"); 
	Command* testCommand = new Command(testCommandString, testCommandArgs); 
	EXPECT_EQ(true, testCommand->evaluate()); 
	testCommandArgs.clear(); 
	
	//Test that the 'e' flag test command returns false when the path is invalid 
	testCommandArgs.push_back("-e"); 
	testCommandArgs.push_back("./DoesNotExist.txt"); 
	testCommand = new Command(testCommandString, testCommandArgs); 
	EXPECT_EQ(false, testCommand->evaluate()); 
	testCommandArgs.clear(); 

	//Test that the 'f' flag test command returns true when the path is valid and file exists
	testCommandArgs.push_back("-f"); 
	testCommandArgs.push_back("./CMakeLists.txt"); 
	testCommand = new Command(testCommandString, testCommandArgs); 
	EXPECT_EQ(true, testCommand->evaluate()); 
	testCommandArgs.clear(); 
	
	//Test that the 'f' flag test command returns false when the path is invalid (file does not exist)
	testCommandArgs.push_back("-f");
        testCommandArgs.push_back("./lib");
        testCommand = new Command(testCommandString, testCommandArgs);
        EXPECT_EQ(false, testCommand->evaluate());
        testCommandArgs.clear();

	//Test that the 'd' flag test command returns true when the path is valid and directory exists
	testCommandArgs.push_back("-d");
        testCommandArgs.push_back("./lib");
        testCommand = new Command(testCommandString, testCommandArgs);
        EXPECT_EQ(true, testCommand->evaluate());
        testCommandArgs.clear();

	
	//Test that the 'd' flag test command returns false when the path is invalid (directory does not exist)
	testCommandArgs.push_back("-d");
        testCommandArgs.push_back("./CMakeLists.txt");
        testCommand = new Command(testCommandString, testCommandArgs);
        EXPECT_EQ(false, testCommand->evaluate());
        testCommandArgs.clear();

}


//ASSIGNMENT 2 UNIT TESTS BELOW
TEST(CommandTest, CheckFunctionality) {

        //Check return of 'evaluate()' function for VALID command WITH NO flags.
        //Command under test is 'ls'.
        std::string commandString = "ls";
        std::vector<std::string> commandArgs;
        Command* commandOne = new Command(commandString, commandArgs);
        //Run test
        EXPECT_EQ(true, commandOne->evaluate());

        //##########

        //Check return of 'evaluate()' for VALID command WITH VALID flags.
        //Command under test is 'ls -a -l'.
        commandArgs.push_back("-a");
        commandArgs.push_back("-l");
        Command* commandTwo = new Command(commandString, commandArgs);
        //Run test
        EXPECT_EQ(true, commandTwo->evaluate());

        //clear arg vector
        commandArgs.clear();

        //##########

        //Check return of 'evaluate()' function for INVALID command WITH NO flags.
        //Command under test is 'pineapple'.
        commandString = "pineapple";
        Command* commandThree = new Command(commandString, commandArgs);
        //Run test
        EXPECT_EQ(false, commandThree->evaluate());

        //##########

        //Check return of 'evaluate()' function for VALID command with INVALID flags.
        //Command under test is 'ls -pineapple'.
        commandArgs.push_back("-pineapple");
        Command* commandFour = new Command(commandString, commandArgs);
        //Run test
        EXPECT_EQ(false, commandFour->evaluate());
}

TEST(ConnectorTest, CheckFunctionality) {

        //Check if '&&' connector returns true when both commands succeed.
        //Input built under test is 'echo hello world && ls -a'

        //build first command and args to it
        std::string commandOneString = "echo";
        std::vector<std::string> commandOneArgs;
        commandOneArgs.push_back("hello");
        commandOneArgs.push_back("world");
        Command* commandOne = new Command(commandOneString, commandOneArgs);

        //build second command and args to it
        std::string commandTwoString = "ls";
        std::vector<std::string> commandTwoArgs;
        commandTwoArgs.push_back("-a");
        Command* commandTwo = new Command(commandTwoString, commandTwoArgs);

        //build '&&' connector
        And* andConnector = new And(commandOne, commandTwo);
        //Run test
        EXPECT_EQ(true, andConnector->evaluate());

        //##########

        //Check if the '&&' connector returns false when 1 command fails.
        //Input built under test is 'ls -pineapple && ls -a'

        //clear vectors from previous test
        commandOneArgs.clear();
        commandTwoArgs.clear();

        //build first command and arg to it
        commandOneString = "ls";
        commandOneArgs.push_back("-pineapple");
        commandOne = new Command(commandOneString, commandOneArgs);

        //second command stays the same

        //build '&&' connector
        andConnector = new And(commandOne, commandTwo);
        //Run test
        EXPECT_EQ(false, andConnector->evaluate());

        //##########

        //Check if the '||' connector returns true when one command succeeds.
        //Input built under test is 'ls -pineapple || ls -a'

        //Use the same commands and arg vectors from previous test.

        //build '||' connector
        Or* orConnector = new Or(commandOne, commandTwo);
        //Run test
        EXPECT_EQ(true, orConnector->evaluate());

        //##########

        //Check if the '||' connector returns true when both commands succeed.
        //Input built under test is 'echo hello world || ls -a'

        //clear only first command's arg vector, since not changing second command/args.
        commandOneArgs.clear();

        //build first command and args to it
        commandOneString = "echo";
        commandOneArgs.push_back("hello");
        commandOneArgs.push_back("world");
        commandOne = new Command(commandOneString, commandOneArgs);

        //build '||' connector. Second command stays the same
        orConnector = new Or(commandOne, commandTwo);
        //Run test
        EXPECT_EQ(true, orConnector->evaluate());

        //##########

        //Check if the ';' connector works as expected. Its 'evaluate()' should return right child command's execution status
        //regardless of whether left child succeeds or fails. If no right child (when the ';' signals no more commands in input),
        //then returns execution status of left child.

        //Input built under test is 'echo hello world; ls -a'

        //Use same commands from previous test

        //build ';' connector.
        SemiColon* semiColonConnector = new SemiColon(commandOne, commandTwo);
        //Run test. Should return true because right command succeeds.
        EXPECT_EQ(true, semiColonConnector->evaluate());

        //##########

        //Check if the ';' connector works as expected with no right child.

        //build ';' connector
        semiColonConnector = new SemiColon(commandOne, NULL);
        //Run test. Should return true because left command succeeds.
        EXPECT_EQ(true, semiColonConnector->evaluate());

        //##########

        //Check that return is false when left command fails.

        //build command that fails
        commandOneString = "fakeCommand";
        commandOne = new Command(commandOneString, commandOneArgs);

        //build ';' connector
        semiColonConnector = new SemiColon(commandOne, NULL);
        EXPECT_EQ(false, semiColonConnector->evaluate());

}

TEST(inputLineToTree, CheckFunctionality) {

	//Build a small tree of multiple connectors 
	//Input line is: ls -a; echo hello world || exit && echo this will run 
	
	std::string commandOneString ="ls"; 
	std::vector<std::string> commandOneVec;  
	commandOneVec.push_back("-a");

	std::string commandTwoString = "echo"; 
	std::vector<std::string> commandTwoVec; 
	commandTwoVec.push_back("hello"); 
	commandTwoVec.push_back("world"); 

	std::string commandThreeString = "exit"; 
	std::vector<std::string> commandThreeVec; 
	
	std::string commandFourString = "echo"; 
	std::vector<std::string> commandFourVec; 
	commandFourVec.push_back("this"); 
	commandFourVec.push_back("will"); 
	commandFourVec.push_back("run"); 

	Command* commandOne = new Command(commandOneString, commandOneVec); 
	Command* commandTwo = new Command(commandTwoString, commandTwoVec); 
	Command* commandThree = new Command(commandThreeString, commandThreeVec); 
	Command* commandFour = new Command(commandFourString, commandFourVec); 

	SemiColon* semiColon = new SemiColon(commandOne, commandTwo); 
	Or* orCon = new Or(semiColon, commandThree); 
	And* andCon = new And(orCon, commandFour); 

	
	//And connector is the root now
	//We expect true, and we can see the 'echo this will run' command runs. exit never runs, adhering to connector rules. 
	EXPECT_EQ(true, andCon->evaluate()); 

}



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
