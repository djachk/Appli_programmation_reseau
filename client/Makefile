CC=gcc
CFLAGS= -Wall -g
EXEC=run

all: $(EXEC)

run: client.o gestion_client.o protocole.o connexion.o
	$(CC)  -o  $@ $^

connexion.o: protocole.h

client.o: gestion_client.h connexion.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
