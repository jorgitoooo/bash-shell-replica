#!/bin/sh

#Brian Crites said that it is fine to have the 'exit' command at the end of commands to test execution for integration tests. 

# Moving up a directory to correct place of rshell executable 
cd ..

# Begin running tests
echo "Running integration tests" 

echo "Test 1 is going to be: ls -a;" 

 ./rshell << EOF
ls -a; exit 
EOF

echo "Test 2 is going to be: ls -a -l;" 

./rshell << EOF 
ls -a -l; exit
EOF

echo "Test 3 is going to be: echo hello world!" 

./rshell << EOF 
echo hello world!; exit
EOF

echo "Test 4 is going to be: cat CMakeLists.txt" 

./rshell << EOF 
cat CMakeLists.txt; exit 
EOF

echo "Test 5 is going to be: mkdir thisIsAFakeDirectory!"
echo "NOTE: Make sure to look in same folder as ./rshell executable for the folder." 

./rshell << EOF 
mkdir thisIsAFakeDirectory; exit
EOF
