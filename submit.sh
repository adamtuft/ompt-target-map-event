#!/bin/bash

qsub -l nodes=1:gen9:ppn=2 -d ./tmp -o out.txt -e err.txt $1
