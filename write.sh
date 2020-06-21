#!/bin/bash
set -ex

# usage: ./write.sh blink.hex

minipro -p AT89C2051 -w $1
