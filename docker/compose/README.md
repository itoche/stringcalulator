A environment variable with the SHA1 of the commit you want to deploy is needed.

For instance, you can deploy the application with:

    SHA1=$(git rev-parse HEAD | head -c8) docker-compose up

If you want to have multiple local deployments, set for each a different project name using the environment variable `COMPOSE_PROJECT_NAME`
