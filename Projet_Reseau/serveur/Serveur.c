/*
 * Serveur.c
 * 
 * Copyright 2017 Jonathan Monbeig <monbeigj@ltsp26>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "headers/Reseau.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAX 512
#define CHOIX 1

/**
struct in_addr { u_long s_addr; } ;

struct sockaddr_in
{ 	short sin_family;   lui donner la valeur AF_INET 
	u_short sin_port;   numéro de port
	struct in_addr sin_addr;   adresse de la machine
	char sin_zero[8];  champ rempli de 0
} ;
 
int socket(int domaine, int type, int protocole);
 
int bind(int descripteur, const struct sockaddr *p, int len);+

**/
typedef enum { REDUC, SUPPL } Action_prix;

struct trains
{
	int num_train;
	char ville_depart[MAX];
	char ville_arrivee[MAX];
	char horaire_depart[MAX];
	char horaire_arrivee[MAX];
	double prix_usuel;
	Action_prix evenement;	 
} ;


int main(int argc, char **argv)
{
	//char filename[MAX] = argv[2];
	//FILE *fichier_train = fopen(filename, "r");
	
	int noport = atoi(argv[1]);
	int serv_sock;
	char message[MAX];
	int nb_lu;
	pid_t fils;
	int p;
	/*
	struct sockaddr_in s;
	int p = socket(AF_INET, SOCK_STREAM, 0);
	s.sin_family = AF_INET;
	s.sin_addr.s_addr = htonl(INADDR_ANY);
	s.sin_port = htons(noport);
	if(bind( p, (struct sockaddr *) &s, sizeof(s))<0)
	{
		printf("Il y a eu une erreur bind");
	}
	printf("serveur prêt\n");
	if(listen( p, 5)<0)
	{
		printf("Il y a eu un erreur listen");
	}*/
	p = socketServer(noport, TCP);
	while(1)
	{
		serv_sock = accept(p, NULL, NULL);
		if(serv_sock<0)
		{
			printf("Il y a eu un erreur accept");
		}
		fils = fork();
		switch(fils)
		{
			case 0:
			{
				sprintf(message,"Bonjour, bienvenue sur le serveur.\nSouhaitez-vous consulter les trains? (Oui = 1/ Non = 0)");
				write(serv_sock, message, strlen(message));
				nb_lu = read( serv_sock, message, MAX-1);
				
				//DEMANDE DE CONSULTATION TRAIN
				if(strcmp(message, "1"))
				{
					//CHOIX DU MODE DE SAISIE
					sprintf(message,"Très bien. Quels modes voulez-vous utiliser ?\n\t1.Par horaire de départ\n\t2.Par tranche d'horaire\n\t3.Sans horaires\n");
					write(serv_sock, message, strlen(message));
					
					nb_lu = read( serv_sock, message, MAX-1);
					switch(atoi(message))
					{
						case 1:
							//VILLE DE DÉPART
							sprintf(message,"Saisissez une ville de départ :");
							write(serv_sock, message, strlen(message));
							nb_lu = read( serv_sock, message, MAX-1);
							//VILLE D'ARRIVÉE
							sprintf(message,"Saisissez une ville d'arrivée");
							write(serv_sock, message, strlen(message));
							nb_lu = read( serv_sock, message, MAX-1);
							//HORAIRE DE DÉPART
							sprintf(message,"Saisissez un horaire de départ");
							write(serv_sock, message, strlen(message));
							nb_lu = read( serv_sock, message, MAX-1);
							break;
						case 2:
							sprintf(message,"Saisissez une ville de départ :");
							write(serv_sock, message, strlen(message));
							break;
						case 3:
							sprintf(message,"Saisissez une ville de départ :");
							write(serv_sock, message, strlen(message));
							sprintf(message,"Saisissez une ville d'arrivée :");
							write(serv_sock, message, strlen(message));
							break;
					}		
				
					
				}
				message[nb_lu+1] = 0;
				printf("%s", message);
				write( serv_sock, message, nb_lu);
				break;
			}
			default:
				break;
		}
		
	}
	close( p);
	return 0;
}


