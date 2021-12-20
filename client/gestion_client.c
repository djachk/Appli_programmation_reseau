#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gestion_client.h"

#define MAX 256

int verifyID(char * buff,int size){
	int i;
	if(size>8){printf("identifiant trop long (8char max)\n");return RESULT_FAIL;}
	for (i =0; i<size-1;i++){
		if (isalnum((int) buff[i])==0) {
			printf("caracteres non-alphanumerique non supportés\n");
			return RESULT_FAIL;}
	}
	return RESULT_OK;
}
int verifyMDP(char * buff,int size){
	int i;
        for (i =0; i<size-1;i++){
                if (isdigit((int) buff[i])==0) {
			printf("chiffres uniquement\n");
			return RESULT_FAIL;}
        }
	if ((strtol(buff,NULL,10)>MAXMDP)||(strtol(buff,NULL,10)<=0)){
		printf("mdp trop grand\n");
		return RESULT_FAIL;}
	return RESULT_OK;
}
int saisieID(client * c){
	char * buff = malloc((MAX)*sizeof(char));
	do{
		printf("rentrez votre identifiant (8 char max)\n");
		if (scanf("%s",buff)==0) {return RESULT_FAIL;}
	}while(verifyID(buff,strlen(buff))!=RESULT_OK);
	c->identifiant = malloc((MAXPSEUDO+1)*sizeof(char));
	strcpy(c->identifiant,buff);
	return RESULT_OK;
}

int saisieMDP(client * c){
	char * buff = malloc((MAX)*sizeof(char));
        do{
		printf("rentrez votre mdp (0<mdp<65535)\n");
		if (scanf("%s",buff)==0) {return RESULT_FAIL;}
	}while(verifyMDP(buff,strlen(buff))!=RESULT_OK);
        c->mdp = strtol(buff,NULL,10);
        return RESULT_OK;
}

int createClient(client * c){
	int r=saisieID(c);
	if (r ==RESULT_OK){
		r = saisieMDP(c);
	}
	c->port = 0;
	return r;
}
void setPort(client * c,int p){
	c->port = p;
}
void freeClient(client * c){
	free(c->identifiant);
	free(c);
}
