#!/bin/sh

dir=$1
testData=$2

if [ -z $dir ]; then
  echo "Usage: ./test [source_dir] [test_data]"
  exit 1
fi

# compile
cp $dir/enumTsp.cpp .
make clean
make enumTsp

if [ -z $testData ]; then
  exit 0
fi

./enumTsp $testData

