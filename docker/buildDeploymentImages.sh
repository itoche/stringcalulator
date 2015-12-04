#!/bin/bash -x

# Make sure we are in the right directory
cd `dirname $0`
source ./configrc

# Enforce script failure upon any error
set -e
set -o pipefail

ENVIRONMENTS=`ls deployEnvironments`
if [ "x$1" != "x" ]
then
	ENVIRONMENTS=`ls deployEnvironments | grep -i $1`
fi

# We build the Docker images for each deploy environment
for envName in $ENVIRONMENTS
do
	echo "Building deploy environment image $envName"
	cd deployEnvironments/$envName
    if [ -e setup.sh ]
    then
    	./setup.sh
    fi
    set +e
	docker build -t $PROJECT_NAME.deploy.$envName:$SHA1 .
    BUILD_SUCCESS=$?
    if [ -e teardown.sh ]
    then
        ./teardown.sh
    fi
    set -e
    if [ $BUILD_SUCCESS != 0 ]
    then
        exit 1
    fi
	cd ../..
done
