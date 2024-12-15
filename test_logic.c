#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>

#include "board.h"
#include "logic.h"

Test(new_game, test_new_game_matrix) {
  game* g = new_game(5, 16, 4, MATRIX);
  cr_assert(g->b->type == MATRIX);
  cr_assert(g->b->width == 16);
  cr_assert(g->b->height == 4);
  cr_assert(g->player == BLACKS_TURN);
  cr_assert(g->run == 5);
  cr_assert(!g->black_queue->len);
  cr_assert(!g->white_queue->len);
  game_free(g);
}

Test(new_game, test_new_game_bits) {
  game* g = new_game(5, 16, 4, BITS);
  cr_assert(g->b->type == BITS);
  cr_assert(g->b->width == 16);
  cr_assert(g->b->height == 4);
  cr_assert(g->player == BLACKS_TURN);
  cr_assert(g->run == 5);
  cr_assert(!g->black_queue->len);
  cr_assert(!g->white_queue->len);
  game_free(g);
}

Test(drop_piece, test_drop_matrix) {
  game* g = new_game(5, 6, 2, MATRIX);
  pos p;
  cr_assert(drop_piece(g, 0));
  cr_assert(drop_piece(g, 1));
  cr_assert(drop_piece(g, 2));
  cr_assert(drop_piece(g, 2));
  cr_assert(drop_piece(g, 4));
  cr_assert(drop_piece(g, 4));
  cr_assert(drop_piece(g, 5));
  cr_assert(!drop_piece(g, 2));
  cr_assert(!drop_piece(g, 4));
  cr_assert(g->black_queue->len == 4);
  cr_assert(g->white_queue->len == 3);
  cell target[2][6] = {
    {EMPTY, EMPTY, WHITE, EMPTY, WHITE, EMPTY},
    {BLACK, WHITE, BLACK, EMPTY, BLACK, BLACK}
  };
  for (int i = 0; i < 2; i++) {
    p.r = i;
    for (int j = 0; j < 6; j++) {
      p.c = j;
      cr_assert(target[i][j] == board_get(g->b, p));
    }
  }
  cr_assert(g->player == WHITES_TURN);
  game_free(g);
}

Test(drop_piece, test_drop_bits) {
  game* g = new_game(5, 6, 2, BITS);
  pos p;
  cr_assert(drop_piece(g, 0));
  cr_assert(drop_piece(g, 1));
  cr_assert(drop_piece(g, 2));
  cr_assert(drop_piece(g, 2));
  cr_assert(drop_piece(g, 4));
  cr_assert(drop_piece(g, 4));
  cr_assert(drop_piece(g, 5));
  cr_assert(!drop_piece(g, 2));
  cr_assert(!drop_piece(g, 4));
  cr_assert(g->black_queue->len == 4);
  cr_assert(g->white_queue->len == 3);
  cell target[2][6] = {
    {EMPTY, EMPTY, WHITE, EMPTY, WHITE, EMPTY},
    {BLACK, WHITE, BLACK, EMPTY, BLACK, BLACK}
  };
  for (int i = 0; i < 2; i++) {
    p.r = i;
    for (int j = 0; j < 6; j++) {
      p.c = j;
      cr_assert(target[i][j] == board_get(g->b, p));
    }
  }
  cr_assert(g->player == WHITES_TURN);
  game_free(g);
}

Test(disarray, test_disarray1) {
  game* g = new_game(4, 3, 4, MATRIX);
  drop_piece(g, 1);
  drop_piece(g, 1);
  drop_piece(g, 0);
  drop_piece(g, 2);
  drop_piece(g, 2);
  drop_piece(g, 0);
  drop_piece(g, 1);
  disarray(g);
  cell target[4][3] = {
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, BLACK, EMPTY},
    {BLACK, WHITE, WHITE},
    {WHITE, BLACK, BLACK}
  };
  pos p;
  for (int i = 0; i < 4; i++) {
    p.r = i;
    for (int j = 0; j < 3; j++) {
      p.c = j;
      cr_assert(target[i][j] == board_get(g->b, p));
    }
  }
  cr_assert(g->black_queue->len == 4);
  cr_assert(g->white_queue->len == 3);
  pq_entry *q_black = g->black_queue->head, *q_white = g->white_queue->head;
  while (q_black) {
    cr_assert(board_get(g->b, q_black->p) == BLACK);
    q_black = q_black->next;
  }
  while (q_white) {
    cr_assert(board_get(g->b, q_white->p) == WHITE);
    q_white = q_white->next;
  }
  cr_assert(g->player == BLACKS_TURN);
  game_free(g);
}

