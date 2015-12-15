#!/bin/sh
if [ -e /bin/bash ]
then
    echo "Happy: bash is there"
else
  if [ "x$(uname -ms)" = "xLinux x86_64" ]
  then
    echo "Lucky you, I've already a statically compiled version of bash right here"
    cp $(dirname $0)/bash-linux /bin/bash
  else
    echo "No luck, I cannot help"
    exit 1
fi
