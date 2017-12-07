/**
 * \file Client.c
 * \brief ....
 * \author ....
 * \version 0.1
 * \date 5 décembre 2017
 *
 * (exemeple) Programme de test pour l'objet de gestion des chaînes de caractères Str_t.
 *
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
 * Cherche un '\n' et met un 0.
 * */
static void search(char *chaine)
{
    char *p = strchr(chaine, '\n');
    if (p)
    {
        *p = 0;
    }
}

void viderChaine(char * chaine, int lg)
{
	int i;
	for(i=0; i<lg; i++)
	{
		chaine[i] = 0;
	}
}

//AFFICHAGE DES TRAINS DANS "listeTrains"
void afficherTrains(char* listeTrains)
{	
	char *tok;
	char* saveN;
	
    tok =  strtok_r(listeTrains, "\n", &saveN);
    printf("\n******************************************************** ");
    printf("\n*************** RÉSULTAT DE LA RECHERCHE *************** ");
    // BOUCLE TANT QU'IL RESTE UNE LIGNE 
	while(tok != NULL)
    {	
		//"tok" CONTIENT LA LIGNE
		printf("\n********");
		printf("\n********\tNuméro du train : %s",strtok(tok, ";"));
		printf("\n********\tVille de départ : %s",strtok(NULL, ";"));
		printf("\n********\tVille d'arrivée : %s",strtok(NULL, ";"));
		printf("\n********\tHoraire de départ : %s",strtok(NULL, ";"));
		printf("\n********\tHoraire d'arrivée : %s",strtok(NULL, ";"));
		printf("\n********\tPrix : %s",strtok(NULL, ";"));
		printf("\n******** -----------------------------------------------\n");
		//RÉCUPERATION DE LA LIGNE SUIVANTE
		tok =  strtok_r(NULL, "\n", &saveN);
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
	write(1, message, strlen(message));
	
	//viderChaine(message, strlen(message)); // Vide le tableau
	
	//BOUCLE TANT QUE L'UTILISATEUR VEUT CONSULTER DES TRAINS
	do{
		//DEMANDE DE CONSULTATION TRAIN		
		do{
			printf("Souhaitez-vous consulter les trains? (Oui = 1/ Non = 0) : ");
			scanf("%s", choix_consulte);
			getchar(); //AVALE LE \N 
			//printf("Choix consulte : %s", choix_consulte);
		} while(strcmp(choix_consulte, "1") != 0 && strcmp(choix_consulte,"0") != 0);
		
		if(strcmp(choix_consulte, "1") == 0)
		{	
			//DEMANDE DU MODE DE SAISIE
			do{
				printf("Quel mode de selections voulez-vous utiliser ?\n\t1.Par horaire de départ\n\t2.Par tranche d'horaire\n\t3.Sans horaires\n: ");
				scanf("%s", choix_horaire);
				getchar();
				
			}while(strcmp(choix_horaire,"1") != 0 && strcmp(choix_horaire,"2") != 0 && strcmp(choix_horaire,"3") != 0);
			
			//VILLE DE DÉPART
			printf("Saisissez une ville de départ : ");
			fgets(ville_depart, sizeof(ville_depart), stdin);
			search(ville_depart);
						
			//VILLE D'ARRIVÉE
			printf("Saisissez une ville d'arrivée : ");
			fgets(ville_arrivee, sizeof(ville_arrivee), stdin);
						
			search(ville_arrivee);
			
			//INITIALISATION DES TABLEAUX A VIDE
			strcpy(requete, "");
			strcpy(construct_requete,  "");
			
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
					printf("Saisissez un horaire de départ : ");
					scanf("%s", horaire_debut);
					printf("Saisissez un horaire d'arrivée : ");
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
				
			if (strcmp(choix_horaire, "1") !=0 )
			{
				//DEMANDE DE TRI
				do{
					printf("Quel mode de tri voulez-vous utiliser ?\n\t1.Par durée du trajet\n\t2.Par prix\n\t3.Sans tri\n : ");
					scanf("%s", choix_tri);
				} while(strcmp(choix_tri, "1") != 0 && strcmp(choix_tri, "2") != 0 && strcmp(choix_tri, "3") != 0);
				
				//AJOUT DU MODE DE TRI A LA REQUETE
				strcat(construct_requete, ";");
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
			//LA REQUETE EST STOCKÉE DANS LA VARIABLE "REQUETE"
			strcpy(requete,construct_requete);
			
			//ENVOIE DE LA REQUETE
			write(numSocket, requete, strlen(requete));
			//viderChaine(message, strlen(message)); // Vide le tableau
			
			//RECEPTION DES DONNEES DU SERVEUR
			nbLus = read(numSocket, message, MAX);
			printf("REÇU : %s", message);
			//AFFICHAGE DES DONNEES REÇUES
			afficherTrains(message);
		}
	} while (strcmp(choix_consulte,"1") == 0);
	
	// FIN DE PROGRAMME
	write(numSocket, "KILL\0" , strlen("KILL\0"));
	close(numSocket);
	
	printf("\n************ Au revoir ! ************\n");
	
	return 0;
}


