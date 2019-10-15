# Bash Shell Clone 
## Developed by: Jorge L. Garcia & Ryan Meoni
#

# Introduction

This Bash Shell Clone is an interactive shell where the user inputs their command(s) and our program runs a series of parsing functions on the input which will create a composite object tree from that line of command(s). Once the tree is created the commands will be forked and ran in child processes. This shell has been implemented using the composite design pattern and the decorator pattern.


# Diagram

![Composite UML](/images/composite_uml.png)

# Classes

**Line** :
 * This will be the abstract class that all classes must implement. All implementations of ```Execute()``` will return a 1 upon successful execution and 0 otherwise. All implementations of ```Exit()``` will deal with system exit calls.

**Command** :
 * This class will contain the command that was used along with a vector containing the arguments that were passed to that command. The execute function will be the only implementation of ```Execute()``` which will actually create a child process to execute the command. ```Execute()``` will return a 1 upon successful completion and 0 upon failure. ```Exit()``` will safely terminate all running processes.

**Connector** :
 * This will be another abstract class which all of the control operators will implement. The control operators will have the capacity to hold a collection of commands and/or one or more control operators.
 * Control operator classes which will implement the Connector abstract class are:
   * *And_Op*
   * *Or_Op*
   * *Semicol_Op*

**And_Op** :
 * This class will contain a collection of Command and/or Connector objects and will represent this collection as private members ```line_1``` & ```line_2```. The execute function will run both ``` line_1->Execute()``` and ``` line_1->Execute()``` regardless of their corresponding return value. The class’ execute function will return a 1 if and only if both line_1 and line_2 executions succeed. The exit function will call all of the children’s exit functions to safely terminate any running processes.

**Or_Op** :
 * This class will contain a collection of Command and/or Connector objects and will represent this collection as private members ```line_1``` & ```line_2```. The execute function will only run ```line_2->Execute()``` if ```line_1->Execute()``` fails (returns a 0). This class’ execute function will return a 1 if either ```line_1->Execute()``` or ```line_2->Execute()``` succeed. The exit function will call all of the children’s exit functions to safely terminate any running processes.

**Semicol_Op**
 * This class will contain a single Command and will represent it as private member ```line```. The execute function will run ```line->Execute()``` and will return the ```line->Execute()``` return value (1 upon completion and 0 otherwise). The exit function will call its child's exit function to safely terminate any running processes.

# Prototypes/Research
```c++
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>

int main(int argc, char **argv) {

        pid_t pid = fork();

        if (pid == 0) {
                printf("Child process id: %d\n", pid);
                printf("Child: Running execvp()\n");

                if (execvp(argv[1], (argv + 1)) == -1) {
                // Pointer arithmetic used to shift argv pointer to actual
                //   command that was passed rather than this programs's executable
                        
                        printf("\n\nError in child process\n\n");
                        perror(argv[1]);
                }
                std::cout << "\nChild: Process finished.\n\n"; // Anything beyond execvp will not run
        }
        if (pid > 0) {
                printf("Parent process id: %d\n", pid);
                printf("Parent: Waiting for child process to finish.\n");

                pid_t pid_wait = waitpid(0, NULL, 0);

                printf("pid_wait = %d\n\n", pid_wait);
                if (pid_wait == -1) {
                        perror("Parent");
                } else {
                        std::cout <<"\nParent: Child process completed\n\n";
                }

        }
        return 0;
}

```
**Steps Taken**:

  * **Compile**: ``` g++ -std=c++11 few_prototype.cpp -o few_prototype ```

  * **Run 1**: ``` ./few_prototype ls -l && echo prototype```

  * **Output 1**: 
  ``` 
Parent process id: 9188                                                         
Parent: Waiting for child process to finish.                                    
Child process id: 0                                                             
Child: Running execvp()                                                         
total 40                                                                        
-rwxr-xr-x 1 jgarc271 csmajs  9256 Apr 27 13:03 few_prototype                   
-rw-r--r-- 1 jgarc271 csmajs   947 Apr 26 17:34 few_prototype.cpp               
pid_wait = 9188                                                                 
                                                                                
                                                                                
Parent: Child process completed                                                 
                                                                                
prototype
```
  * **Run 2**: ```./few_prototype la```

  * **Output 2**: 
``` 
Parent process id: 12066                                                        
Parent: Waiting for child process to finish.                                    
Child process id: 0                                                             
Child: Running execvp()                                                         
                                                                                
                                                                                
Error in child process                                                         
                                                                                
la: No such file or directory                                                   
                                                                                
Child: Process finished.                                                        
                                                                                
pid_wait = 12066                                                                
                                                                                
                                                                                
Parent: Child process completed                                                
```
* **Findings**:

    * The waitpid function returns before the execvp finishes outputting its results to the standard output as seen in *Output 1*.

      * **Use Case**: We can have the parent process perform other functionality while the child is done running but hasn’t outputted to the standard output.
    * The execvp function returns a -1 upon an unsuccessful command as seen in *Output 2*.
      * **Use Case**: We can implement any necessary logic within an if statement that checks for this value and performs any extra functionality that we may want to implement upon failure of a command.
    * If the execvp function runs successfully, it terminates the child process therefore preventing the execution of any code that proceeds it.
      * **Use Case**: Like in our previous finding (b), we can implement extra functionality. For example, after the execvp call, we may decide to prompt the user and ask if they would like to modify and rerun that specific command instead of having them input an entire command, or multiple commands in the case of the usage of connectors.
  

# Development and Testing Roadmap

* **Development & Testing**:
  1. Create the component base class ```Line```.
  2. Create the leaf class ```Command```.
     * **Test**: Run unit tests on this class once completed to test that commands and arguments can be passed correctly and that child processes are created and terminated appropriately.
  3. Create the abstract composite class ```Connector```.
  4. Create all of the composite classes ```And_Op```, ```Or_Op```, and ```Semicol_Op```.
     * **Test**: Run unit tests on every composite class once completed to test that all of the logic runs correctly. We will also run integration tests to make sure that the classes properly integrate with ```Command``` classes and/or other ```Connector``` classes.
  5. Create the parsing functions that will parse the user's input and create the object trees.
     * **Test**: Run all of the necessary unit and integration tests to verify that trees are being properly formed and the interaction between these functions and the composite objects is as it should be.
  6. Create the main function which will integrate all of the components of this project.
     * **Test**: Run an extensive set of integration tests to ensure that everything is working as it should be.
