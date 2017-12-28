all: lines
lines: lines.c
	gcc -Wall lines.c -o lines
clean:
	$(RM) lines
dist:
	tar -cvf dist2.tar Makefile lines.c
