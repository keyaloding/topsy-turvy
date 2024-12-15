#ifndef BOARD_H
#define BOARD_H

#include "pos.h"

enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;

/* Board type. `matrix` uses a 2-dimensional array and `bits` uses an array of
 * unsigned integers, with each integer divided into 16 cells (2 bits each). */
union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};

struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* Creates a new, empty board with the specified size and type. */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* Frees an existing board from memory. */
void board_free(board* b);

/* Prints the board to the screen with row and column headers. */
void board_show(board* b);

/* Gets the cell at a position on the board. Raises an error if
 * the cell is out of bounds. */
cell board_get(board* b, pos p);

/* Sets the cell at a position on the board. Raises an error if
 * the cell is out of bounds. */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
