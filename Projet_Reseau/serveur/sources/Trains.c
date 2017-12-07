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
 * \fn void txtVersStructure(FILE* fichierTrain, struct trains *tabTrains)
 * \brief Procédure d'association des éléments d'un objet struct trains à partir d'une ligne d'un fichier.
 *
 * \param fichierTrain Fichier utilisé pour récupérer les éléments de l'objet , ne peut être NULL.
 * \param tabTrains objet de type struct trains dans lequel on associe les éléments à la ligne du fichier.
 * \return void.
 */
void txtVersStructure(FILE* fichierTrain, struct trains *tabTrains)
{
	char ligne[VALUE];
	filelinetotxt(ligne, fichierTrain);
	//printf("LIGNE : %s\n", ligne);
	//const char * REDUC = ACTION_PRIX_STRING[0];
	//const char * SUPPL = ACTION_PRIX_STRING[1];

	tabTrains->num_train = atoi(strtok(ligne, ";"));
	strcpy(tabTrains->ville_depart,strtok(NULL, ";"));
	strcpy(tabTrains->ville_arrivee,strtok(NULL, ";"));
	strcpy(tabTrains->horaire_depart,strtok(NULL, ";"));
	strcpy(tabTrains->horaire_arrivee,strtok(NULL, ";"));
	tabTrains->prix_usuel = atof(strtok(NULL, ";"));

	const char * actionPrix = strtok(NULL, ";");
	
	if(strcmp(actionPrix, "REDUC\n") == 0)
	{
		tabTrains->evenement = 0;  // REDUC
	}
	else if(strcmp(actionPrix, "SUPPL\n") == 0)
	{
		tabTrains->evenement = 1;  // SUPPL
	}
	else
	{
		tabTrains->evenement = 2;  // VIDE
	}
}

/**
 * \fn void filelinetotxt(char * ligne, FILE* fichier_train)
 * \brief Procédure transmettant la ligne actuelle du fichier fichier_train à la chaîne de caractères ligne.
 *
 * \param ligne, chaine de caractères, ne peut être NULL.
 * \param fichier_train, fichier parcouru, ne peut être NULL.
 * \return void.
 */
void filelinetotxt(char * ligne, FILE* fichier_train)
{
	fgets(ligne, VALUE, fichier_train);
}

/**
 * int nb_line_file(char * filename)
 * \brief Fonction de calcul du nombre de lignes d'un fichier de racine filename.
 *
 * \param filename, racine du fichier, ne peut être NULL.
 * \return nb_line, nombre de lignes du fichier de racine filename.
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
 * void getTemps(char * temps, int * res)
 * \brief Procédure adaptant une heure en chaîne de caractère temps dans un entier res de la forme "00:00" à l'entier 0000.
 *
 * \param temps Chaîne de caractère contenant l'heure, ne peut être NULL.
 * \param res, entier contenant l'heure sous la forme 2359 pour "23:59", ne peut être NULL.
 * \return void.
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
 * \fn void getOneTrainWithStartArrivalAndTime(char * villeDepart, char * villeArrivee,char * horaireDebut, struct trains *train, struct trains *tabTrains, int nbLignes)
 * \brief Procédure associant un objet struct trains train à un des élément d'un tableau d'objets struct trains tabTrains en fonction d'une ville de départ, d'arrivée et un horaire.
 *
 * \param villeDepart, chaîne de caractère contenant la ville de départ, ne peut être NULL.
 * \param villeArrivee, chaîne de caractère contenant la ville d'arrivée, ne peut être NULL.
 * \param horaireDebut, chaîne de caractère contenant l'horaire, ne peut être NULL.
 * \param train, objet de type struct trains contenant le résultat de la procédure, ne peut être NULL.
 * \param tabTrains, tableau d'objets de type struct trains contenant les données du programme, ne peut être NULL.
 * \param nbLignes, nombre de lignes du tableau tabTrains, ne peut être NULL.
 * \return void.
 */
void getOneTrainWithStartArrivalAndTime(char * villeDepart, char * villeArrivee, char * horaireDebut, struct trains *train, struct trains *tabTrains, int nbLignes)
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
 * \fn void getTrainsWithTimePeriods(char * villeDepart, char * villeArrivee, char * horaireDebut, char * horaireFin, struct trains *ListTrains, struct trains *tabTrains, int nbLignes)
 * \brief Procédure associant un tableau d'objets struct trains train à des éléments d'un tableau d'objets struct trains tabTrains en fonction d'une ville de départ, d'arrivée et une tranche horaire(début, fin).
 *
 * \param villeDepart, chaîne de caractère contenant la ville de départ, ne peut être NULL.
 * \param villeArrivee, chaîne de caractère contenant la ville d'arrivée, ne peut être NULL.
 * \param horaireDebut, chaîne de caractère contenant l'horaire, ne peut être NULL.
 * \param horaireFin, chaîne de caractère contenant l'horaire, ne peut être NULL.
 * \param train, objet de type struct trains contenant le résultat de la procédure, ne peut être NULL.
 * \param tabTrains, tableau d'objets de type struct trains contenant les données du programme, ne peut être NULL.
 * \param nbLignes, nombre de lignes du tableau tabTrains, ne peut être NULL.
 * \return int, le nombre de trains revoyés..
 */
