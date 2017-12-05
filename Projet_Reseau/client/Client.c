/*
 * Client.c
 * 
 * Copyright 2017 Jonathan Monbeig <monbeigj@ltsp26>
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

int main(int argc, char **argv)
{
	int noport = atoi(argv[2]);
	char message[MAX];
	int nb;
	/*struct sockaddr_in s;
	struct hostent * h;
	int p = socket(AF_INET, SOCK_STREAM, 0);
	h = gethostbyname(argv[1]);
	printf("%s\n ", (*h).h_name);

	s.sin_family = AF_INET;
	memcpy((char*) &s.sin_addr.s_addr,(char*) h->h_addr_list[0], h->h_length);
	s.sin_port = htons(noport);
	int echange = connect( p, (struct sockaddr *) &s, sizeof(s));
	if(echange<0)
	{	
		printf("Il y a eu une erreur de connexion\n");
	}*/
	int p = socketClient(argv[1], noport, TCP);
	nb = read(p, message, MAX);
	message[nb+1] = 0;
	printf("%s", message);
	scanf("%s", (char *)&message);
	write(p, message, 1);
	if(strcmp(message, "1"))
	{
		nb = read(p, message, MAX);
		message[nb+1] = 0;
		printf("%s", message);
	}
	nb = read(p, message, MAX);
	printf("\n");
	write(1, message, nb);
	return 0;
}


