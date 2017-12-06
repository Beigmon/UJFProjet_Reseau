/* Module pour l'ouverture de connexions UDP ou TCP client ou serveur
 * Auteur : Damien Genthial, Octobre 2002, d'après la primitive
 * ouvreConnexion de André Lagrèze (Octobre 2000)
 */

#ifndef TRAIN_H
#define TRAIN_H
#define MAX 512
#define FIN_FILE -1

#include <stdlib.h>
#include <stdio.h>

/* Définition des protocoles possibles */
typedef enum  {REDUC, SUPPL, VIDE} Action_prix;

static const char *ACTION_PRIX_STRING[] = { "REDUC", "SUPPL", "VIDE" };

struct trains {
	int num_train;
	char ville_depart[30];
	char ville_arrivee[30];
	char horaire_depart[6];
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
