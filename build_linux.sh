#!/bin/bash

set -x
CLEAN_BUILD=NO
BUILD_TEST=false
EXAMPLE_NAME=""
# parse command line args
# https://stackoverflow.com/a/14203146/12003165
for i in "$@"; do
  case $i in
    --clean)
      CLEAN_BUILD=YES
      ;;
    --test)
      BUILD_TEST=true
      ;;
    -e=*|--example=*)
      EXAMPLE_NAME="${i#*=}"
      ;;
    -*|--*)
      echo "Unknown option $i"
      exit 1
      ;;
    *)
      ;;
  esac
done

if [ "$CLEAN_BUILD" = "YES" ]; then
  rm -fr ./build
  echo "Removed build directory"
  exit 0
fi

mkdir build
cd build
cmake .. -DBUILD_TEST=${BUILD_TEST} -DEXAMPLE_NAME=${EXAMPLE_NAME} -DPLATFORM_NAME="linux"
make -j7

if [ $? -eq 0 ]; then
  # Run tests
  if [ "${BUILD_TEST}" = "true" ]; then
    echo "Running tests"
    ./tests
  else # Run examples
    if [ "${EXAMPLE_NAME}" = "" ]; then
        EXAMPLE_NAME="minimal"
    fi
    echo "./${EXAMPLE_NAME}"
    ./${EXAMPLE_NAME}
  fi
else
    echo "Building failed!"
    exit 1
fi