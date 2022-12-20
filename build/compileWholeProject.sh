#!/bin/bash

# This File does Fetching, pulling, linking, compiling and executing in one call.
# (Recommendation:) Optimal to use when newly checked out a branch

echo "----------------- GIT FETCHING -------------------------------------------"
git fetch

echo "----------------- GIT PULLING --------------------------------------------"
git pull

echo "----------------- LINKING LIBRARIES---------------------------------------"
cmake ../

echo "----------------- COMPILING PROJECT---------------------------------------"
make

echo "----------------- EXECUTING BINARY----------------------------------------"
./../bin/FreeRTOS_Emulator