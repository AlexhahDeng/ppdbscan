#!/usr/bin/env bash
./build/ppdbscan r=2
./build/ppdbscan r=1 d=lsun
python3 drawImg.py