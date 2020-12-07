CC = gcc
CFLAGS = -std=c99 -Wall -Wconversion -Werror
FLAGS_VALGRIND = --leak-check=full --track-origins=yes --show-reachable=yes 

evento_pesca: pesca.c evento_pesca.c evento_pesca.h
	$(CC)  *c -o evento_pesca $(CFLAGS)

.PHONY : clean
clean :
	-rm evento_pesca

debug :
	$(CC)  *c -g -o evento_pesca $(CFLAGS)
	gdb ./evento_pesca

valgrind:
	$(CC)  *c -g -o evento_pesca $(CFLAGS)
	valgrind $(FLAGS_VALGRIND) ./evento_pesca