FROM ubuntu:wily
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y build-essential cmake ninja-build lcov && apt-get clean && apt-get purge
ADD ./build.sh /root/build.sh
CMD /root/build.sh
