FROM ubuntu:wily
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y clang cmake ninja-build && apt-get clean && apt-get purge && apt-get remove -y gcc
ADD ./build.sh /root/build.sh
CMD /root/build.sh
