.PHONY: clean

play: pos.h pos.c board.h board.c logic.h logic.c play.c
	clang -Wall -g -O0 -o play pos.c board.c logic.c play.c -lpthread

test: pos.h pos.c board.h board.c logic.h logic.c test_pos.c test_board.c test_logic.c
	clang -Wall -g -O0 -o test pos.c board.c logic.c test_pos.c test_board.c test_logic.c -lpthread -lcriterion

clean:
	rm -rf test play *.o *~ *dSYM