int getTrainsWithTimePeriods(char * villeDepart, char * villeArrivee, char * horaireDebut, char * horaireFin, struct trains *ListTrains, struct trains *tabTrains, int nbLignes)
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
					ListTrains[indexTab] = tabTrains[index];
					indexTab++;
				}
			}
		}
	}
	return indexTab;
}

/**
 * \fn void getAllTrainsWithStartAndArrival(char * villeDepart, char * villeArrivee, struct trains *ListTrains, struct trains *tabTrains, int nbLignes)
 * \brief Procédure associant un tableau d'objets struct trains train à tous les éléments du tableau d'objets struct trains tabTrains concordant à une ville de départ et d'arrivée.
 *
 * \param villeDepart, chaîne de caractère contenant la ville de départ, ne peut être NULL.
 * \param villeArrivee, chaîne de caractère contenant la ville d'arrivée, ne peut être NULL.
 * \param ListTrains, objet de type struct trains contenant le résultat de la procédure, ne peut être NULL.
 * \param tabTrains, tableau d'objets de type struct trains contenant les données du programme, ne peut être NULL.
 * \param nbLignes, nombre de lignes du tableau tabTrains, ne peut être NULL.
 * \return int, le nombre de trains revoyés.
 */
int getAllTrainsWithStartAndArrival(char * villeDepart, char * villeArrivee, struct trains *ListTrains, struct trains *tabTrains, int nbLignes)
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
	return indexTab;
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

//FONCTION DE COMPARAISON PAR PRIX CROISSANT
int compParPrix(const void *v1, const void *v2)
{
	char test[MAX];
    //RES = 0 SI LES PRIX SONT EGAUX
    int res = 0;
    //TYPAGE DE V1 ET V2 DANS T1 ET T2
    struct trains *t1 = (struct trains *)v1;
    struct trains *t2 = (struct trains *)v2;
    
    //RÉCUPÉRATION DES PRIX (DÉJÀ CALCULÉS)
    double prix1 = t1->prix_usuel;
    double prix2 = t2->prix_usuel;
    
    //COMPARAISON DES PRIX
    if(prix1 > prix2)
    {
        res = 1;
    }
    if(prix1 < prix2)
    {
        res = -1;
    }
    return res;
}

//FONCTION DE COMPARAISON PAR TEMPS CROISSANT
int compParTemps(const void *v1, const void *v2)
{
    //RES = 0 SI LES TEMPS SONT EGAUX
    int res = 0;

    //TYPAGE DE V1 ET V2 DANS T1 ET T2
    struct trains *t1 = (struct trains *)v1;
    struct trains *t2 = (struct trains *)v2;

    //VARIABLES DE RÉCUPERATION DES TEMPS
    int tempsDepart1;
    int tempsArrivee1;
    int tempsDepart2;
    int tempsArrivee2;

    //AFFECTATION DES DONNÉES AUX VARIABLES
    getTemps(t1->horaire_depart, &tempsDepart1);
    getTemps(t1->horaire_arrivee, &tempsArrivee1);
    getTemps(t2->horaire_depart, &tempsDepart2);
    getTemps(t2->horaire_arrivee, &tempsArrivee2);

    //COMPARAISON DES TEMPS
    if((tempsArrivee1 - tempsDepart1) > (tempsArrivee2 - tempsDepart2)) // ex: 750-650(100) > 850-780 (70)
    {
        res = 1;
    }
    if((tempsArrivee1 - tempsDepart1) < (tempsArrivee2 - tempsDepart2)) // ex: 850-780 (70) < 750-650(100)
    {
        res = -1;
    }
    return res;
}

//TRI UN TABLEAU DE TRAIN (*tabTrains) EN FONCTION DU PARAMÈTRE "mode" (prix/temps)
void trierTrains(struct trains *tabTrains, int tailleTableau, char* mode)
{
    int index;
    //TRI EN PAR PRIX CROISSANT
    if(strcmp(mode, "prix") == 0)
    {
        //CALCULE LE PRIX DES TRAINS
        for(index = 0; index<tailleTableau;index++)
        {
            calculePrix(&tabTrains[index]);
        }

        //TRI LE TABLEAU DE TRAINS
        qsort(tabTrains, tailleTableau,sizeof(struct trains), compParPrix);
    }
    //TRI EN PAR DUREE CROISSANTE
    else if(strcmp(mode, "temps") == 0)
    {
        //TRI LE TABLEAU DE TRAINS
        qsort(tabTrains, tailleTableau,sizeof(struct trains), compParTemps);
    }
}
