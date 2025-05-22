Go Board
========
this is a gui to play go with the gnugo ai.
[Check it out on youtube](https://youtu.be/VXvJqaTiypQ)

[![Video Title](https://img.youtube.com/vi/VXvJqaTiypQ/0.jpg)](https://www.youtube.com/watch?v=VXvJqaTiypQ)

dependencies
============
make
ImageMagick
gcc
raylib
gnugo

installation
========
make

turn it on
=======
`sh start.sh`

controls
=========

click where you want to play. use the U key to undo. ESC turns it off.

files explained
==================

board.c (50 lines)
this program uses raylib to display a png of the game board, and it waits for the user to click where they want to play. When you click, it writes the coordinates to a file.
This is the part that gets compiled by make.

main.awk (122 lines)
this is the main program. It creates the png of the game board and talks to the gnugo server.
