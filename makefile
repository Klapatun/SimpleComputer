
run: prog
	./prog

prog:
	$(CC) -w *.c -o prog

clean:
	rm prog
