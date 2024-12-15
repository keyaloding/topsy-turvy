#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "pos.h"
#include "board.h"
#include "logic.h"

game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type) {
  if (run > width && run > height) {
    fprintf(stderr, "Error: Run length must be less than height and/or less "
                    "than width\n");
    exit(1);
  }
  if (!run || !width || !height) {
    fprintf(stderr, "Error: Run length, width, and height must be "
                    "greater than 0\n");
    exit(1);
  }
  game* g = (game*)malloc(sizeof(game));
  if (!g) {
    fprintf(stderr, "Error: Memory allocation error\n");
    exit(1);
  }
  g->b = board_new(width, height, type);
  g->black_queue = posqueue_new();
  g->white_queue = posqueue_new();
  g->player = BLACKS_TURN;
  g->run = run;
  return g;
}

void game_free(game* g) {
  board_free(g->b);
  posqueue_free(g->black_queue);
  posqueue_free(g->white_queue);
  free(g);
}

bool drop_piece(game* g, unsigned int column) {
  pos p = make_pos(0, column);
  if (board_get(g->b, p)) {
    return false;
  }
  p.r = g->b->height - 1;
  while (board_get(g->b, p)) {
    p.r--;
  }
  if (g->player == BLACKS_TURN) {
    board_set(g->b, p, BLACK);
    pos_enqueue(g->black_queue, p);
    g->player = WHITES_TURN;
  } else {
    board_set(g->b, p, WHITE);
    pos_enqueue(g->white_queue, p);
    g->player = BLACKS_TURN;
  }
  return true;
}

/* Applies gravity to a column. Note: This function only works if all of
 * the empty spaces are at the bottom. */
void apply_gravity(game* g, unsigned int col) {
  pos p1, p2;
  p1.c = col;
  p2.c = col;
  cell curr_cell;
  unsigned height = g->b->height, num_spaces = 0;
  int i;
  for (i = height - 1; i >= 0; i--) {
    p1.r = i;
    if (board_get(g->b, p1)) {
      break;
    }
    num_spaces++;
  }
  if (!num_spaces) {
    return;
  }
  for (i = height - 1; i >= 0; i--) {
    p1.r = i;
    if (board_get(g->b, p1)) {
      break;
    }
    p2.r = i - num_spaces;
    if (p2.r < 0 || p2.r >= height) {
      break;
    }
    curr_cell = board_get(g->b, p2);
    board_set(g->b, p1, curr_cell);
    board_set(g->b, p2, EMPTY);
  }
  pq_entry *q_black = g->black_queue->head, *q_white = g->white_queue->head;
  while (q_black) {
    if (q_black->p.c == col) {
      q_black->p.r += num_spaces;
    }
    q_black = q_black->next;
  }
  while (q_white) {
    if (q_white->p.c == col) {
      q_white->p.r += num_spaces;
    }
    q_white = q_white->next;
  }
}

/* Struct for pthread arguments.*/
struct thread_args {
  game* g;
  unsigned int col;
};

/* Performs `disarray` on a single column. */
void* col_disarray(void* a) {
  struct thread_args* thread_args = (struct thread_args*)a;
  game* g = thread_args->g;
  unsigned int i, col = thread_args->col, height = g->b->height;
  pos p1, p2;
  p1.c = col;
  p2.c = col;
  cell temp_cell;
  for (i = 0; i < height / 2; i++) {
    p1.r = i;
    p2.r = height - 1 - i;
    temp_cell = board_get(g->b, p1);
    board_set(g->b, p1, board_get(g->b, p2));
    board_set(g->b, p2, temp_cell);
  }
  pq_entry *q_black = g->black_queue->head, *q_white = g->white_queue->head;
  while (q_black) {
    if (q_black->p.c == col) {
      q_black->p.r = height - 1 - q_black->p.r;
    }
    q_black = q_black->next;
  }
  while (q_white) {
    if (q_white->p.c == col) {
      q_white->p.r = height - 1 - q_white->p.r;
    }
    q_white = q_white->next;
  }
  apply_gravity(g, col);
  return NULL;
}

