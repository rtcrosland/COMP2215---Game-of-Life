# COMP2215 - Game of Life

# Description
This is an implementation of Conway's Game of Life for the LaFortuna to be entered into the best application category.

# Running on your Fortuna
This can be compiled and flashed using Klaus' universal Makefile, it may also be necessary to power cycle the Fortuna after flashing.

# Playing
Currently the settings menu has no effect on the actual game, this is something to be completed in the future.
When playing simply press and hold the centre button to draw in a cell and use the direction keys to move your cursor around.
When you are happy with your pattern move your cursor to the top left corner of the screen and press the centre button, it is important you don't accidently press this when you're drawing.
Once the simulation starts it will run indefinitely, so to play again simply turn the Fortuna off then on again.

# Rules for Conway's Game of Life
1. Any live cell with fewer than two live neighbors dies, as if caused by under population.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
