#!/bin/bash

`./lab0 --input=test_input.txt --output=test_output.txt`
if [ $? -eq 0 ]
then
	echo "Program successfully exited"
else
	echo "Program was not able to copy input to output"
fi

`cmp test_input.txt test_output.txt`
if [ $? -eq 0 ]
then
	echo "Test files for input and output match"
else
	echo "Test files for input and output do not match"
fi

`./lab0 --segfault --catch`
if [ $? -eq 3 ]
then
	echo "Segfault successfully created and caught"
else
	echo "Error in catching segfault"
fi
