# Topsy-Turvy

Note: The game logic currently uses C, but I am in the process of switching to a C++ implementation.

## Rules

Topsy-Turvy is a two-player board game that can be played in the terminal.
The two players take turns dropping pieces into columns and the game is over
when a player forms a line of their pieces either vertically, horizontally,
or diagonally, similar to Connect-4.
Players can also perform two special moves: disarray and offset.

- Disarray reflects the board across the horizontal centerline then applies
  the effects of gravity.
- Offset removes the current player's oldest piece and the opponent's newest
  piece then applies the effects of gravity.  

If both players have winning runs after a special move is played, then
the game results in a draw.

## How to Play

Run the following commands in the terminal:

```Unix
git clone https://github.com/keyaloding/topsy-turvy.git
cd topsy-turvy
make play
./play
```

Running the program using the commands above creates a game with an 8x8 board
and a winning run length of 4. To specify a game configuration, run the
following command, where `x`, `y`, and `z` are placeholders that should be
replaced with integers representing the height, width, and winning run length
of the game. They can be given in any order.  
Note: all values must be greater than 0 and the run length cannot be greater
than both the height and width.

```Unix
./play -h x -w y -r z
```

- To drop a piece, type the column name (0-9, A-Z, a-z), then press enter/return.
- To perform a disarray move, type an exclamation mark (!), then press enter/return.
- To perform an offset move, type a carat (^), then press enter/return.
- To forfeit, type a period (.), then press enter/return.

## Matrix vs Bits Board Configuration

The two different ways that a board can be represented are with a
two-dimensional array of `cell` enums (matrix) or with a one-dimensional array
of unsigned integers (bits). The game uses less memory if the bits
configuration is used because it can store information from 16 cells in one
unsigned int.
For example, the default 8x8 board takes 256 bytes of memory with the
matrix configuration and 64 bytes of memory with the bits configuration.  
The game runs faster if the matrix configuration is used because the CPU does
not have to use bitwise operators to get/set cells and because this
configuration uses multithreading to perform disarray moves.  
To use the bits configuration, run `./play -h x -w y -r z -b`.  
To use the matrix configuration, run `.play -h x -w y -r z -m`.
