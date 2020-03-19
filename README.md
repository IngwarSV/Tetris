# Tetris
Console tetris

1. BaseApp allows to produce graphical output of the program, process the keyboard and maintain main program cycle.
2. TetrisApp is based on logic:
    - each tetromino is represented by four numbers, which can be converted into array[4] with four structures Point(x,y);
    - during game cycle we work with previously mentioned array and rotation number (0, 90, 180, 270) of current tetromino.

