/**
 * \file Trains.c
 * \brief ....
 * \author ....
 * \version 0.1
 * \date 5 décembre 2017
 *
 * (exemeple) Programme de test pour l'objet de gestion des chaînes de caractères Str_t.
 *
 */

#define TRAIN_C
#define VALUE 128


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../headers/Trains.h"

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void txtVersStructure(FILE* fichierTrain, struct trains *tabTrains)
{
	char ligne[VALUE];
	filelinetotxt(ligne, fichierTrain);
	//printf("LIGNE : %s\n", ligne);
	const char * REDUC = ACTION_PRIX_STRING[0];
	const char * SUPPL = ACTION_PRIX_STRING[1];

	tabTrains->num_train = atoi(strtok(ligne, ";"));
	strcpy(tabTrains->ville_depart,strtok(NULL, ";"));
	strcpy(tabTrains->ville_arrivee,strtok(NULL, ";"));
	strcpy(tabTrains->horaire_depart,strtok(NULL, ";"));
	strcpy(tabTrains->horaire_arrivee,strtok(NULL, ";"));
	tabTrains->prix_usuel = atof(strtok(NULL, ";"));
	
	const char * actionPrix = strtok(NULL, ";");
	if(strcmp(actionPrix, REDUC) == 0)
	{
		tabTrains->evenement = 0;  // REDUC
	}
	else if(strcmp(actionPrix, SUPPL) == 0)
	{
		tabTrains->evenement = 1;  // SUPPL
	}
	else 
	{
		tabTrains->evenement = 2;  // VIDE
	}
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void filelinetotxt(char * ligne, FILE* fichier_train)
{
	fgets(ligne, VALUE, fichier_train);
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
int nb_line_file(char * filename)
{
	int c, nb_line = 0;
	FILE *fichier_train = fopen(filename, "r");
	/* voici ma boucle */
	while ((c = getc(fichier_train)) != EOF){
		if (c == '\n')
			++nb_line;
	}
	fclose(fichier_train);
	return nb_line;
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void getTemps(char * temps, int * res)
{
	char *t = temps;
	char * pEnd;
	int heure = strtol(t, &pEnd, 10);
	int minute = strtol (pEnd+1,&pEnd,10);
	
	*res = (heure*100+minute);
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void getOneTrainWithStartArrivalAndTime(char * villeDepart, char * villeArrivee,char * horaireDebut, struct trains *train, struct trains *tabTrains, int nbLignes)
{
	int index, horaire = 2360;  // Valeur Max 23:59 -> 2359
	
	for(index = 0; index < nbLignes; index++)
	{
		if (strcmp(tabTrains[index].ville_depart, villeDepart) == 0)
		{
			if (strcmp(tabTrains[index].ville_arrivee, villeArrivee) == 0) 
			{
				int bestFoundH, currentH;
				getTemps(tabTrains[index].horaire_depart, &bestFoundH);
				getTemps(horaireDebut, &currentH);
				if (bestFoundH >= currentH)
				{
					if(horaire > bestFoundH)
					{
						getTemps(tabTrains[index].horaire_depart, &horaire);
						*train = tabTrains[index];
					}
				}
			}
		}
	}
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void getTrainsWithTimePeriods(char * villeDepart, char * villeArrivee, char * horaireDebut, char * horaireFin, struct trains *ListTrains, struct trains *tabTrains, int nbLignes)
{
	int index, indexTab = 0;
	int hDeb; getTemps(horaireDebut, &hDeb);
	int hFin; getTemps(horaireFin, &hFin);
	
	for(index = 0; index < nbLignes; index++)
	{
		if (strcmp(tabTrains[index].ville_depart, villeDepart) == 0)
		{
			if (strcmp(tabTrains[index].ville_arrivee, villeArrivee) == 0) 
			{
				int currentDeb;
				getTemps(tabTrains[index].horaire_depart, &currentDeb);
								
				if (currentDeb>=hDeb && currentDeb<=hFin)
				{
					ListTrains[indexTab++] = tabTrains[index];
				}
			}
		}
	}
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void getAllTrainsWithStartAndFinish(char * villeDepart, char * villeArrivee, struct trains *ListTrains, struct trains *tabTrains, int nbLignes)
{
	int index, indexTab = 0;
	
	for(index = 0; index < nbLignes; index++)
	{
		if (strcmp(tabTrains[index].ville_depart, villeDepart) == 0)
		{
			if (strcmp(tabTrains[index].ville_arrivee, villeArrivee) == 0) 
			{			
				ListTrains[indexTab++] = tabTrains[index];			
			}
		}
	}
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void structureVersTxt(struct trains train, char* txt) 
{
	char construct_txt[MAX]; 
	//char txt[MAX]; 
	char num[MAX]; 
	char prix[MAX];
	
	strcpy(txt, ""); 
	strcpy(construct_txt, ""); 
	
	strcpy(num, ""); 
	sprintf(num, "%d", train.num_train);
	
	strcpy(prix, "");
	sprintf(prix, "%.2f", train.prix_usuel);

	strcat(construct_txt, num);
	strcat(construct_txt, ";");
	
	strcat(construct_txt, train.ville_depart);
	strcat(construct_txt, ";");
	
	strcat(construct_txt, train.ville_arrivee);
	strcat(construct_txt, ";");
	
	strcat(construct_txt, train.horaire_depart);
	strcat(construct_txt, ";");
	
	strcat(construct_txt, train.horaire_arrivee);
	strcat(construct_txt, ";");
	
	strcat(construct_txt, prix);
	strcat(construct_txt, "\n");
	
	strcpy(txt, construct_txt); 
}

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
void calculePrix(struct trains *train)
{
	//REDUC
	double prix = train->prix_usuel;
	if(train->evenement == 0) 
	{
		prix = prix - (prix * 0.2);
	}
	//SUPPL
	if(train->evenement == 1)
	{
		prix = prix + (prix * 0.1);
	}
	//VIDE
	train->prix_usuel = prix;
}
