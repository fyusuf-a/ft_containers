#!/usr/bin/env bash

echo  "tests that do not have to be compared with the standard library: "
./tests/tree

echo "tests without seed: "
echo "* miscellaneous: using my library: "
./tests/miscellaneous
echo "* miscellaneous: using the standard library: "
./tests/miscellaneous_std
echo "* vector: using my library: "
./tests/vector
echo "* vector: using the standard library: "
./tests/vector_std
echo "* stack: using my library: "
./tests/stack
echo "* stack: using the standard library: "
./tests/stack_std

SEED=$RANDOM
echo "tests with seed (seed = $SEED)"
echo "* map: using my library: "
./tests/map SEED
echo "* map: using the standard library: "
./tests/map_std SEED

echo "* intra tests: using my library: "
./tests/intra-main SEED
echo "* intra tests: using the standard library: "
./tests/intra-main_std SEED
