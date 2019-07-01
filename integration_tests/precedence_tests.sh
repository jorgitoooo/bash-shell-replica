#!/bin/sh

# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

# ------------------------------------------------ TEST 1
echo "Test 1: (echo hi) && (( test -d dkajdf || echo doesnt exist) && echo bye)"
expected_output=$(echo "$ hi
doesnt exist
bye
(FALSE)")
actual_output=$(./rshell << EOF
(echo hi) && (( test -d dkajdf || echo doesnt exist) && echo bye); exit
EOF)

echo "
Expected Output:
\"${expected_output}\"
"
echo "
Actual Ouput:
\"${actual_output}\"
"

    if [ "${expected_output}" = "${actual_output}" ]
    then
        echo "
		TEST PASSED
	"
    else
        echo "
		TEST FAILED
	"
    fi

# ------------------------------------------------ TEST 2
echo "Test 2: (( test -d dkajdf || test fake/path  && echo doesnt exist) && echo print?)"
expected_output=$(echo "$ (FALSE)
(FALSE)")
actual_output=$(./rshell << EOF
(( test -d dkajdf || test fake/path  && echo doesnt exist) && echo print?); exit
EOF)

echo "
Expected Output:
\"${expected_output}\"
"
echo "
Actual Ouput:
\"${actual_output}\"
"

    if [ "${expected_output}" = "${actual_output}" ]
    then
        echo "
		TEST PASSED
	"
    else
        echo "
		TEST FAILED
	"
    fi

# ------------------------------------------------ TEST 3
echo "Test 3: ([ -f ./CMakeLists.txt ] || (( test -d dkajdf || echo doesnt exist) && echo bye))"
expected_output=$(echo "$ (TRUE)")
actual_output=$(./rshell << EOF
([ -f ./CMakeLists.txt ] || (( test -d dkajdf || echo doesnt exist) && echo bye); exit)
EOF)

echo "
Expected Output:
\"${expected_output}\"
"
echo "
Actual Ouput:
\"${actual_output}\"
"

    if [ "${expected_output}" = "${actual_output}" ]
    then
        echo "
		TEST PASSED
	"
    else
        echo "
		TEST FAILED
	"
    fi


