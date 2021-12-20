CC=gcc
CFLAGS= -Wall -g
EXEC=promoteur

all: $(EXEC)

promoteur: promoteur.o
	$(CC)  -o  $@ $^

promoteur.o: promoteur.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
