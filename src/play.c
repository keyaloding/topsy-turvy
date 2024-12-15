#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../headers/board.h"
#include "../headers/logic.h"

/* Raises an error indicating that the game could not be started because of
 * incorrect command-line arguments. */
void raise_game_init_error() {
  fprintf(stderr, "Error: Invalid command-line argument format. Run:\n"
          "./play -h x -w y -r z\n" 
          "'x', 'y', and 'z' should be unsigned integers representing the "
          "height, width, and winning run length of the game, respectively.\n");
  exit(1);
}

/* Returns a boolean indicating whether a string is a valid unsigned int. */
bool is_unsigned_int(char* s) {
  if (!s[0]) {
    return false;
  }
  unsigned int i = 0;
  while (s[i]) {
    if (s[i] < '0' || s[i] > '9') {
      return false;
    }
    i++;
  }
  return true;
}

/* Initializes a game using the command line arguments. Exits the program if
 * the game cannot be configured. */
game* game_init(int argc, char** argv) {
  if (argc < 7) {
    raise_game_init_error();
  }
  unsigned int i, h = 0, w = 0, r = 0;
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h")) {
      if (i + 1 == argc || h || !is_unsigned_int(argv[i + 1])) {
        raise_game_init_error();
      }
      h = atoi(argv[i + 1]);
    } else if (!strcmp(argv[i], "-w")) {
      if (i + 1 == argc || w || !is_unsigned_int(argv[i + 1])) {
        raise_game_init_error();
      }
      w = atoi(argv[i + 1]);
    } else if (!strcmp(argv[i], "-r")) {
      if (i + 1 == argc || r) {
        raise_game_init_error();
      }
      r = atoi(argv[i + 1]);
    } else if (is_unsigned_int(argv[i])) {
      if (!strcmp(argv[i - 1], "-h") || !strcmp(argv[i - 1], "-w") ||
          !strcmp(argv[i - 1], "-r") ) {
        continue;
      } else {
        raise_game_init_error();
      }
    } else {
      raise_game_init_error();
    }
  }
  if (!h || !w || !r) {
    raise_game_init_error();
  }
  game* g = new_game(r, w, h, BITS);
  return g;
}

/* Performs a player move and returns the resulting game outcome. */
outcome play_move(game* g, char move) {
  unsigned int col;
  if (move == '!') {
    disarray(g);
    return game_outcome(g);
  } else if (move == '^') {
    if (!offset(g)) {
      fprintf(stdout, "Invalid move: Both players must have at least one piece "
             "on the board to perform an offset move\n");
      return IN_PROGRESS;
    }
    return game_outcome(g);
  } else if (move >= '0' && move <= '9') {
    col = move - '0';
  } else if (move >= 'A' && move <= 'Z') {
    col = move - '7';
  } else if (move >= 'a' && move <= 'z') {
    col = move - '=';
  } else if (move == '?') {
    fprintf(stdout, "Invalid move: Pieces cannot be dropped in '?' columns\n");
    return IN_PROGRESS;
  } else if (move == '.') {
    if (g->player == BLACKS_TURN) {
      return WHITE_WIN;
    } else {
      return BLACK_WIN;
    }
  } else {
    fprintf(stdout, "'%c' (ASCII: %d) is an invalid move\n"
           "To drop a piece, enter the column name (1-9, A-Z, a-z)\n"
           "To perform a disarray move, enter '!'\n"
           "To perform an offset move, enter '^'\n", move, move);
    return IN_PROGRESS;
  }
  if (col >= g->b->width) {
    fprintf(stdout, "Invalid move: Column %u is out of bounds.\n", col);
    return IN_PROGRESS;
  }
  if (!drop_piece(g, col)) {
    fprintf(stdout, "Invalid move: Column %u is full.\n", col);
  }
  return game_outcome(g);
}

int main(int argc, char** argv) {
  game* g = game_init(argc, argv);
  outcome status = IN_PROGRESS;
  while (status == IN_PROGRESS) {
    board_show(g->b);
    char move;
    if (g->player == BLACKS_TURN) {
      fprintf(stdout, "Black: ");
    } else {
      fprintf(stdout, "White: ");
    }
    scanf("%c%*c", &move);
    status = play_move(g, move);
    if (status != IN_PROGRESS) {
      board_show(g->b);
      if (status == BLACK_WIN) {
        fprintf(stdout, "Black wins!\n");
      } else if (status == WHITE_WIN) {
        fprintf(stdout, "White wins!\n");
      } else {
        fprintf(stdout, "Draw!\n");
      }
    }
  }
  game_free(g);
  return 0;
}
