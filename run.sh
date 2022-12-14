#!/usr/bin/env bash
# setarray=(lsun1k lsun2k lsun3k lsun4k lsun5k)
# echo 'new start' > log.txt
# for element in ${setarray[*]}
# do 
# ./build/ppdbscan r=2 & 
# ./build/ppdbscan r=1 d=$element >> log.txt

# ps -a|grep ppdbscan|awk '{print $1}'|xargs kill -9
# done

make -j16
./ppdbscan r=2 &
./ppdbscan r=1 d=$1

python3 /home/dd/Ubu20/ppdbscan/pyFile/drawImg.py $1

ps -a|grep ppdbscan|awk '{print $1}'|xargs kill -9