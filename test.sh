#!/bin/bash
make clean
make

total=0
score=0

run_unit_test () 
    { 
    message=$($1 $2 $3) 
    echo "Test case: $1 $2 $3" 
    $1 $2 $3 > null 
    out=$?
    total=$(( $total+2 ))

    if [ $out == $4 ]
    then
        echo "PASSED [return value]: expected output value \"$4\" got $out"
        (( score++ ))
    else
        echo "FAILED [return value]: expected output value \"$4\" got $out"
    fi

    if [ "$message" == "$5" ] 
    then
        echo "PASSED [output message]: expected printout message \"$5\" got \"$message\""
        (( score++ ))
    else
        echo "FAILED [output message]: expected printout message \"$5\" got \"$message\""
    fi
    }

EXES=(build/main)

for testExecutable in ${EXES[@]}
do 
    path="test_data/"
    echo "-------------- UNIT TESTING $testExecutable --------------"

    echo ""
    echo "Usage"
    run_unit_test ./$testExecutable "" "" 0 "Usage: ./$testExecutable <mapfile>"

    echo ""
    echo "Bad Arguments"
    run_unit_test ./$testExecutable "1" "2" 253 "ERROR: Bad Argument Count"

    echo ""
    echo "Bad Filename"
    run_unit_test ./$testExecutable "blahblah.map" "" 252 "ERROR: Bad File Name"
done

make clean
rm null