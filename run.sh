#!/bin/bash
#A script to run the forest fire simulation

exe=forestfire
infile=test/topfire.txt
N=1000
I=0.001
G=0.001

case $# in
5)
    #Specify all of the parameters
    exe=$1
    infile=$2
    N=$3
    I=$4
    G=$5
    ;;
4)
    #Use default exe
    infile=$1
    N=$2
    I=$3
    G=$4
    ;;
3)
    #Use default exe and infile
    N=$1
    I=$2
    G=$3
    ;;
1)
    #Only specify the infile
    infile=$1
    ;;
0)
    #Use all defaults
    ;;
*)
    echo "Bad input."
    exit 1
esac

#Run this baby!
mpirun -np 8 $exe $infile $N $I $G
exit $?
