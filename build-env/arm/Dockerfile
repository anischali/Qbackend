FROM --platform=linux/arm64 multiarch/qemu-user-static as qemu_arm
FROM --platform=linux/arm64 arm64v8/ubuntu:20.04 as base
COPY --from=qemu_arm /usr/bin/qemu-arm-static /usr/bin/qemu-arm-static


ENV DEBIAN_FRONTEND noninteractive
# Package installation
RUN apt-get update -y
## Common packages for linux arm64 build environment
RUN apt-get install -y build-essential \
    nano python3 curl gnupg libncurses5 clang  \
    python3 pkg-config git curl bzip2 unzip autopoint \
    ninja-build autoconf automake make cmake libtool \
    libcurl4 libcurlpp-dev libfmt-dev libssl-dev libcurl4-openssl-dev \

RUN ln -s $(which python3) /usr/bin/python

# Working environemnt
ENV HOME /home/builder
RUN useradd --create-home --shell /bin/bash --base-dir /home builder
RUN mkdir -p /home/builder/tmp
RUN mkdir -p /home/builder/tmp
WORKDIR /home/builder/tmp

# Install the nlohmann json library
RUN curl -LO https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.zip && \
    unzip v3.11.2.zip && rm v3.11.2.zip && cd json-3.11.2 && \
    cmake . && make -j $(nproc) && make install && cd /home/builder/ && \
    rm -rf /home/builder/tmp

RUN apt -y install 

USER builder
WORKDIR /home/builder
ENTRYPOINT [ "/bin/bash" ]
