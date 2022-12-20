#!/bin/bash

# This File does git pulling, cmake linking, c compiling and executing in one call.
# (Recommendation:) Optimal to use when newly checked out a branch

echo "----------------- GIT PULLING --------------------------------------------"
git pull

echo "----------------- LINKING LIBRARIES---------------------------------------"
cmake ../

echo "----------------- COMPILING PROJECT---------------------------------------"
make

echo "----------------- EXECUTING BINARY----------------------------------------"
./../bin/FlappyBird