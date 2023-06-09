#!/bin/bash


set -x
CLEAN_BUILD=NO
BUILD_TEST=false
BUILD_DOCS=false
BUILD_RELEASE=false
CI=false
GENERATE_KERNELS=false
EXAMPLE_NAME=""
PLATFORM_NAME="linux"
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
    -d|--docs)
      BUILD_DOCS=true
      ;;
    -g|--generate-kernels)
      GENERATE_KERNELS=true
      ;;
    -r=*|--release=*)
      BUILD_RELEASE=true
      PLATFORM_NAME="${i#*=}"
      ;;
    -e=*|--example=*)
      EXAMPLE_NAME="${i#*=}"
      ;;
    --ci)
      CI=true
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
  echo "removed build directory"
  exit 0
fi

mkdir build
cd build
cmake .. -DEXAMPLE_NAME=${EXAMPLE_NAME}
make -j7

if [ $? -eq 0 ]; then
  if [ "${EXAMPLE_NAME}" = "" ]; then
      EXAMPLE_NAME="minimal"
  fi
  echo "./${EXAMPLE_NAME}"
  ./${EXAMPLE_NAME}
else
    echo "Building fails!"
    exit 1
fi