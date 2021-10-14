#!/bin/bash
#/usr/lib/debug/lib/libc6-prof/x86_64-linux-gnu
for var in "$@"
do
	nm $var > /tmp/their.txt
	./ft_nm $var > /tmp/my.txt
	TEST=$(diff /tmp/their.txt /tmp/my.txt)
	if [[ "$TEST" == "" ]]; then
		echo 'OK'
	else
		printf "%s\n" $TEST
	fi
done
