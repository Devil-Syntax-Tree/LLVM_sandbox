FROM ubuntu:latest

RUN apt update && apt upgrade -y \
    && DEBIAN_FRONTEND=noninteractive apt install -y \
        sudo git wget lsb-release software-properties-common gnupg \
        build-essential gdb binutils python3 python3-pip cmake ninja-build \
        pkg-config mold libzstd-dev libcurl4-openssl-dev libedit-dev \
        zlib1g-dev libffi-dev libtinfo-dev libxml2-dev locales \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8 \
    && groupadd devgroup \
    && useradd -g devgroup -m -s /bin/bash dev \
    && echo "dev ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers \
    && echo "LANG=en_US.utf8" >> /etc/environment \
    && bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" \
    && DEBIAN_FRONTEND=noninteractive apt install -y clang-format-17

USER dev

WORKDIR /workspace
