#! /bin/bash

builder="$(docker buildx create --platform=arm64)"
docker buildx build --output type=docker --rm --platform linux/arm64 --builder=$builder -t $USER/aarch64/ubuntu_20.04 .