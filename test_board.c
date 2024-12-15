#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <criterion/criterion.h>

#include "board.h"

Test(board_new, test_new_board_matrix) {
  board* b = board_new(8, 6, MATRIX);
  cr_assert(b->width = 8);
  cr_assert(b->height = 6);
  cr_assert(b->type == MATRIX);
  unsigned int i, j;
  pos p;
  for (i = 0; i < b->height; i++) {
    p.r = i;
    for (j = 0; j < b->width; j++) {
      p.c = j;
      cr_assert(!board_get(b, p));
    }
  }
  board_free(b);
}

Test(board_new, test_new_board_bits) {
  board* b = board_new(4, 5, BITS);
  cr_assert(b->width = 4);
  cr_assert(b->height = 5);
  cr_assert(b->type == BITS);
  unsigned int i, j;
  pos p;
  for (i = 0; i < b->height; i++) {
    p.r = i;
    for (j = 0; j < b->width; j++) {
      p.c = j;
      cr_assert(!board_get(b, p));
    }
  }
  board_free(b);
}

Test(board_get, test_get1_matrix) {
  board* b = board_new(7, 8, MATRIX);
  pos p = make_pos(6, 3);
  cr_assert(!board_get(b, p));
  board_free(b);
}

Test(board_get, test_get1_bits) {
  board* b = board_new(7, 8, BITS);
  pos p = make_pos(6, 3);
  cr_assert(!board_get(b, p));
  board_free(b);
}

Test(board_get, test_get2_matrix) {
  board* b = board_new(8, 9, MATRIX);
  pos p = make_pos(4, 5);
  board_set(b, p, BLACK);
  cr_assert(board_get(b, p) == BLACK);
  board_set(b, p, WHITE);
  cr_assert(board_get(b, p) == WHITE);
  board_set(b, p, EMPTY);
  cr_assert(!board_get(b, p));
  board_free(b);
}

Test(board_get, test_get2_bits) {
  board* b = board_new(8, 9, BITS);
  pos p = make_pos(4, 5);
  board_set(b, p, BLACK);
  cr_assert(board_get(b, p) == BLACK);
  board_set(b, p, WHITE);
  cr_assert(board_get(b, p) == WHITE);
  board_set(b, p, EMPTY);
  cr_assert(!board_get(b, p));
  board_free(b);
}