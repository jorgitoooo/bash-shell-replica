# CS 100 RShell
 
# Project Information: 
  Spring '19
  Ryan Meoni 
  Jorge L. Garcia 
  
# Assignment 4

## Updates

We could not make all the new IO redirection follow the same behavior as our '&&', '||', and ';' connectors. This led to the file descriptors being set for commands that were not intended. We created a solution using the decorator pattern that decorates any commands or other decorators with the desired IO functionality. This ensured that IO functionality corresponded to the correct command. We modified our logic in our infix to postfix function and postifx to expression tree function to track any commands appearing in parentheses. 

The piping IO redirection symbol could not be a standard decorator in our model. This is because it needs two Base pointers in the class. In this case, the piping becomes just like the other connectors we had in assignment 2 and 3. We forked within the new 

We fork() within the new IO redirection classes because we it gives us an efficient way of preserving the file descriptions when necessary. 

The classes section is updated as well to reflect new additions. 

# Diagram:
![umlDiagram](/images/umlDiagram.png)


# Classes:
The parser logic is all handled in our main.cpp file. Once the expression tree is built from the postfix expression, you could call 'evaluate()' on the root node in the expression tree. This will execute all the command specificed with the precedence accounted for. 

The commands class will have a vector that will store any flags that need to be passed. 

The commands and connectors classes will inherit from an abstract base class. This class will have a pure virtual function called "evaluate()", which will return a 1 or a 0 and act similar to the function seen in Lab 3. The commands subclass will use this function to handle the logic of running its respective command. If the command runs successfully with the given flags, the function will return a 1. If unsuccessful, the function will return a 0. 

The connectors subclasses will use the "evaluate()" function in their own way. This function will call "evaluate()" on it's left child, which is the command to the left of the connector. Depending on what connector we have encounted, we will either execute or not execute the right child depending on the return status of the left child. 

Because we have three connectors, we will have three subclasses that inherit from the base class for each connector. The 'exit' command is handled in the Command class because it is a simple check of the command string. 

We have a decorator class that is the parent class to our new IO classes. We have a class for each IO redirection. This means the IO redirection functionality demands five new class additions into our program. Each of these IO redirection classes utilize the fork(), waitpid() and execvp() functions seen previously in our commands class. 

The piping class forks twice due to the left and right elements to the '|' symbol being commands. It is the only IO redirection class that forks twice. The others fork once. 


# Development and Testing Roadmap
  When working on the development of this assignment we came across few issues that we thought we should considered when writing its code. Here is the list of issues we thought we might run into in the future:
  1) First, we need to create our base class representing the shell, since here is where we will reference everything to. 
  2) Once we have our interface created we need to work on the classes that derive from our base/interface class such as: "Command" class and "connector" classes
  3) Our plan on working on these classes is from the bottom down, since we want to provide a solid base for our RShell and then we start deriving each of the tasks. 
  4) We know that as we build our RShell we will run into certain issues that might be caused by user input such as, optimally the user is supposed to write the connectors in the following  format: "||" or "&&" or ";", but what if they happened to enter a typo and our program wont know what to do with that erroneous input. For this reason we have to set some time to do some debugging and essentially consider all possible issues that might cause our RShell to crash or output data that's unwanted. 
  5) Due to some of the issues that we might run into, we will have to create functions that will act as a safeguard to prevent the user from entering data that won't be recognized by the program. 
  6) Note that as we progress through our RShell we must test each individual change that has been attached to the program. So, if we add any new functions and/or classes to the program we must make sure that it runs properly and outputs the correct data.
  7) Now that we have tested the functions individually and we have covered the possible roadblocks, we can now test the program as a whole by integrating the functions and classes as one to make sure that it works properly.
  8) Since we have now tested all the functions and classes individually and as a group, we should have by now tested that our program runs properly and we further test erroneous inputs to see how it behaves. By doing this we will make sure our program will efficiently take any input and respond depending on what the user wants it to do even when entering unwanted inputs.
