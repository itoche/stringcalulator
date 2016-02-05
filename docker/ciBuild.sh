#!/bin/bash -x

# Enforce script failure upon any error
set -e
set -o pipefail

# Make sure we are in the right directory
cd `dirname $0`

# Get global config variables
source ./configrc

# Clean up the artifacts directory
rm -rf artifacts
mkdir artifacts

if [ -n "$(type -t build_setup)" ] && [ "$(type -t build_setup)" = function ]
then
    build_setup
fi

# Make sure we set the permissions right upon exit
function finish {
	echo "Making sure permissions on artifacts are ok"
	docker run --rm -v $PWD/artifacts:/artifacts busybox chown -R $(id -u $USER):$(id -g $USER) /artifacts
    if [ -n "$(type -t build_teardown)" ] && [ "$(type -t build_teardown)" = function ]
    then
        build_teardown
    fi
}
trap finish EXIT

ENVIRONMENTS=`ls buildEnvironments`
# You can filter which build environment(s) you wanna use
if [ "x$1" != "x" ]
then
	ENVIRONMENTS=`ls buildEnvironments | grep -i $1`
fi

# We build the Docker images for each build environment
for envName in $ENVIRONMENTS
do
	echo "Building build environment image $envName"
	cd buildEnvironments/$envName
	docker build -t $PROJECT_NAME.build.$envName:$SHA1 .
	cd ../..
done

# Build the application within each of the build environments
for envName in $ENVIRONMENTS
do
	echo "Building application on build environment image $envName"
	echo $PWD
	docker run --rm -v $PWD/..:/repository:ro -v $PWD/artifacts:/artifacts $DOCKER_BUILD_ARGS $PROJECT_NAME.build.$envName:$SHA1
done

