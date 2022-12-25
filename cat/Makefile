all: s21_cat

s21_cat: s21_cat.o
	gcc -Wall -Werror -Wextra -std=c11 -pedantic s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	gcc -Wall -Werror -Wextra -std=c11 -pedantic -c s21_cat.c

clean:
	rm -rf *.o *.out s21_cat
rebuild: clean all