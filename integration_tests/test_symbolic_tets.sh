#!/bin/sh

# Moving up a directory to correct place of rshell executable
cd ..

# Begin running tests
echo "Running integration tests"

# ------------------------------------------------ TEST 1
echo "Test 1: [ -e ./src/]"
expected_output=$(echo "$ (TRUE)")
actual_output=$(./rshell << EOF
[ -e ./src/]; exit
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

#  ------------------------------------------------TEST 2
echo "Test 2: [ ./src/ ]"
expected_output=$(echo "$ (TRUE)")
actual_output=$(./rshell << EOF
[ ./src/ ]; exit
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
echo "Test 3: [ -f ./src/ ]"
expected_output=$(echo "$ (FALSE)")
actual_output=$(./rshell << EOF
[ -f ./src/ ]; exit
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

# ------------------------------------------------ TEST 4
echo "Test 4: [ -d ./src/ ]" 
expected_output=$(echo "$ (TRUE)")
actual_output=$(./rshell << EOF
[ -d ./src/ ]; exit
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

# ------------------------------------------------ TEST 5
echo "Test 5: [ -d ./dir_doesntexist/ ]" 
expected_output=$(echo "$ (FALSE)")
actual_output=$(./rshell << EOF
[ -d ./dir_doesntexist/ ]; exit
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

# ------------------------------------------------ TEST 6
echo "Test 6: [ -d ./dir_doesntexist/ ] || [ -f ./src/command.h ] && echo command.h exists" 
expected_output=$(echo "$ command.h exists
(FALSE)
(TRUE)")
actual_output=$(./rshell << EOF
[ -d ./dir_doesntexist/ ] || [ -f ./src/command.h ] && echo command.h exists; exit
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

# ------------------------------------------------ TEST 7
echo "Test 7: [ -f ./src/command.h ] || [ -d ./dir_doesntexist/ ] && echo command.h exists" 
expected_output=$(echo "$ command.h exists
(TRUE)")
actual_output=$(./rshell << EOF
[ -f ./src/command.h ] || [ -d ./dir_doesntexist/ ] && echo command.h exists; exit
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
