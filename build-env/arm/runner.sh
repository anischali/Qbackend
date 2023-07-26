#! /bin/bash

docker run --mount type=bind,source="${HOME}/sources",destination="/home/builder/sources" --interactive --tty --rm $USER/aarch64/ubuntu_20.04