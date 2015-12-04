# Continuous Deployment with Docker: guidelines

## TLDR;
We want to build & deploy without installing anything specific locally.
The steps are the following:
 1. build Docker images that shall be used to build the application
 2. run container based on images from 1. to build the application
 3. build Docker images that shall be used for deployment
 4. deploy the application using Docker Compose and the images of 3.

## Getting started
The skeleton of the build process is maintained in a dedicated git repository `docker-build-process`.
The make your project work with it, do the following:
 - make sure you don't have a `docker` directory at the root of the repository
 - `git remote add docker-build-process URL_OF_THE_BUILD_PROCESS_REPO`
 - `git fetch docker-build-process`
 - `git merge docker-build-process/master`

This strategy is somehow similar to git subtree strategy (https://www.kernel.org/pub/software/scm/git/docs/howto/using-merge-subtree.html), we just don't use a subfolder.

Now, you have a `docker` directory inside your repository.
You will need to add things specific to your project.

### Setting the variables & hooks
You must write the `docker/configrc` file, similar to the example one already in the repository.
You'll need to define the following environment variable:

    export PROJECT_NAME=myproject

This variable will be used for naming your Docker images and containers.

You can (but it is not mandatory) define the following bash functions (the body here is just given as example):

    function build_setup() {
        # Run a data container to expose the secrets
        docker run -d --name=$PROJECT_NAME.secrets.$SHA1 $PROJECT_NAME.secrets
    }
    
    function build_teardown() {
        echo "Dropping data container run as daemon"
        docker rm -f $PROJECT_NAME.secrets.$SHA1
    }

This allows custom behavior to happen before and after "build application" stage. The teardown function will be called even if the build fails.

You can (but it is not mandatory) define the following environment variable (again, content given as example):

    export DOCKER_BUILD_ARGS="--volumes-from $PROJECT_NAME.secrets.$SHA1"

This will be added to the command line arguments for starting each container for building the application.

### Building the application
In the end, you'll build your application by simply running the `ciBuild.sh` script in the `docker` directory.
Here is what you need to do to make things work:

In the `docker` directory, you must create a `buildEnvironments` directory.
Each subfolder of that directory is meant to configure a build environment. For instance, if you have a web application with a Java backend and an AngularJS frontend, you'll have two build environments. One for Java, that'll use Maven as build tool. Another for AngularJS, that would use a NodeJS toolchain like Gulp or Grunt.

So in short:
 - `cd docker`
 - `mkdir -p buildEnvironments/maven3`
 - `cd buildEnvironments/maven3`

in this directory, you describe a Docker image to build your application, knowing that the git repository will be mounted at `/repository` and will be read-only. Typically, this involves writing a `Dockerfile` using a relevant public image (like `maven:3-jdk8` or `node:4`), adding a build script that will copy `/repository` somewhere inside the container and run the build process. Make sure that your build script exits with an error code if the build is not successful (i.e. use `set -e` and `set -o pipefail` in bash).

During the build, a volume `/artifacts` is mounted inside the container running the build. Your build script should copy the artifacts to that folder before exiting. The artifacts include all the things needed to deploy the application (e.g. the `.war` file or the minified javascript/html/css) and the test/code coverage reports.

### Deploying the application
Now that you can successfully build the application using Docker, you want to deploy it locally.
You first need to build the Docker images that will be used for deployment.
You'll describe them in the `docker/deployEnvironments` directory.
As for building the Docker images used for the build process, you need to specify one subfolder of `deployEnvironments` for the image used by each container needed for your deployment.
For instance, for a Java/AngularJS web application, you will typically have three containers:
 - the Java backend, using e.g. Tomcat
 - a database server, e.g. PostgreSQL
 - a web server, serving your Javascript/Css/Html and proxying the API calls to the Java backend

For each image you need to provide:
 - the `Dockerfile`
 - a `setup.sh` script, that should copy to the local directory the artifacts that you need (remember, in your `Dockerfile`, you can only `ADD` from the current directory.
 - a `teardown.sh` script that should clean up the stuff you've copied in the `setup.sh`
The images will be named `${PROJECT_NAME}.deploy.${DEPLOY_ENVIRONMENT}:${SHA1}`. Note that they are tagged with the SHA1 hash of the current Git commit.

Upon start, the container built upon this image must wait for the service to be ready, and then block on:

    nc -l -k 54321 &> /dev/null

This means the `netcat` tool should listen on the 54321 tcp port (54321 because it looks like a countdown), and the other containers or the tool managing the deployment can tell whether containers are ready.
For that to work, you need to make sure the `netcat-openbsd` package is installed inside your image.
If one container depends upon another one to be started, it should wait for port 54321 to be opened, using for instance

        set +e
        while ! nc -z $CONTAINER_NAME_OF_THE_CONTAINER_IT_DEPENDS_UPON 54321; do   
            sleep 0.1
        done
        set -e

You will read the `$CONTAINER_NAME_OF_THE_CONTAINER_IT_DEPENDS_UPON` variable from environment.

Next, you need to orchestrate the containers built upon those various images. This means writing the file `docker/compose/docker-compose.yml`.
For the process to work, you have a few specific requirements:
 - make sure to specify `container_name` to be `${PROJECT_NAME}.THE_NAME.${DEPLOYMENT_ID}.${SHA1}`, where `THE_NAME` is the same as the name of the container (i.e. the top level item in the yaml file). The other variables will be read from the environment when deploying.
 - the entry point of your application should always be named `entrypoint` and should serve on port 80 (always) and maybe on 443. If your application should be SSL only, then you should configure a redirect from ttp to https. The goal is that hitting that container on port 80 at the root context will actually work. This may require you to add an Apache/Nginx layer to your application (which is quite easy with Docker).

Finally, you can deploy the application locally.
- Make sure all your changes are committed
- `cd docker` 
-  `./ciBuild.sh`
-  `./buildDeploymentImages.sh`
-  `./deploy.sh`: this will set the proper environment variables for Docker Compose and launch `docker-compose up`. This will write the output at the console. Stop it with `Ctrl+C`

## Some guidelines for managing configuration

 - Everything must be in a Git repository. Anyone should be able to have a deployment with just Docker and some git repository clones.
 - The source code repository must not contain any configuration that is specific to a given deployment or infrastructure:
     - no credentials should be stored on the source code repository
     - no URL for external services (e.g. external SMTP, remote service to connect to)

### Suggested repository layout

You should have a few Git repositories for your project:

 - The `main` repository hosting the source code and the build process
     - remember, the build process scripts are maintained by adding the `docker-build-process` remote and merging with it when necessary.
 - The `secrets` repository, that contains all credentials that are necessary to build/deploy internally. 
     - It should allow to build a Docker image containing those secrets.
     - When you start a container based on this image, it should copy the secrets to a volume `/secrets`, so that you can access it from the container building the application using `--volumes-from`.
 - The `arhs-config` repository, that contains Arhs specific configurations, namely:
     - What are the environment variables that we should use to deploy locally.
         - It makes sense that you have a version number to your variables (similar to what you do with versions of the schema).
             - This will ensure you can using the information of the latest commit of this `arhs-config` repository deploy any version of the application.
     - Which version of the management console we want to use for this project.

## Management console

Goals: we want to be able to:
 - deploy automatically just upon `git push`.
 - list the currently deployed environments
 - have the same commit deployed twice
 - easily browse each deployed environment
 - destroy an environment
 - redeploy the same commit on the same url
 - mess with the environment by
     - getting a root shell on any container of the environment
     - uploading data to any container

We've development a small web app to do that. It covers all the features listed above.
### Architecture
 - Frontend in AngularJS 1.X + Angular-Material
 - Python/Flask microservice for listing/deploying/undeploying environments.
 - NodeJS microservice for serving remote shells and uploading to containers
     - for the remote shell, we just start a `docker exec ... bash` session and forward it to the browser using `term.js`. `term.js` uses Socker.IO internally
 - Nginx proxy to:
     - serve the Javascript/CSS/HTML
     - proxy pass to the Python and NodeJS microservices
     - proxy pass to each deployed environment using regular expressions on the domain name.

### Deploying the management console

The management console is built using the process described in this document. You need to
 - clone the repository
 - `cd docker`
 - `./ciBuild.sh && ./buildDeploymentImages.sh`
 - write a launch script looking like
 
     #!/bin/bash -x
    cd `dirname $0`
    export SERVED_PROJECT_NAME=myproject
    export REPO_PATH="path_to_a_git_clone_of_my_project"
    ./deploy.sh

 - Call your script
