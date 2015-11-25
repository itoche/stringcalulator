# Issues with firewall
If you are inside Arhs network, you need to configure docker to use the internal DNS.
That is, start the docker daemon with `--dns 10.0.0.11`

Using Ubuntu 15.10 Wily, you need to add a systemd configuration file at `/lib/systemd/system/docker.service.d/docker.conf` whose content is 

    [Service]
    EnvironmentFile=-/etc/default/docker
    ExecStart=
    ExecStart=/usr/bin/docker daemon $DOCKER_OPTS -H fd://

and have the `/etc/default/docker` file to be

    DOCKER_OPTS="--dns 10.0.0.11 --dns 10.14.255.254"
