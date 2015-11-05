#!/bin/bash

g++ -o test -O2 test.cpp -I ../include/ -I ../../../gflags/include/ -L ../../../gflags/lib -L ../lib/ -lgpr -lgsl -lgslcblas -lgflags -L ../../gflags/lib/ -lpthread
