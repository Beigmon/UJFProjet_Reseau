/**
 * \file Serveur.c
 * \brief ....
 * \author ....
 * \version 0.1
 * \date 4 décembre 2017
 *
 * (exemeple) Programme de test pour l'objet de gestion des chaînes de caractères Str_t.
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
 * \fn int main (void)
 * \brief Entrée du programme.
 *
 * \return EXIT_SUCCESS - Arrêt normal du programme.
 */
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
				int nbLus = read(serv_sock, message, MAX);
				
				//SI ON LIT RIEN
				if(nbLus == -1)
				{
					return -1;
				}	
				
				//DECLARATION DES VARIABLES DE STOCKAGE
				char villeDepartRecu[MAX];
				char villeArriveRecu[MAX];
				char HoraireDebutRecu[MAX];
				char HoraireFinRecu[MAX];
				char choixTri[MAX];
				int nbTrains; 
				
				//FAIRE LA DECOMPOSITION DE LA CHAINE
				strcpy(villeDepartRecu,strtok(message, ";"));
				strcpy(villeArriveRecu,strtok(NULL, ";"));
				strcpy(HoraireDebutRecu,strtok(NULL, ";"));
				strcpy(HoraireFinRecu,strtok(NULL, ";"));
				strcpy(choixTri,strtok(NULL, ";"));
				
				struct trains *ListTrains =  malloc(nbLignes * sizeof(struct trains));	
				//IDENTIFICATION DE LA CHAINE RECU
				if(atoi(HoraireFinRecu) == 0)
				{
					//IDENTIFICATION DE LA CHAINE RECU
					if(atoi(HoraireDebutRecu) == 0)
					{
						//ON EST DANS LA TROISIEME POSSIBILITÉ (Ville de départ et d'arrivée renseignées)															
						nbTrains = getAllTrainsWithStartAndArrival(villeDepartRecu, villeArriveRecu, ListTrains, donnees, nbLignes);
					}
					else
					{
						//ON EST DANS LE PREMIER POSSIBILITÉ (Ville de départ, d'arrivée renseignées ainsi qu'une heure de départ)														
						getOneTrainWithStartArrivalAndTime(villeDepartRecu, villeArriveRecu, HoraireDebutRecu, ListTrains, donnees, nbLignes);
						nbTrains = 1; 
					}
				}
				else
				{
					//ON EST DANS LA DEUXIÈME POSSIBILITÉ (Ville de départ, d'arrivée renseignées ainsi qu'une heure de départ et d'arrivée)																
					nbTrains = getTrainsWithTimePeriods(villeDepartRecu, villeArriveRecu, HoraireDebutRecu, HoraireFinRecu, ListTrains, donnees, nbLignes);
				}
				
				//LE TRI
				switch(atoi(choixTri))
				{
					//TRI PAR DURÉE DE TRAJET
					case 1:
						trierTrains(ListTrains, nbTrains, "temps");
						break;
					//TRI PAR PRIX
					case 2:
						trierTrains(ListTrains, nbTrains, "prix");
						break;
					//AUCUN TRI
					default:
						break;
				}
								
				int index = 0;
				char * trainSousText = malloc(MAX * sizeof(char));		
				while(ListTrains[index].num_train != 0)
				{
					structureVersTxt(ListTrains[index], trainSousText);
					strcat(resultat, trainSousText);
					index++;
				}
				
				write(serv_sock, resultat, strlen(resultat));
				
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

