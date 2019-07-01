#Brian Crites said that it is fine to have the 'exit' command at the end of commands to test execution for integration tests.
#PLEASE NOTE: The exit has to preempt the comment each time. This is due to an infinite loop being generated without using exit in these tests.
#THIS DOES NOT HAPPEN IF YOU RUN ./rshell IN THE ROOT FOLDER. THIS HAPPENS ONLY WHEN USING BASH SCRIPTS FOR SOME REASON. COMMENTS WORK FINE
#IN NORMAL PROGRAM USE.


# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

echo "Test 1 will be: ls -l | more"

 ./rshell << EOF
ls -l | more; exit
EOF

echo "Test 2 will be: test src/; echo hello! && (ls -l | more && echo hello again!) || echo wont run"

./rshell << EOF
test src/; echo hello! && (ls -l | more && echo hello again!) || echo wont run; exit
EOF

echo "Test 3 will be: cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2; ls && (echo a && echo b)"

./rshell << EOF
cat < existingInputFile | tr A-Z a-z | tee newOutputFile1 | tr a-z A-Z > newOutputFile2; ls && (echo a && echo b); exit
EOF

