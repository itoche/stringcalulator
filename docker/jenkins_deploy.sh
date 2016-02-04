#!/bin/bash -x
set -e
cd `dirname $0`
source ./configrc
cd compose
if [ "x$1" != "x" ]
then
	export SHA1=$1
fi
export SHA1=$SHA1
export DEPLOYMENT_ID=jenkins
export COMPOSE_PROJECT_NAME=$PROJECT_NAME$DEPLOYMENTID$SHA1
docker network create $PROJECT_NAME || true
docker-compose --x-networking kill || true
docker-compose --x-networking rm -f || true
docker-compose --x-networking up -d
