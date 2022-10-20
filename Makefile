sudoku: main.o sudoku.o
	cc -osudoku $^

.PHONY: clean

clean:
	rm -rf *.o sudoku
