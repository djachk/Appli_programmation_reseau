#include <stdio.h>
#include <stdlib.h>
#include "gestion_client.h"
#include "connexion.h"

#define SERVERIP "127.0.0.1"
#define SERVERPORT "8888"
//#define CLIENTPORT "9998"

#define EXIT_SUCCESS 0
#define EXIT_ERROR 1

int main (int argc, char * argv[]){
	//test gestion_client
//printf("%d\n",sizeof(char *));
	client *c = malloc(sizeof(client));
	createClient(c);
	setPort(c,strtol(argv[1],NULL,10));
//	printf("id %s\nmdp %d\nport %d\n",c->identifiant,c->mdp,c->port); 
	char mdp[8] ;
	sprintf(mdp,"%d",c->mdp);
	
	int s_u = init_sock_UDP(argv[1]);
	int s_t = init_sock_TCP(SERVERIP,SERVERPORT);
	if (s_t==RESULT_FAIL){printf("no server\n"); return EXIT_ERROR;}
	int r=inscript(s_t,c->identifiant,argv[1],mdp);
	if (r==RESULT_UNKNOW){
		printf("erreur protocole inscription\n");
		return EXIT_ERROR;
	}
	else if (r==RESULT_FAIL){
		printf("Refused by server\n");
		return EXIT_ERROR;
	}
	printf("Connected\n");
	run(s_u,s_t,SERVERIP,SERVERPORT);
	//test protocole

	freeClient(c);
	return EXIT_SUCCESS;
}