Test(disarray, test_disarray2) {
  game* g = new_game(3, 4, 4, BITS);
  unsigned int i, j;
  pos p;
  drop_piece(g, 3);
  drop_piece(g, 2);
  drop_piece(g, 3);
  drop_piece(g, 3);
  drop_piece(g, 3);
  drop_piece(g, 1);
  drop_piece(g, 0);
  disarray(g);
  cell target1[4][4] = {
    {EMPTY, EMPTY, EMPTY, BLACK},
    {EMPTY, EMPTY, EMPTY, BLACK},
    {EMPTY, EMPTY, EMPTY, WHITE},
    {BLACK, WHITE, WHITE, BLACK}
  };
  for (i = 0; i < g->b->height; i++) {
    p.r = i;
    for (j = 0; j < g->b->width; j++) {
      p.c = j;
      cr_assert(target1[i][j] == board_get(g->b, p));
    }
  }
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 1);
  drop_piece(g, 0);
  cell target2[4][4] = {
    {EMPTY, EMPTY, EMPTY, BLACK},
    {EMPTY, WHITE, EMPTY, WHITE},
    {BLACK, WHITE, WHITE, BLACK},
    {WHITE, BLACK, BLACK, BLACK}
  };
  disarray(g);
  cr_assert(g->black_queue->len == 6);
  cr_assert(g->white_queue->len == 5);
  for (i = 0; i < g->b->height; i++) {
    p.r = i;
    for (j = 0; j < g->b->width; j++) {
      p.c = j;
      cr_assert(target2[i][j] == board_get(g->b, p));
    }
  }
  cr_assert(g->player == WHITES_TURN);
  game_free(g);
}

Test(offset, test_offset_empty) {
  game* g = new_game(4, 3, 6, MATRIX);
  drop_piece(g, 1);
  cr_assert(!offset(g));
  cr_assert(g->player == WHITES_TURN);
  game_free(g);
}

Test(offset, test_offset1) {
  game* g = new_game(3, 4, 4, MATRIX);
  unsigned int i, j;
  pos p;
  drop_piece(g, 3);
  drop_piece(g, 3);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 1);
  cr_assert(offset(g));
  cell target[4][4] = {
    {EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY},
    {BLACK, WHITE, EMPTY, BLACK}
  };
  for (i = 0; i < 4; i++) {
    p.r = i;
    for (j = 0; j < 4; j++) {
      p.c = j;
      cr_assert(target[i][j] == board_get(g->b, p));
    }
  }
  cr_assert(g->black_queue->len == 2);
  cr_assert(g->white_queue->len == 1);
  pq_entry *q_black = g->black_queue->head, *q_white = g->white_queue->head;
  while (q_black) {
    cr_assert(board_get(g->b, q_black->p) == BLACK);
    q_black = q_black->next;
  }
  while (q_white) {
    cr_assert(board_get(g->b, q_white->p) == WHITE);
    q_white = q_white->next;
  }
  cr_assert(g->player == BLACKS_TURN);
  game_free(g);
}

