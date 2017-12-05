/* Module pour l'ouverture de connexions UDP ou TCP client ou serveur
 * Auteur : Damien Genthial, Octobre 2002, d'après la primitive
 * ouvreConnexion de André Lagrèze (Octobre 2000)
 */

#ifndef TRAIN_H
#define TRAIN_H
#define MAX 512

/* Définition des protocoles possibles */
typedef enum { REDUC, SUPPL } Action_prix;

struct trains
{
	int num_train;
	char ville_depart[MAX];
	char ville_arrivee[MAX];
	char horaire_depart[MAX];
	char horaire_arrivee[MAX];
	double prix_usuel;
	Action_prix evenement;	 
} ;

#ifndef TRAIN_C
#define PUBLIC extern
#else
#define PUBLIC
#endif



#undef PUBLIC
#endif /* #ifndef TRAIN_H */
