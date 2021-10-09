#!/bin/bash

for var in "$@"
do
	TEST=$(file "$var" | tail -c 13)
	if [[ "$TEST" == "not stripped" ]]; then
	echo $var
	fi
done
