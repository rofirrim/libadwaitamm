#!/usr/bin/env bash

set -x

# Script based on gtk's testing scripts

env

export XDG_RUNTIME_DIR="$(mktemp -p $(pwd) -d xdg-runtime-XXXXXX)"
weston --backend=headless-backend.so --socket=wayland-5 --idle-time=0 &
compositor=$!

export WAYLAND_DISPLAY=wayland-5
export GDK_BACKEND=wayland

meson test
result=$?

kill ${compositor}

exit ${result}
