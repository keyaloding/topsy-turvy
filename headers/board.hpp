#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "pos.hpp"

enum cell {
  EMPTY,
  BLACK,
  WHITE
};

typedef enum cell cell;

/* Board type. `matrix` uses a 2-dimensional array and `bits` uses an array of
 * unsigned integers, with each integer divided into 16 cells (2 bits each). */
union board_rep {
  vector<vector<cell>> matrix;
  vector<int> bits;
};

typedef union board_rep board_rep;

enum type {
  MATRIX, BITS
};

typedef enum type type;

struct board {
  unsigned int width, height;
  enum type type;
  board_rep u;
};

using namespace std;

class Board {
  public:
    /* Getter for the class, creates a new board with the specified size
     * and configuration. */
    Board(int b_height, int b_width, enum type b_type);

    ~Board();

    /* Prints the board to the screen with row and column headers. */
    void board_show();

    /* Gets the cell at the specified position on the board. Raises an error
     * if the position is out of bounds. */
    cell board_get(pos p);

    /* Sets the cell at the specified position on the board. Raises an error
     * if the position is out of bounds. */
    void board_set(pos p, cell c);

    int get_height();

    int get_width();

    type get_type();

    vector<vector<cell>> get_matrix();

    vector<int> get_bits();
  
  private:
    int height, width;
    board_rep u;
    type type;
};

typedef class Board Board;

#endif /* BOARD_H */
