#!/bin/sh

#Brian Crites said that it is fine to have the 'exit' command at the end of commands to test execution for integration tests.

# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

echo "Test 1 is going to be: ls -a; ls -a" 

./rshell << EOF 
ls -a; ls -a; exit
EOF

echo "Test 2 is going to be: ls -a && cat CMakeLists.txt || echo THIS SHOULD NOT RUN" 

./rshell << EOF 
ls -a && cat CMakeLists.txt || echo THIS SHOULD NOT RUN; exit
EOF

echo "Test 3 is going to be: echo Hello World!; echo Hello World Again! && ls -a -l || echo this should also not run" 

./rshell << EOF 
echo Hello World!; echo Hello World Again! && ls -a -l || echo this should also not run; exit
EOF

echo "Test 4 is going to be: ls -a || echo sup! && echo testing this program || echo will run not run" 

./rshell << EOF
ls -a || echo sup! && echo testing this program || echo will run; exit
EOF
