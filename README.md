# Topsy-Turvy

## Rules

Topsy-Turvy is a two-player board game that can be played in the terminal. The two players take turns dropping pieces into columns and the game is over when a player forms a line of their pieces either vertically, horizontally, or diagonally, similar to Connect-4. Players can also perform two special moves: disarray and offset.

- Disarray reflects the board across the horizontal centerline then applies the effects of gravity.
- Offset removes the current player's oldest piece and the opponent's newest piece then applies the effects of gravity.
If each player has a winning run after a special move is played, the game results in a draw.

## How to Play

Run the following commands in the terminal:

```Unix
git clone https://github.com/keyaloding/topsy-turvy.git
cd topsy-turvy
make play
```

Now that the source files have been compiled, you can run the following command, where `x`, `y`, and `z` are placeholders that should be replaced with integers representing the height, width, and winning run length of the game. They can be given in any order.  
Note: all values must be greater than 0 and the run length cannot be greater than both the height and width.

```Unix
./play -h x -w y -r z
```

- To drop a piece, type the column name (0-9, A-Z, a-z), then press enter/return.
- To perform a disarray move, type an exclamation mark (!), then press enter/return.
- To perform an offset move, type a carat (^), then press enter/return.
- To forfeit, type a period (.), then press enter/return.
