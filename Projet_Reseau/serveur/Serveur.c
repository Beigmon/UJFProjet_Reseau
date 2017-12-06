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
#include "headers/Trains.h"
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

int main(int argc, char **argv)
{
	int nbLignes = nb_line_file(argv[2]);
	printf("nbLignes du document : %d\n", nbLignes);
	//struct trains donnees[nbLignes];
	struct trains *donnees = malloc(nbLignes * sizeof(struct trains));
	
	FILE *fichierTrain = fopen(argv[2], "r");
	int currentLine;
	for(currentLine = 0; currentLine < nbLignes; currentLine++)
	{
		//printf("Ligne actuel : %d\n", currentLine);
		txtVersStructure(fichierTrain, &donnees[currentLine]);
		//printf("num_train : %d, ville_depart : %s, ville_arrivee : %s, horaire_depart : %s, horaire_arrivee : %s, prix_usuel : %.2f \n\n",donnees[currentLine].num_train, donnees[currentLine].ville_depart,donnees[currentLine].ville_arrivee, donnees[currentLine].horaire_depart, donnees[currentLine].horaire_arrivee, donnees[currentLine].prix_usuel);
	}
	
	fclose(fichierTrain);
	
	int noport = atoi(argv[1]);
	int numSocket = socketServer(noport, TCP);
	pid_t fils;
	
	while(1)
	{
		int serv_sock = accept(numSocket, NULL, NULL);
		
		if(serv_sock<0)
		{
			printf("Il y a eu une erreur sur accept");
		}
		
		fils = fork();
		switch(fils)
		{
			case 0:
			{
				char message[MAX];
				char resultat[MAX];
				
				sprintf(message,"Bonjour, bienvenue sur le serveur.\n");
				write(serv_sock, message, strlen(message));
				
				//ON RECOIT LA CHAINE DU CLIENT
				int nb_lu = read(serv_sock, message, MAX);
				
				//DECLARATION DES VARIABLES DE STOCKAGE
				char villeDepartRecu[MAX];
				char villeArriveRecu[MAX];
				char HoraireDebutRecu[MAX];
				char HoraireFinRecu[MAX];
				char choixTri[MAX];
				
				//FAIRE LA DECOMPOSITION DE LA CHAINE
				strcpy(villeDepartRecu,strtok(message, ";"));
				strcpy(villeArriveRecu,strtok(NULL, ";"));
				strcpy(HoraireDebutRecu,strtok(NULL, ";"));
				strcpy(HoraireFinRecu,strtok(NULL, ";"));
				strcpy(choixTri,strtok(NULL, ";"));
				
				//IDENTIFICATION DE LA CHAINE RECU
				if(atoi(HoraireFinRecu) == 0)
				{
					//IDENTIFICATION DE LA CHAINE RECU
					if(atoi(HoraireDebutRecu) == 0)
					{
						//ON EST DANS LA TROISIEME POSSIBILITÉ (Ville de départ et d'arrivée renseignées)
						sprintf(resultat,"3333333333\n");		
					}
					else
					{
						//ON EST DANS LE PREMIER POSSIBILITÉ (Ville de départ, d'arrivée renseignées ainsi qu'une heure de départ)
						struct trains train;
						int index;
																		
						getTrain(villeDepartRecu, villeArriveRecu, HoraireDebutRecu, &train, donnees);
						
						printf("COTE LUMIERE\n");
						printf("%d\n", train.num_train);
						printf("%s\n", train.ville_depart);
						printf("%s\n", train.ville_arrivee);
						printf("%s\n", train.horaire_depart);
						printf("%s\n", train.horaire_arrivee);
						
						strcat(resultat, "Le train");
						strcat(resultat, "partant de ");
						strcat(resultat, train.ville_depart);
						strcat(resultat, "et arrivant à ");
						strcat(resultat, train.ville_arrivee);
						strcat(resultat, "partira à ");
						strcat(resultat, train.horaire_depart);
						
						
					}
				}
				else
				{
					//ON EST DANS LA DEUXIÈME POSSIBILITÉ (Ville de départ, d'arrivée renseignées ainsi qu'une heure de départ et d'arrivée)
					sprintf(resultat,"22222222222222\n");
				}	
				
				//LE TRI
				switch(atoi(choixTri))
				{
					//TRI PAR DURÉE DE TRAJET
					case 1:
						//////////////A COMPLETER//////////////
						break;
					//TRI PAR PRIX
					case 2:
						//////////////A COMPLETER//////////////
						break;
					//AUCUN TRI
					case 3:
						break;
				}				
				
				write(serv_sock, resultat, nb_lu);
				
				break;
			}
			
			//LE PÈRE
			default:
			
				break;
		}
		
	}
	close(numSocket);
	return 0;
}


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

