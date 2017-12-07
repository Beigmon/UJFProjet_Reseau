CC = gcc -Wall -c
CO = gcc -Wall -o
SERV = serveur
CLI = client
SOURCE = /sources
HEADER = /headers
RESC = $(SOURCE)/Reseau.c
RESH = $(HEADER)/Reseau.h
OBJS = *.o
EXCCLI = /bin/Client
EXCSERV = /bin/Serveur
OBJSSERV = $(SERV)/Serveur.c $(SERV)$(SOURCE)/*.c  $(SERV)$(RESH)
OBJSCLI = $(CLI)/Client.c $(CLI)$(SOURCE)/*.c $(CLI)$(RESH)

all : Client Serveur clean_o

Serveur : Serveur.o Reseau_serveur.o Trains.o
	$(CO) $(SERV)$(EXCSERV) Serveur.o Reseau.o Trains.o

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

Trains.o : $(SERV)$(SOURCE)/Trains.c $(SERV)$(HEADER)/Trains.h
	$(CC) $(SERV)$(SOURCE)/Trains.c

clean :
	rm -f $(OBJS) $(SERV)$(SOURCE)/$(OBJS) $(CLI)$(SOURCE)/$(OBJS) $(CLI)$(EXCCLI) $(SERV)$(EXCSERV)

clean_o :
	rm -f $(OBJS) $(SERV)/$(OBJS) $(CLI)/$(OBJS) $(SERV)$(SOURCE)/$(OBJS) $(CLI)$(SOURCE)/$(OBJS)


