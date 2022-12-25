all: s21_grep

s21_grep: s21_grep.o
	gcc -Wall -Werror -Wextra -std=c11 -pedantic s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	gcc -Wall -Werror -Wextra -std=c11 -pedantic -c s21_grep.c

clean:
	rm -rf *.o *.out *.gch s21_grep
	
rebuild: clean all
