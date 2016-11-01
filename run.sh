#!/bin/bash
#A script to run the forest fire simulation

if [ $# -ne 5  ]
then
    echo "Bad input."
    exit 1
else
    mpirun -np 8 $1 $2 $3 $4 $5
    exit 0
fi