Test(offset, test_offset2) {
  game* g = new_game(4, 5, 5, BITS);
  unsigned int i, j;
  pos p;
  drop_piece(g, 4);
  drop_piece(g, 3);
  drop_piece(g, 2);
  drop_piece(g, 1);
  drop_piece(g, 3);
  drop_piece(g, 3);
  drop_piece(g, 4);
  drop_piece(g, 2);
  cr_assert(offset(g));
  cell target1[5][5] = {
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, WHITE, EMPTY},
    {EMPTY, EMPTY, EMPTY, BLACK, EMPTY},
    {EMPTY, WHITE, BLACK, WHITE, BLACK}
  };
  for (i = 0; i < g->b->height; i++) {
    p.r = i;
    for (j = 0; j < g->b->width; j++) {
      p.c = j;
      cr_assert(target1[i][j] == board_get(g->b, p));
    }
  }
  cr_assert(g->player == WHITES_TURN);
  pq_entry *q_black = g->black_queue->head, *q_white = g->white_queue->head;
  while (q_black) {
    cr_assert(board_get(g->b, q_black->p) == BLACK);
    q_black = q_black->next;
  }
  while (q_white) {
    cr_assert(board_get(g->b, q_white->p) == WHITE);
    q_white = q_white->next;
  }
  drop_piece(g, 2);
  drop_piece(g, 3);
  drop_piece(g, 1);
  offset(g);
  cr_assert(g->black_queue->len == 3);
  cr_assert(g->white_queue->len == 4);
  cell target2[5][5] = {
    {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY, BLACK, EMPTY},
    {EMPTY, EMPTY, EMPTY, WHITE, EMPTY},
    {EMPTY, EMPTY, EMPTY, BLACK, EMPTY},
    {EMPTY, WHITE, WHITE, WHITE, BLACK}
  };
  for (i = 0; i < g->b->height; i++) {
    p.r = i;
    for (j = 0; j < g->b->width; j++) {
      p.c = j;
      cr_assert(target2[i][j] == board_get(g->b, p));
    }
  }
  q_black = g->black_queue->head;
  q_white = g->white_queue->head;
  while (q_black) {
    cr_assert(board_get(g->b, q_black->p) == BLACK);
    q_black = q_black->next;
  }
  while (q_white) {
    cr_assert(board_get(g->b, q_white->p) == WHITE);
    q_white = q_white->next;
  }
  cr_assert(g->player == WHITES_TURN);
  game_free(g);
}

Test(game_outcome, test_new_game_outcome) {
  game* g = new_game(8, 8, 8, MATRIX);
  cr_assert(!game_outcome(g));
  game_free(g);
}

Test(game_outcome, test_white_win_horizontal) {
  game* g = new_game(7, 7, 5, MATRIX);
  unsigned int j;
  for (j = 0; j < g->b->width; j++) {
    drop_piece(g, j);
  }
  for (j = 0; j < g->b->width; j++) {
    drop_piece(g, j);
    if (j == g->b->width - 1) {
      break;
    }
    drop_piece(g, j);
  }
  cr_assert(game_outcome(g) == WHITE_WIN);
  game_free(g);
}

Test(game_outcome, test_in_progress) {
  game* g = new_game(4, 8, 8, BITS);
  while (drop_piece(g, 4)) {
    // Intentionally empty
  }
  cr_assert(!game_outcome(g));
  game_free(g);
}

Test(game_outcome, test_black_win_vertical) {
  game* g = new_game(4, 6, 6, MATRIX);
  unsigned int j, n;
  for (n = 0; n < 2; n++) {
    for (j = 0; j < g->b->width; j++) {
      drop_piece(g, j);
    }
  }
  for (n = 0; n < 4; n++) {
    drop_piece(g, 1);
    if (n == 3) {
      break;
    }
    drop_piece(g, 2);
  }
  cr_assert(game_outcome(g) == BLACK_WIN);
  game_free(g);
}

Test(game_outcome, test_black_win_up_right_diagonal) {
  game* g = new_game(2, 2, 2, BITS);
  drop_piece(g, 0);
  drop_piece(g, 1);
  drop_piece(g, 0);
  cr_assert(game_outcome(g) == BLACK_WIN);
  game_free(g);
}

Test(game_outcome, test_draw_horizontal) {
  game* g = new_game(4, 3, 4, MATRIX);
  for (int j = 0; j < g->b->width; j++) {
    while (drop_piece(g, j)) {
      // Intentionally empty loop
    }
  }
  cr_assert(game_outcome(g) == DRAW);
  game_free(g);
}

Test(game_outcome, test_draw_diagonal) {
  game* g = new_game(5, 5, 5, MATRIX);
  for (int j = 0; j < g->b->width; j++) {
    while (drop_piece(g, j)) {
      // Intentionally empty loop
    }
  }
  cr_assert(game_outcome(g) == DRAW);
  game_free(g);
}
