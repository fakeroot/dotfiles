#!/bin/sh

xcompmgr -cfF -r7 -o1 -l-10 -t-10 -D5 &

feh  --image-bg black --bg-center '/usr/home/f1y/.tapety/03588_morrainelake_1920x1200.jpg'

dunst -conf $HOME/.dunstrc & 
wmname LG3D &

setxkbmap -layout pl
xset r rate 300 30 
xrdb -load $HOME/.Xresources

urxvtd &

exec dwm
