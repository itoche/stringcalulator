FROM ubuntu:trusty
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y g++ lcov cmake ninja-build && apt-get clean && apt-get purge
ADD ./build.sh /root/build.sh
CMD /root/build.sh
