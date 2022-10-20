sudoku: main.o sudokucom.o
	cc -osudoku -lcurl $^

.PHONY: clean

clean:
	rm -rf *.o sudoku
