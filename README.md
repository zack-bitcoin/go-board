Go Board
========
this is a gui to play go with the gnugo ai.
[Check it out on youtube](https://youtu.be/VXvJqaTiypQ)

[![Video Title](https://img.youtube.com/vi/VXvJqaTiypQ/0.jpg)](https://www.youtube.com/watch?v=VXvJqaTiypQ)

dependencies
============
ImageMagick
gcc
gnugo

installation
========

`sh x11_compile.sh`

or to use the raylib version `make`

turn it on
=======
`sh start.sh`

controls
=========

click where you want to play. use the U key to undo. use the S key to estimate the score. ESC turns it off.

files explained
==================

board.c (42 lines)
this program uses raylib to display a png of the game board, and it waits for the user to click where they want to play. When you click, it writes the coordinates to a file.

x11_board.c (114 lines)
this is a rewrite of board.c, but using x11 instead of raylib.

main.awk (126 lines)
this is the main program. It creates the png of the game board and talks to the gnugo server.
