#!/bin/bash

qsub -l nodes=1:gen9:ppn=2 -d . -o out.txt -e err.txt $1
