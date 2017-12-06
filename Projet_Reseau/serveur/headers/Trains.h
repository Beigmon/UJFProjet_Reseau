/**
 * \file Trains.h
 * \brief ....
 * \author ....
 * \version 0.1
 * \date 5 décembre 2017
 *
 * (exemeple) Programme de test pour l'objet de gestion des chaînes de caractères Str_t.
 *
 */
#ifndef TRAIN_H
#define TRAIN_H
#define MAX 512
#define FIN_FILE -1

#include <stdlib.h>
#include <stdio.h>

/* Définition des protocoles possibles */
typedef enum {
	REDUC,	/*!< REDUC. */
	SUPPL,	/*!< SUPPL. */
	VIDE	/*!< VIDE. */
} Action_prix;

static const char *ACTION_PRIX_STRING[] = { "REDUC", "SUPPL", "VIDE" };

/**
 * \struct Str_t
 * \brief Objet chaîne de caractères.
 *
 * Str_t est un petit objet de gestion de chaînes de caractères. 
 * La chaîne se termine obligatoirement par un zéro de fin et l'objet 
 * connait la taille de chaîne contient !
 */
struct trains {
	int num_train; 	/*!< Chaîne avec  caractère null de fin de chaîne. */
	char ville_depart[30];	/*!< Chaîne avec  caractère null de fin de chaîne. */
	char ville_arrivee[30];	/*!< Chaîne avec  caractère null de fin de chaîne. */
	char horaire_depart[6];	/*!< Chaîne avec  caractère null de fin de chaîne. */
	char horaire_arrivee[6];
	double prix_usuel;
	Action_prix evenement;	 
};

#ifndef TRAIN_C
#define PUBLIC extern
#else
#define PUBLIC
#endif

PUBLIC int nb_line_file(char* fileName);

PUBLIC void filelinetotxt(char * ligne, FILE* file);

PUBLIC void txtVersStructure(FILE* fichierTrain, struct trains *tabTrains);

PUBLIC void getTemps(char * temps, int * res);

PUBLIC void getTrain(char * villeDepart, char * villeArrivee,char * horaireDepart, struct trains *train, struct trains *tabTrains, int nbLignes);

PUBLIC void structureVersTxt(struct trains train, char* txt);

PUBLIC void calculePrix(struct trains *train);

#undef PUBLIC
#endif /* #ifndef TRAIN_H */
