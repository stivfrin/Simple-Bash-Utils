CC = gcc -Wall -Werror -Wextra -std=c11 -o s21_grep
SOURCE = s21_grep.c

all: s21_grep

s21_grep:
	$(CC) $(SOURCE)
clean:
	rm s21_grep
test:
	sh tests.sh | grep -i fail

rebuild: clean s21_grep