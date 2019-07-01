#!/bin/sh

#Brian Crites said that it is fine to have the 'exit' command at the end of commands to test execution for integration tests.
#ONLY TESTING 3 RUNS BECAUSE 'exit' IS USED IN ALL PREVIOUS TESTS. 

# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

echo "Test 1 is going to be: ls -a; echo exit in next command will run && exit"

 ./rshell << EOF
ls -a; echo exit in next command will run && exit
EOF

echo "Test 2 is going to be: ls -a -l; ls -a -l; exit"

./rshell << EOF
ls -a -l; ls -a -l; exit
EOF

echo "Test 3 will be: echo hello world! || exit && echo previous exit skipped! && exit" 
./rshell << EOF
echo hello world! || exit && echo previous exit skipped! && exit
EOF
