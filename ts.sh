#!/bin/sh

dir=$1
testData=$2

if [ -z $dir ] || [ -z $testData ]; then
  echo "Usage: ./test [source_dir] [test_data]"
fi

# compile
cp $dir/enumTsp.cpp .
make clean
make enumTsp

./enumTsp $testData

