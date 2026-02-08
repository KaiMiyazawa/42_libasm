FROM ubuntu:22.04

# Avoid interactive prompts during package install
ENV DEBIAN_FRONTEND=noninteractive

# Base toolchain for libasm development and tests
# - build-essential: gcc/ld and basic build tools
# - nasm: assembler required by libasm
# - make/git: build + dependency fetch
RUN apt-get update && apt-get install -y \
	build-essential \
	nasm \
	make \
	git \
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*

# Project workspace mount point
WORKDIR /work

# Default to an interactive shell
CMD ["/bin/bash"]

# Keep PATH consistent in interactive sessions
RUN echo 'export PATH="/usr/bin:$PATH"' >> ~/.bashrc
