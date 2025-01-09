#include <stdlib.h>
#include <stdio.h>

#include "../headers/board.h"

board* board_new(unsigned int width, unsigned int height, enum type type) {
  board* b = (board*)malloc(sizeof(board));
  if (!b) {
    fprintf(stderr, "Error: Memory allocation error\n");
    exit(1);
  }
  b->type = type;
  if (b->type == MATRIX) {
    cell** matrix = (cell**)calloc(height, sizeof(cell*));
    if (!matrix) {
      fprintf(stderr, "Error: Memory allocation error\n");
      exit(1);
    }
    for (unsigned int i = 0; i < height; i++) {
      matrix[i] = (cell*)calloc(width, sizeof(cell));
      if (!matrix[i]) {
        fprintf(stderr, "Error: Memory allocation error\n");
        exit(1);
      }
    }
    b->u.matrix = matrix;
  } else {
    unsigned int len = width * height / 16;
    if (width * height % 16) {
      len++;
    }
    unsigned int* bits = (unsigned int*)calloc(len, sizeof(unsigned int));
    if (!bits) {
      fprintf(stderr, "Error: Memory allocation error\n");
      exit(1);
    }
    b->u.bits = bits;
  }
  b->width = width;
  b->height = height;
  return b;
}

void board_free(board* b) {
  unsigned int i;
  if (b->type == MATRIX) {
    for (i = 0; i < b->height; i++) {
      if (b->u.matrix[i]) {
        free(b->u.matrix[i]);
      }
    }
    free(b->u.matrix);
  } else {
    free(b->u.bits);
  }
  free(b);
}

void board_show(board* b) {
  unsigned int i, j;
  pos p;
  fprintf(stdout, "\n  ");
  for (j = 0; j < b->width; j++) {
    if (j < 10) {
      fprintf(stdout, "%u", j);
    } else if (j < 36) {
      fprintf(stdout, "%c", '7' + j);
    } else if (j < 62) {
      fprintf(stdout, "%c", '=' + j);
    } else {
      fprintf(stdout, "?");
    }
  }
  fprintf(stdout, "\n");
  for (i = 0; i < b->height; i++) {
    if (i < 10) {
      fprintf(stdout, "%u ", i);
    } else if (i < 36) {
      fprintf(stdout, "%c ", '7' + i);
    } else if (i < 62) {
      fprintf(stdout, "%c ", '=' + i);
    } else {
      fprintf(stdout, "? ");
    }
    p.r = i;
    for (j = 0; j < b->width; j++) {
      p.c = j;
      switch (board_get(b, p)) {
        case EMPTY:
          fprintf(stdout, ".");
          break;
        case BLACK:
          fprintf(stdout, "*");
          break;
        case WHITE:
          fprintf(stdout, "o");
          break;
      }
    }
    fprintf(stdout, " %u\n", i);
  }
  fprintf(stdout, "  ");
  for (j = 0; j < b->width; j++) {
    fprintf(stdout, "-");
  }
  fprintf(stdout, "\n  ");
  for (j = 0; j < b->width; j++) {
    fprintf(stdout, "%u", j);
  }
  fprintf(stdout, "\n\n");
}

cell board_get(board* b, pos p) {
  unsigned int width = b->width, height = b->height;
  if (p.r >= height || p.c >= width) {
    fprintf(stderr, "Error: Position (%u, %u) is out of bounds\n", p.r, p.c);
    exit(1);
  }
  if (b->type == MATRIX) {
    return b->u.matrix[p.r][p.c];
  } else {
    unsigned int bit_index = (p.r * width + p.c) * 2;
    unsigned int val = (b->u.bits[bit_index / 32] >> (bit_index % 32)) & 0x3;
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

void board_set(board* b, pos p, cell c) {
  unsigned int width = b->width, height = b->height;
  if (p.r >= height || p.c >= width) {
    fprintf(stderr, "Error: Position (%u, %u) is out of bounds\n", p.r, p.c);
    exit(1);
  }
  if (b->type == MATRIX) {
    b->u.matrix[p.r][p.c] = c;
  } else {
    unsigned int bit_index = (p.r * width + p.c) * 2, bit_val;
    b->u.bits[bit_index / 32] &= ~(0x3 << (bit_index % 32));
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
    b->u.bits[bit_index / 32] |= (bit_val & 0x3) << (bit_index % 32);
  }
}
