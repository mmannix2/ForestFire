#!/bin/bash
#A script to run the forest fire simulation

exe=forestfire
infile=test/topfire.txt
N=1000
I=0.001
G=0.001

case $# in
5)
    exe=$1
    infile=$2
    N=$3
    I=$4
    G=$5
    ;;
4)
    infile=$1
    N=$2
    I=$3
    G=$4
    ;;
3)
    N=$1
    I=$2
    G=$3
    ;;
1)
    infile=$1
    ;;
0)
    ;;
*)
    echo "Bad input."
    exit 1
esac

mpirun -np 8 $exe $infile $N $I $G
