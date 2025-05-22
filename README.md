Go Board
========
this is a gui to play go with the gnugo ai.

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

click where you want to play. use the U key to undo.

files explained
==================

board.c (50 lines)
this program uses raylib to display a png of the game board, and it waits for the user to click where they want to play. When you click, it writes the coordinates in a file.
This is the part that gets compiled by make.

main.awk (130 lines)
this is the main program.
