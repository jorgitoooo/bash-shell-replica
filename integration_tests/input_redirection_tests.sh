#Brian Crites said that it is fine to have the 'exit' command at the end of commands to test execution for integration tests.
#PLEASE NOTE: The exit has to preempt the comment each time. This is due to an infinite loop being generated without using exit in these tests.
#THIS DOES NOT HAPPEN IF YOU RUN ./rshell IN THE ROOT FOLDER. THIS HAPPENS ONLY WHEN USING BASH SCRIPTS FOR SOME REASON. COMMENTS WORK FINE
#IN NORMAL PROGRAM USE.


# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

echo "Test 1 will be: cat < names.txt"

 ./rshell << EOF
cat < names.txt; exit
EOF

echo "Test 2 will be: ((cat < names.txt))"

./rshell << EOF
((cat < names.txt)); exit
EOF

echo "Test 3 will be: ls && cat < names.txt && (test -d src/ && cat < CMakeLists.txt)"

./rshell << EOF
ls && cat < names.txt && (test -d src/ && cat < CMakeLists.txt); exit
EOF

echo "Test 4 will be: (cat < names.txt) || echo wont run; cat < CMakeLists.txt #does nothing"

./rshell << EOF
(cat < names.txt) || echo wont run; cat < CMakeLists.txt; exit #does nothing
EOF
