#!/bin/bash
#/usr/lib/debug/lib/libc6-prof/x86_64-linux-gnu
nm $@ > /tmp/their.txt
./ft_nm $@ > /tmp/my.txt
TEST=$(diff /tmp/their.txt /tmp/my.txt)
if [[ "$TEST" == "" ]]; then
	echo 'OK'
else
	printf "%s\n" $TEST
fi
