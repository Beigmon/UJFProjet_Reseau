/*
 * trains.c
 * 
 * Copyright 2017 Louis Reynaud <reynloui@ltsp20>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
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

void filelinetotxt(char * ligne, FILE* fichier_train)
{
	fgets(ligne, VALUE, fichier_train);
}

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

void getTemps(char * temps, int * res)
{
	char *t = temps;
	char * pEnd;
	int heure = strtol(t, &pEnd, 10);
	int minute = strtol (pEnd+1,&pEnd,10);
	
	*res = (heure*100+minute);
}

void getTrain(char * villeDepart, char * villeArrivee,char * horaireDepart, struct trains *train, struct trains *tabTrains)
{
	int index, horaire = 2360;  // Valeur Max 23:59 -> 2359
 	//int length = (sizeof(tabTrains[0])*sizeof(tabTrains))/sizeof(struct trains);
 	//printf("%d\n", length);
	
	for(index = 0; index < 21; index++)
	{
		if (strcmp(tabTrains[index].ville_depart, villeDepart) == 0)
		{
			if (strcmp(tabTrains[index].ville_arrivee, villeArrivee) == 0) 
			{
				int bestFoundH, currentH;
				getTemps(tabTrains[index].horaire_depart, &bestFoundH);
				getTemps(horaireDepart, &currentH);
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


