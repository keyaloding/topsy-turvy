#include <iostream>
#include "../headers/pos.h"
#include "../headers/board.hpp"

using namespace std;

Board::Board(int b_height, int b_width, enum type b_type) : u () {
  Board::set_height(b_height);
  Board::set_width(b_width);
  Board::set_type(b_type);
}

vector<vector<cell>> Board::get_matrix() {
  return Board::u.matrix;
}

vector<int> Board::get_bits() {
  return Board::u.bits;
}

void Board::board_show() {
  int i, j, height = Board::get_height(), width = Board::get_width();
  pos p;
  cout << "\n  ";
  for (j = 0; j < width; j++) {
    if (j < 10) {
      cout << "%u", j;
    } else if (j < 36) {
      cout << "%c", '7' + j;
    } else if (j < 62) {
      cout << "%c", '=' + j;
    } else {
      cout << "?";
    }
  }
  cout << endl;
  for (i = 0; i < height; i++) {
    if (i < 10) {
      cout << "%u ", i;
    } else if (i < 36) {
      cout << "%c ", '7' + i;
    } else if (i < 62) {
      cout << "%c ", '=' + i;
    } else {
      cout << "? ";
    }
    for (j = 0; j < width; j++) {
      p.c = j;
      switch (board_get(p)) {
        case EMPTY:
          cout << ".";
          break;
        case BLACK:
          cout << "*";
          break;
        case WHITE:
          cout << "o";
          break;
      };
    }
    cout << "|\n";
  }
  cout << "  ";
  for (j = 0; j < width; j++) {
    cout << "-";
  }
  cout << endl << endl;
}

cell Board::board_get(pos p) {
  int height = Board::get_height(), width = Board::get_width();
  if (p.r >= height || p.c >= width) {
    fprintf(stderr, "Error: Position (%u, %u) is out of bounds\n", p.r, p.c);
    exit(1);
  }
  if (Board::get_type() == MATRIX) {
    return Board::get_matrix()[p.r][p.c];
  } else {
    int bit_index = (p.r * width + p.c) * 2;
    int val = (Board::get_bits()[bit_index / 32] >> (bit_index % 32)) & 0x3;
    switch (val) {
      case 0:
        return EMPTY;
      case 1:
        return BLACK;
      case 2:
        return WHITE;
      default:
        fprintf(stderr, "Error: Invalid bit representation\n");
        exit(1);
    };
  }
}

void Board::board_set(pos p, cell c) {
  int width = Board::get_width(), height = Board::get_height();
  if (p.r >= height || p.c >= width) {
    fprintf(stderr, "Error: Position (%u, %u) is out of bounds\n", p.r, p.c);
    exit(1);
  }
  if (Board::get_type() == MATRIX) {
    Board::get_matrix()[p.r][p.c] = c;
  } else {
    int bit_val, bit_index = (p.r * width + p.c) * 2;
    Board::get_bits()[bit_index / 32] &= ~(0x3 << (bit_index % 32));
    switch (c) {
      case EMPTY:
        bit_val = 0;
        break;
      case BLACK:
        bit_val = 1;
        break;
      case WHITE:
        bit_val = 2;
        break;
    };
  }
}
 
int Board::get_height() {
  return Board::height;
}

int Board::get_width() {
  return Board::width;
}

type Board::get_type() {
  return Board::type;
}
