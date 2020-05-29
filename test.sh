#!/bin/bash

assert() {

	expected="$1"
	input="$2"

	./9cc "$input" > tmp.s
	gcc -o tmp tmp.s
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$input => $expected expected, but got $actual"
		exit
	fi
}

assert 0 0
assert 42 42

assert 21 "5+20-4"
assert 101 "255-102+2-54"
