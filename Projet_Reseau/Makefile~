CC = gcc -Wall -c
CO = gcc -Wall -o
SERV = serveur
CLI = client
RESC = /sources/Reseau.c
RESH = /headers/Reseau.h
OBJS = serveur.o Client.o Reseau_client.o Reseau_serveur.o
EXCCLI = /bin/Client
EXCSERV = /bin/Serveur
OBJSSERV = $(SERV)/Serveur.c  $(SERV)$(RESH)
OBJSCLI = $(CLI)/Client.c $(CLI)$(RESH)

all : Client Serveur
 
Serveur : Serveur.o Reseau_serveur.o
	$(CO) $(SERV)$(EXCSERV) Serveur.o Reseau.o

Client : Client.o Reseau_client.o
	$(CO) $(CLI)$(EXCCLI) Client.o Reseau.o

Serveur.o : $(OBJSSERV)
	$(CC) $(SERV)/Serveur.c

Client.o : $(OBJSCLI)
	$(CC) $(CLI)/Client.c
 
Reseau_client.o : $(CLI)$(RESC) $(CLI)$(RESH)
	$(CC) $(CLI)$(RESC)

Reseau_serveur.o : $(SERV)$(RESC) $(SERV)$(RESH)
	$(CC) $(SERV)$(RESC)

clean :
	rm -f $(OBJS) $(EXC)
