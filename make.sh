#!/bin/bash
set -ex

sdcc blink.c
packihx blink.ihx > blink.hex
