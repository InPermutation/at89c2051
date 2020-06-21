#!/bin/bash
set -ex

[ -f "$1.c" ]

sdcc "$1.c"
packihx "$1.ihx" > "$1.hex"
