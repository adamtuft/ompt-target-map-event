#!/bin/bash

# Compile tool
icx -g -fPIC -shared tool.c -o libtool.so

# Compile matmul example
icx -g -qopenmp -fopenmp-targets=spir64 matmul-offload.c -o matmul

export OMP_TOOL_LIBRARIES=./libtool.so
export OMP_TARGET_OFFLOAD=MANDATORY
export LIBOMPTARGET_DEBUG=2
export LIBOMPTARGET_INFO=4

echo HOSTNAME=${HOSTNAME} >hostname.${PBS_JOBID}.txt
./matmul
