/**
 * \file Client.c
 * \brief Le Client.
 * \author Florian PIGNARD & Jonathan MONBEIG & Mathieu CLUSEL & Louis REYNAUD 
 * \version 0.1
 * \date 4 décembre 2017
 */

#include "headers/Reseau.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#define MAX 512

/**
 * \fn void search(char *chaine)
 * \brief Recherche '\n' et remplace par un 0. Utilisé sur la chaine captée par fgets puisque un '\n' est présent dans cette chaine et nous devons l'enlever.
 *
 * \param char *chaine, c'est dans cette chaine que l'on va recherché un retour à la ligne afin de le remplacer par un 0.
 * \return void.
 */
void search(char *chaine)
{
    char *caractere = strchr(chaine, '\n');
    if (caractere)
    {
        *caractere = 0;
    }
}

/**
 * \fn void afficherTrains(char* listeTrains)
 * \brief Décompose, met en forme et affiche une liste de trains.
 *
 * \param char* listeTrains, contient le resultat de la requete.
 * \return void.
 */
//AFFICHAGE DES TRAINS DANS "listeTrains"
void afficherTrains(char* listeTrains)
{	
	char *trainAAfficher;
	char* saveN;
	
    trainAAfficher =  strtok_r(listeTrains, "\n", &saveN);
    printf("\n******************************************************** ");
    printf("\n*************** RÉSULTAT DE LA RECHERCHE *************** ");
    printf("\n********");
    // BOUCLE TANT QU'IL RESTE UNE LIGNE 
	while(trainAAfficher != NULL)
    {	
		//"trainAAfficher" CONTIENT LA LIGNE
		printf("\n********\tNuméro du train : %s",strtok(trainAAfficher, ";"));
		printf("\n********\tVille de départ : %s",strtok(NULL, ";"));
		printf("\n********\tVille d'arrivée : %s",strtok(NULL, ";"));
		printf("\n********\tHoraire de départ : %s",strtok(NULL, ";"));
		printf("\n********\tHoraire d'arrivée : %s",strtok(NULL, ";"));
		printf("\n********\tPrix : %s",strtok(NULL, ";"));
		printf("\n******** -----------------------------------------------");
		//RÉCUPERATION DE LA LIGNE SUIVANTE
		trainAAfficher =  strtok_r(NULL, "\n", &saveN);
    }
}


