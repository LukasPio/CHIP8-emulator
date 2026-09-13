# Ubuntu 22.04 establishes a glibc 2.35 baseline for the Linux executable.
FROM ubuntu:22.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build curl ca-certificates pkg-config libsdl2-dev \
    gcc-mingw-w64-x86-64 zip python3 xvfb xauth xdotool \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /work
ENV CHIP8_BUILD_ROOT=/work/build/docker
CMD ["bash", "scripts/release.sh", "all"]
