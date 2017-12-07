/**
 * \file Trains.h
 * \brief Gestion des Trains.
 * \author Florian PIGNARD & Jonathan MONBEIG & Mathieu CLUSEL & Louis REYNAUD 
 * \version 0.1
 * \date 5 décembre 2017
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

//static const char *ACTION_PRIX_STRING[] = { "REDUC", "SUPPL", "VIDE" };

/**
 * \struct trains
 * \brief Des information pour consulter des trains.
 */
struct trains {
	int num_train; 	/*!< Numéro du train. */
	char ville_depart[30];	/*!< Nom de la ville de départ. */
	char ville_arrivee[30];	/*!< Nom de la ville d'arrivée. */
	char horaire_depart[6];	/*!< horaire de départ du train. */
	char horaire_arrivee[6]; /*!< horaire d'arrivée du train. */
	double prix_usuel; /*!< Prix du voyage. */
	int evenement; /*!< Correspond à un changement de tarif. */// todo use enum 
};

#ifndef TRAIN_C
#define PUBLIC extern
#else
#define PUBLIC
#endif

/**
 * \fn void txtVersStructure(FILE* fichierTrain, struct trains *tabTrains)
 * \brief Procédure d'association des éléments d'un objet struct trains à partir d'une ligne d'un fichier.
 *
 * \param fichierTrain Fichier utilisé pour récupérer les éléments de l'objet , ne peut être NULL.
 * \param tabTrains objet de type struct trains dans lequel on associe les éléments à la ligne du fichier.
 * \return void.
 */
PUBLIC int nb_line_file(char* fileName);

/**
 * \fn void filelinetotxt(char * ligne, FILE* fichier_train)
 * \brief Procédure transmettant la ligne actuelle du fichier fichier_train à la chaîne de caractères ligne.
 *
 * \param ligne, chaine de caractères, ne peut être NULL.
 * \param fichier_train, fichier parcouru, ne peut être NULL.
 * \return void.
 */
PUBLIC void filelinetotxt(char * ligne, FILE* file);

/**
 * int nb_line_file(char * filename)
 * \brief Fonction de calcul du nombre de lignes d'un fichier de racine filename.
 *
 * \param filename, racine du fichier, ne peut être NULL.
 * \return nb_line, nombre de lignes du fichier de racine filename.
 */
PUBLIC void txtVersStructure(FILE* fichierTrain, struct trains *tabTrains);

/**
 * void getTemps(char * temps, int * res)
 * \brief Procédure adaptant une heure en chaîne de caractère temps dans un entier res de la forme "00:00" à l'entier 0000.
 *
 * \param temps Chaîne de caractère contenant l'heure, ne peut être NULL.
 * \param res, entier contenant l'heure sous la forme 2359 pour "23:59", ne peut être NULL.
 * \return void.
 */
PUBLIC void getTemps(char * temps, int * res);

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
PUBLIC void getOneTrainWithStartArrivalAndTime(char * villeDepart, char * villeArrivee,char * horaireDepart, struct trains *train, struct trains *tabTrains, int nbLignes);

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
 * \return int, le nombre de trains revoyés.
 */
PUBLIC int getTrainsWithTimePeriods(char * villeDepart, char * villeArrivee, char * horaireDebut, char * horaireFin, struct trains *ListTrains, struct trains *tabTrains, int nbLignes);

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
PUBLIC int getAllTrainsWithStartAndArrival(char * villeDepart, char * villeArrivee, struct trains *ListTrains, struct trains *tabTrains, int nbLignes);

/**
 * \fn static Str_t * str_new (const char * sz)
 * \brief Fonction de création d'une nouvelle instance d'un objet Str_t.
 *
 * \param sz Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Instance nouvellement allouée d'un objet de type Str_t ou NULL.
 */
PUBLIC void structureVersTxt(struct trains train, char* txt);

/**
 * \fn void calculePrix(struct trains *train)
 * \brief Calcule le prix d'un billet de train selon sa réduction (20%) ou son augmentation (10%).
 *
 * \param struct trains *train, un train.
 * \return void.
 */
PUBLIC void calculePrix(struct trains *train);

/**
 * \fn int compParPrix(const void *elmTrain1, const void *elmTrain2)
 * \brief Fonction de comparaison par prix croissant.
 *
 * \param const void *elmTrain1, un train.
 * \param const void *elmTrain2, un train.
 * \return int, si la fontion revoi 1 elmTrain1 est plus chère que elmTrain2 sinon on a -1 si elmTrain1 est moins chère que elmTrain2 sinon on a 0.
 */
PUBLIC int compParPrix(const void *v1, const void *v2);

/**
 * \fn int compParTemps(const void *elmTrain1, const void *elmTrain2)
 * \brief Fonction de comparaison par prix croissant.
 *
 * \param const void *elmTrain1, un train.
 * \param const void *elmTrain2, un train.
 * \return int, si la fontion revoi 1 elmTrain1 est plus tard que elmTrain2 sinon on a -1 si elmTrain1 est plus tôt que elmTrain2 sinon on a 0 soit il arrive à la même heure.
 */
PUBLIC int compParTemps(const void *v1, const void *v2);

/**
 * \fn void trierTrains(struct trains *tabTrains, int tailleTableau, char* mode)
 * \brief Tri un tableau de train en fonction du prix ou du temps.
 *
 * \param struct trains *tabTrains, le tableau à trier.
 * \param int tailleTableau, la taille du tableau tabTrains.
 * \param char* mode, mode = "PRIX" on trie par rapport au prix ou mode = "TEMPS" on trie sur le temps de trajet.
 * \return void
 */
PUBLIC void trierTrains(struct trains *tabTrains, int tailleTableau, char* mode);

#undef PUBLIC
#endif /* #ifndef TRAIN_H */