void disarray(game* g) {
  if (g->player == BLACKS_TURN) {
    g->player = WHITES_TURN;
  } else {
    g->player = BLACKS_TURN;
  }
  if (!g->black_queue->len || !g->white_queue->len) {
    return;
  }
  unsigned int i, j, width = g->b->width, height = g->b->height;
  if (g->b->type == MATRIX) {
    pthread_t threads[width];
    struct thread_args thread_args[width];
    for (i = 0; i < width; i++) {
      thread_args[i].g = g;
      thread_args[i].col = i;
      pthread_create(&threads[i], NULL, col_disarray, &thread_args[i]);
    }
    for (i = 0; i < width; i++) {
      pthread_join(threads[i], NULL);
    }
  } else {
    cell temp_cell;
    pos p1, p2;
    for (i = 0; i < height / 2; i++) {
      p1.r = i;
      p2.r = height - 1 - i;
      for (j = 0; j < width; j++) {
        p1.c = j;
        p2.c = j;
        temp_cell = board_get(g->b, p1);
        board_set(g->b, p1, board_get(g->b, p2));
        board_set(g->b, p2, temp_cell);
      }
    }
    pq_entry *q_black = g->black_queue->head, *q_white = g->white_queue->head;
    while (q_black) {
      q_black->p.r = height - 1 - q_black->p.r;
      q_black = q_black->next;
    }
    while (q_white) {
      q_white->p.r = height - 1 - q_white->p.r;
      q_white = q_white->next;
    }
    for (j = 0; j < width; j++) {
      apply_gravity(g, j);
    }
  }
}

bool offset(game* g) {
  if (!g->black_queue->len || !g->white_queue->len) {
    return false;
  }
  pos p1, p2;
  if (g->player == BLACKS_TURN) {
    p1 = pos_dequeue(g->black_queue);
    board_set(g->b, posqueue_remback(g->white_queue), EMPTY);
    g->player = WHITES_TURN;
  } else {
    board_set(g->b, posqueue_remback(g->black_queue), EMPTY);
    p1 = pos_dequeue(g->white_queue);
    g->player = BLACKS_TURN;
  }

  board_set(g->b, p1, EMPTY);
  p2 = make_pos(p1.r - 1, p1.c);
  cell color;
  unsigned int original_row = p1.r;
  while (p1.r > 0) {
    color = board_get(g->b, p2);
    board_set(g->b, p1, color);
    board_set(g->b, p2, EMPTY);
    p1.r--;
    p2.r--;
  }
  pq_entry *q_black = g->black_queue->head, *q_white = g->white_queue->head;
  while (q_black) {
    if (q_black->p.c == p1.c && q_black->p.r < original_row) {
      q_black->p.r++;
    }
    q_black = q_black->next;
  }
  while (q_white) {
    if (q_white->p.c == p1.c && q_white->p.r < original_row) {
      q_white->p.r++;
    }
    q_white = q_white->next;
  }
  return true;
}

/* Performs a depth-first search from the specified position to determine
 * if there is a winner. */
outcome dfs(game* g, pos p) {
  cell color = board_get(g->b, p);
  pos p1 = p, p2;
  unsigned int i, width = g->b->width, height = g->b->height, run_len;
  if (g->black_queue->len < g->run && g->white_queue->len < g->run) {
    return IN_PROGRESS;
  }
  char directions[8][2] = {
    {0, -1}, // Left
    {0, 1}, // Right
    {-1, 0}, // Up
    {1, 0}, // Down
    {-1, -1}, // Up + Left
    {-1, 1}, // Up + Right
    {1, -1}, // Down + Left
    {1, 1} // Down + Right
  };
  for (i = 0; i < 8; i++) {
    run_len = 0;
    p2 = make_pos(p1.r, p1.c);
    while (p2.r < height && p2.c < width) {
      if (board_get(g->b, p2) == color) {
        run_len++;
        p2.r += directions[i][0];
        p2.c += directions[i][1];
      } else {
        break;
      }
    }
    if (run_len >= g->run) {
      if (color == BLACK) {
        return BLACK_WIN;
      } else {
        return WHITE_WIN;
      }
    }
  }
  return IN_PROGRESS;
}

outcome game_outcome(game* g) {
  unsigned int j, width = g->b->width, height = g->b->height, empty_count;
  int i;
  // `i` is signed to avoid overflow when iterating over rows backwards
  pos p;
  if (g->black_queue->len < g->run && g->white_queue->len < g->run) {
    return IN_PROGRESS;
  }
  bool black_win = false, white_win = false;
  for (i = height - 1; i >= 0; i--) {
    p.r = i;
    empty_count = 0; // number of empty cells in the row
    for (j = 0; j < width; j++) {
      p.c = j;
      if (!board_get(g->b, p)) {
        empty_count++;
        continue;
      }
      // No cells above if the current row is empty
      if (empty_count == width) {
        return IN_PROGRESS;
      }
      outcome status = dfs(g, p);
      if (status == BLACK_WIN) {
        black_win = true;
      } else if (status == WHITE_WIN) {
        white_win = true;
      }
      if (black_win && white_win) {
        return DRAW;
      }
    }
  }
  if (g->black_queue->len + g->white_queue->len == width * height) {
    return DRAW;
  } else if (black_win) {
    return BLACK_WIN;
  } else if (white_win) {
    return WHITE_WIN;
  } else {
    return IN_PROGRESS;
  }
}
