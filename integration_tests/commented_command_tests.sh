#!/bin/sh

#Brian Crites said that it is fine to have the 'exit' command at the end of commands to test execution for integration tests.
#PLEASE NOTE: The exit has to preempt the comment each time. This is due to an infinite loop being generated without using exit in these tests.
#THIS DOES NOT HAPPEN IF YOU RUN ./rshell IN THE ROOT FOLDER. THIS HAPPENS ONLY WHEN USING BASH SCRIPTS FOR SOME REASON. COMMENTS WORK FINE 
#IN NORMAL PROGRAM USE. 


# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

echo "Test 1 will be: ls # not a flag!" 

 ./rshell << EOF
ls; exit #still not a command
EOF

echo "Test 2 will be: ls #not a flag as well!" 

./rshell << EOF
ls; exit #not a flag as well!;
EOF

echo "Test 3 will be: echo hello world; #comment still respected"

./rshell << EOF
echo hello world; exit #comment still respected
EOF

echo "Test 4 will be: #does nothing"

./rshell << EOF
exit #does nothing
EOF

echo "Test 5 will be: echo hello world && echo sup dog || echo this won't run #comments"

./rshell << EOF
echo hello world && echo sup dog || echo this won't run; exit #comments
EOF

