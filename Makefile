.PHONY: clean

play: headers/pos.h src/pos.c headers/board.h src/board.c headers/logic.h src/logic.c src/play.c
	clang -Wall -g -O0 -o play src/pos.c src/board.c src/logic.c src/play.c -lpthread

test: headers/pos.h src/pos.c headers/board.h src/board.c headers/logic.h src/logic.c tests/test_pos.c tests/test_board.c tests/test_logic.c
	clang -Wall -g -O0 -o test src/pos.c src/board.c src/logic.c tests/test_pos.c tests/test_board.c tests/test_logic.c -lpthread -lcriterion

clean:
	rm -rf test play *.o *~ *dSYM
