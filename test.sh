#!/bin/bash
cc="$1"
bin="$2"

assert(){
    expected="$1"
    input="$2"

    "$bin" "$input" > tmp.s
    "$cc" -o tmp tmp.s
    compiled="$?"
    if [ "$compiled" != "0" ]; then
        echo "failed to compile \"$cc\" -o tmp tmp.s"
        cat tmp.s
        exit
    fi
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "SUCCESS: $input => $actual"
    else
        echo "FAILURE: $input => $expected expected, but got $actual"
        cat tmp.s
        rm tmp tmp.s
        exit
    fi
    rm tmp tmp.s
}

compile_fail_assert(){
    input="$1"
    echo ""
    echo "$bin \"$input\""
    "$bin" "$input" # 2> /dev/null
    success="$?"
    if [ "$success" != "1" ]; then
        echo "FAILURE: $bin $input expected failure"
        exit
    else
        echo "SUCCESS: Failed Compilation $input"
    fi
}

assert 0 "0;"
assert 42 "42;"

assert 21 "5+20-4;"
assert 101 "255-102+2-54;"

assert 203 "255     - 102+ 102+2 -54;"

assert 47 "5 + 6 *7;"
assert 15 "5 * (9    -  6);"
assert 4 "(3+5)/2;"
assert 40 "((10+(2*3))*5)/2;"

assert 47 "+5 + 6 *7;"
assert 15 "+5 * ((-3*-3)    +  (-6));"

assert 5 "(33 % 10) * (20 % 3) + (-1);"

assert 1 "1 + 9 == 11 - 1;"
assert 0 "10 != 20 / 2;"
assert 1 "10 > 8 + (1 == 1);"
assert 0 "9 > 10;"
assert 0 "10 < 9;"
assert 1 "9 < 10;"
assert 1 "10 >= 9;"
assert 0 "9 >= 10;"
assert 0 "10 <= 9;"
assert 1 "9 <= 10;"
assert 1 "10 <= 10;"
assert 1 "10 >= 10;"
assert 2 "(10 >= 10 == 1) + (1 < 0 == 0);"

assert 255 "a = 255;"
assert 12 "a = 12;a;"
assert 23 "a = 12; b = 11; a + b;"
assert 33 "a = 2; d = 3; v = 5; m = d * 11; (a + d == v) * m;"

assert 255 "IMMULTI_BYTE = 255;"
assert 12 "IMMULTI_BYTE = 12;IMMULTI_BYTE;"
assert 23 "IMMULTI_BYTE = 12; b = 11; IMMULTI_BYTE + b;"
assert 33 "IMMULTI_BYTE = 2; d = 3; v = 5; m = d * 11; (IMMULTI_BYTE + d == v) * m;"

compile_fail_assert ""
compile_fail_assert "                      "
compile_fail_assert ";"
compile_fail_assert "                      ;"
compile_fail_assert "+"
compile_fail_assert ")"
compile_fail_assert "="
compile_fail_assert "9"
compile_fail_assert "10 + 1; 4"
compile_fail_assert "("
compile_fail_assert "(;"
compile_fail_assert "(1+( 2 + 4);"
compile_fail_assert "+ 1 + 3 + ;"
compile_fail_assert "+ 1 - 2"
compile_fail_assert "*1+1;"
compile_fail_assert "_d = 1; d;"
compile_fail_assert "a = 1 + 2; _c = 2; a + c;"
compile_fail_assert "a\$#\` = 1; a"
