#include <iostream>
#include "../headers/pos.hpp"
#include "../headers/board.hpp"

using namespace std;

Board::Board(int b_height, int b_width, enum type b_type)
            : height(b_height), width(b_width), type(b_type) {}

Board::~Board() {}

vector<vector<cell>> Board::get_matrix() {
  if (Board::get_type() == BITS) {
    fprintf(stderr, "");
    exit(1);
  }

}

vector<int> Board::get_bits() {

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

  }
}

void Board::board_set(pos p, cell c) {
  
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
