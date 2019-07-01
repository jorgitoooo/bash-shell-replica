#Brian Crites said that it is fine to have the 'exit' command at the end of commands to test execution for integration tests.
#PLEASE NOTE: The exit has to preempt the comment each time. This is due to an infinite loop being generated without using exit in these tests.
#THIS DOES NOT HAPPEN IF YOU RUN ./rshell IN THE ROOT FOLDER. THIS HAPPENS ONLY WHEN USING BASH SCRIPTS FOR SOME REASON. COMMENTS WORK FINE
#IN NORMAL PROGRAM USE.


# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

echo "Test 1 will be: echo hello! > newFile1.txt"

 ./rshell << EOF
echo hello! > newFile1.txt; exit
EOF

echo "Test 2 will be: (echo hello! && echo hello again!) > newFile2.txt"

./rshell << EOF
(echo hello! && echo hello again!) > newFile2.txt; exit
EOF

echo "Test 3 will be: test src/ && echo sup > newFile3.txt && (echo yo! > newFile3.txt)"

./rshell << EOF
test src/ && echo sup > newFile3.txt && (echo yo! > newFile3.txt); exit
EOF

echo "Test 4 will be: echo first >> newFile4.txt; echo second >> newFile4.txt"

./rshell << EOF
echo first >> newFile4.txt; echo second >> newFile4.txt; exit
EOF

echo "Test 5 will be: echo first >> newFile5.txt || (echo second >> newFile5.txt)"

./rshell << EOF
echo first >> newFile5.txt || (echo second >> newFile6.txt); exit
EOF

echo "Test 6 will be: ls; echo hello there > newFile6.txt; test -f names.txt && ((echo hello again >> newFile6.txt))"

./rshell << EOF
ls; echo hello there > newFile6.txt; test -f names.txt && ((echo hello again >> newFile6.txt)); exit
EOF
