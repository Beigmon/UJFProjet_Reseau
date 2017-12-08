/**
 * \file Serveur.c
 * \brief Le serveur
 * \author Florian PIGNARD & Jonathan MONBEIG & Mathieu CLUSEL & Louis REYNAUD 
 * \version 0.1
 * \date 4 décembre 2017
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
			return EXIT_FAILURE;
		}
		
		fils = fork();
		switch(fils)
		{
			case 0:
			{
				write(serv_sock, "Bonjour, bienvenue sur le serveur.\n", strlen("Bonjour, bienvenue sur le serveur.\n"));
				
				while(1)
				{
					char *message = malloc(MAX * sizeof(char));
					
					//ON RECOIT LA CHAINE DU CLIENT
					int nbLus = read(serv_sock, message, MAX);
					
					//SI ON LIT RIEN
					if(nbLus == -1)
					{
						return -1;
					}	
					
					if(strcmp(message, "KILL") == 0)
					{
						break;
					}
					
					//DECLARATION DES VARIABLES DE STOCKAGE
					char *villeDepartRecu = malloc(MAX * sizeof(char));
					char *villeArriveRecu = malloc(MAX * sizeof(char));
					char *HoraireDebutRecu = malloc(MAX * sizeof(char));
					char *HoraireFinRecu = malloc(MAX * sizeof(char));
					char *choixTri = malloc(MAX * sizeof(char));
					int nbTrains = 0; 
					
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
					
					if(nbTrains != 0 && ListTrains[0].num_train != 0)
					{
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
						strcpy(message, "");  // VIDE la chaine
						char trainSousText[MAX];
						for(index = 0; index < nbTrains; index++)
						{
							structureVersTxt(ListTrains[index], trainSousText);
							strcat(message, trainSousText);
						}
						write(serv_sock, message, strlen(message)-1);
					}
					else
					{
						// ERREUR rien à renvoyer
						write(serv_sock, "Aucun train trouvé.;Inconnu;Inconnu;Inconnu;Inconnu;Inconnu", strlen("Aucun train trouvé.;Inconnu;Inconnu;Inconnu;Inconnu;Inconnu"));
					}
					strcpy(message, "");  // VIDE la chaine
					free(message); free(villeDepartRecu); free(villeArriveRecu); free(HoraireDebutRecu); free(HoraireFinRecu); free(choixTri);
				}
				
				return EXIT_SUCCESS;
			}
			
			//LE PÈRE
			default:
			
				break;
		}
		
	}
	close(numSocket);
	return 0;
}
