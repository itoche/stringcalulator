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
	docker build -t $PROJECT_NAME.deploy.$envName:$SHA1 .
    if [ -e teardown.sh ]
    then
        ./teardown.sh
    fi
	cd ../..
done