int main(int argc, char **argv)
{
	int noport = atoi(argv[2]);
	char message[MAX];
	int nbLus;
	
	char choix_consulte[MAX];
	char choix_horaire[MAX];
	char choix_tri[MAX];
	
	char ville_depart[MAX];
	char ville_arrivee[MAX];
	char horaire_debut[MAX];
	char horaire_fin[MAX];
	char construct_requete[MAX];
	char requete[MAX];
	
	//CONNEXION AU SERVEUR
	int numSocket = socketClient(argv[1], noport, TCP);
	
	//TEST DE LA CONNEXION AU SERVEUR
	nbLus = read(numSocket, message, MAX);
	message[nbLus+1] = 0;
<<<<<<< HEAD
	printf("%s", message);
	//write(1, message, strlen(message));
	
	//viderChaine(message, strlen(message)); // Vide le tableau
=======
>>>>>>> 43dd9b5792b6a961d803f9ed02351e8c421a4253
	
	//BOUCLE TANT QUE L'UTILISATEUR VEUT CONSULTER DES TRAINS
	do{
		//DEMANDE DE CONSULTATION TRAIN		
		do{
			printf("\nSouhaitez-vous consulter les trains? (Oui = 1/ Non = 0) : ");
			scanf("%s", choix_consulte);
			getchar(); //AVALE LE \N 
			//printf("Choix consulte : %s", choix_consulte);
		} while(strcmp(choix_consulte, "1") != 0 && strcmp(choix_consulte,"0") != 0);
		
		//SI L'UTILISATEUR SOUHAITE CONSULTER DES TRAINS
		if(strcmp(choix_consulte, "1") == 0)
		{	
			//DEMANDE DU MODE DE SAISIE
			do{
				printf("Quel mode de selections voulez-vous utiliser ?\n\t1.Par horaire de départ\n\t2.Par tranche d'horaire\n\t3.Sans horaires\n: ");
				scanf("%s", choix_horaire);
				getchar();
				
			}while(strcmp(choix_horaire,"1") != 0 && strcmp(choix_horaire,"2") != 0 && strcmp(choix_horaire,"3") != 0);
			
			//SAISIE DE LA VILLE DE DÉPART
			printf("Saisissez une ville de départ : ");
			fgets(ville_depart, sizeof(ville_depart), stdin);
			search(ville_depart);
						
			//SAISIE DE LA VILLE D'ARRIVÉE
			printf("Saisissez une ville d'arrivée : ");
			fgets(ville_arrivee, sizeof(ville_arrivee), stdin);
			search(ville_arrivee);
			
			//INITIALISATION DES TABLEAUX A VIDE
			strcpy(requete, "");
			strcpy(construct_requete,  "");
			
			//EN FONCTION DU CHOIX DU MODE DE SAISIE
			switch(atoi(choix_horaire))
			{
				case 1:
				
					//RÉCUPÉRATION DES DONNÉES
					printf("Saisissez un horaire de départ : ");
					scanf("%s", horaire_debut);
					
					//CONSTRUCTION DE LA REQUETE DANS REQUETE
					strcat(construct_requete, ville_depart);
					strcat(construct_requete, ";");
					strcat(construct_requete, ville_arrivee);
					strcat(construct_requete, ";");
					strcat(construct_requete, horaire_debut);
					strcat(construct_requete, ";");
					strcat(construct_requete, "0");
					
					break;
					
				case 2:
				
					//RÉCUPÉRATION DES DONNÉES
					printf("Saisissez le début de la tranche horaire : ");
					scanf("%s", horaire_debut);
					printf("Saisissez la fin de la tranche horaire : ");
					scanf("%s", horaire_fin);
					
					//CONSTRUCTION DE LA REQUETE DANS REQUETE
					strcat(construct_requete, ville_depart);
					strcat(construct_requete, ";");
					strcat(construct_requete, ville_arrivee);
					strcat(construct_requete, ";");
					strcat(construct_requete, horaire_debut);
					strcat(construct_requete, ";");
					strcat(construct_requete, horaire_fin);
					
					break;
					
				case 3:
				
					//CONSTRUCTION DE LA REQUETE DANS REQUETE
					strcat(construct_requete, ville_depart);
					strcat(construct_requete, ";");
					strcat(construct_requete, ville_arrivee);
					strcat(construct_requete, ";");
					strcat(construct_requete, "0");
					strcat(construct_requete, ";");
					strcat(construct_requete, "0");
					
					break;
			}
			
			//ON RAJOUTE LE ; APRES HORAIRE	
			strcat(construct_requete, ";");
			
			if (strcmp(choix_horaire, "1") !=0 )
			{
				//DEMANDE DE TRI
				do{
					printf("Quel mode de tri voulez-vous utiliser ?\n\t1.Par durée du trajet\n\t2.Par prix\n\t3.Sans tri\n : ");
					scanf("%s", choix_tri);
				} while(strcmp(choix_tri, "1") != 0 && strcmp(choix_tri, "2") != 0 && strcmp(choix_tri, "3") != 0);
				
				//AJOUT DU MODE DE TRI A LA REQUETE
				switch(atoi(choix_tri))
				{	
					//TRI PAR DURÉE DE TRAJET
					case 1:
						strcat(construct_requete, "1");
						break;
					//TRI PAR PRIX
					case 2:
						strcat(construct_requete, "2");
						break;
					//AUCUN TRI
					case 3:
						strcat(construct_requete, "3");
						break;
				}
			}
			else
			{
				strcat(construct_requete, "3");
			}
			
			//LA REQUETE EST STOCKÉE DANS LA VARIABLE "REQUETE"
			strcpy(requete,construct_requete);
			
			//ENVOIE DE LA REQUETE
			write(numSocket, requete, strlen(requete));
						
			//RECEPTION DES DONNEES DU SERVEUR
			nbLus = read(numSocket, message, MAX);
			//AFFICHAGE DES DONNEES REÇUES
			afficherTrains(message);
		}
	} while (strcmp(choix_consulte,"1") == 0);
	
	// FIN DE PROGRAMME
	write(numSocket, "KILL" , strlen("KILL"));
	
	close(numSocket);
	
	printf("\n************ Au revoir ! ************\n");
	
	return 0;
}


