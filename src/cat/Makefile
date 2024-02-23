CC = gcc -Wall -Werror -Wextra -std=c11 -o s21_cat
SOURCE = s21_cat.c

all: s21_cat

s21_cat:
	$(CC) $(SOURCE)
clean:
	rm s21_cat
test:
	sh tests.sh | grep -i fail

rebuild: clean s21_cat