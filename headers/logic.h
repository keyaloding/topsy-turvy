#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "board.h"
#include "pos.h"
#include "logic.h"

enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;

enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;

struct game {
    unsigned int run;
    board* b;
    posqueue *black_queue, *white_queue;
    turn player;
};

typedef struct game game;

/* Creates a new game with the specified size, configuration, and type. */
game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type);

/* Frees an existing game from memory. */
void game_free(game* g);

/* Drops a piece in the specified column. Returns true if the board changes and
 * false if the column is already full. */
bool drop_piece(game* g, unsigned int column);

/* Reflects the board across the horizontal center line and applies gravity. */
void disarray(game* g);

/* Removes the oldest piece of the current player and newest piece of the
 * opponent, then updates the board. Returns false if one or both players
 * have no pieces on the board and true otherwise. */
bool offset(game* g);

/* Reports the current status of the game. */
outcome game_outcome(game* g);

#endif /* LOGIC_H */
