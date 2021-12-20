#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protocole.h"

#define VERBOSE 0


int match_r(char *msg){
	if(strncmp(msg,"WELCO",5)==0){return WELCO;}
	else if(strncmp(msg,"GOBYE",5)==0){return GOBYE;}
	else if(strncmp(msg,"HELLO",5)==0){return HELLO;}
	else if(strncmp(msg,"FRIE>",5)==0){return okFRIE;}
	else if(strncmp(msg,"FRIE<",5)==0){return noFRIE;}
	else if(strncmp(msg,"MESS>",5)==0){return okMESS;}
	else if(strncmp(msg,"MESS<",5)==0){return noMESS;}
	else if(strncmp(msg,"FLOO>",5)==0){return okFLOO;}
	else if(strncmp(msg,"RLIST",5)==0){return RLIST;}
	else if(strncmp(msg,"LINUM",5)==0){return LINUM;}
	else if(strncmp(msg,"SSEM>",5)==0){return okSSEM;}
	else if(strncmp(msg,"MUNEM",5)==0){return MUNEM;}
	else if(strncmp(msg,"OOLF>",5)==0){return okOOLF;}
	else if(strncmp(msg,"EIRF>",5)==0){return okEIRF;}
	else if(strncmp(msg,"ACKRF",5)==0){return ACKRF;}
	else if(strncmp(msg,"FRIEN",5)==0){return FRIEN;}
	else if(strncmp(msg,"NOFRI",5)==0){return NOFRI;}
	else if(strncmp(msg,"LBUP>",5)==0){return okLBUP;}
	else if(strncmp(msg,"NOCON",5)==0){return NOCON;}
//	else if(strncmp(msg,"IQUIT",5)==0){return IQUIT;}
	return -1;
}
int match_c(char *msg){
	if(strncmp(msg,"regis",5)==0){return REGIS;}
	else if(strncmp(msg,"conne",5)==0){return CONNE;}
	else if(strncmp(msg,"setfr",5)==0){return setFRIE;}
	else if(strncmp(msg,"sendm",5)==0){return setMESS;}
//	else if(strncmp(msg,"MENUM",5)==0){return MENUM;}
	else if(strncmp(msg,"floom",5)==0){return setFLOO;}
	else if(strncmp(msg,"liste",5)==0){return setLIST;}
	else if(strncmp(msg,"consu",5)==0){return CONSU;}
	else if(strncmp(msg,"decon",5)==0){return IQUIT;}
	return -1;
/*
	if(strncmp(msg,"REGIS",5)==0){return REGIS;}
	else if(strncmp(msg,"CONNE",5)==0){return CONNE;}
	else if(strncmp(msg,"FRIE?",5)==0){return setFRIE;}
	else if(strncmp(msg,"MESS?",5)==0){return setMESS;}
	else if(strncmp(msg,"MENUM",5)==0){return cMENUM;}
	else if(strncmp(msg,"FLOO?",5)==0){return setFLOO;}
	else if(strncmp(msg,"LIST?",5)==0){return setLIST;}
	else if(strncmp(msg,"CONSU",5)==0){return CONSU;}
	else if(strncmp(msg,"IQUIT",5)==0){return IQUIT;}
	return -1;
*/
}
//-------
int protocole_udp(char * msg){
	int a =strlen(msg);
#if VERBOSE > 1
	printf("message : %d-%d-%d  \n", (int) *msg,(int) *(1+msg),(int) *(2+msg));
#endif
	if(a>3 && strncmp(msg,"PRO",3)==0){
		return ISPROM;
	}// else if (a==3) {return ISNOTIF;}
	//return RESULT_FAIL; 
	return ISNOTIF;
}
void protocole_notif(char * msg){
	int code_notif = (int) msg[0];
	int nb_notif = msg[1]|msg[2]<<8;
	printf("nouvelle notification : ");
	switch(code_notif){
		case(0):printf("nouvelle demande d'amitie");break;
		case(1):printf("demande d'amitie acceptee");break;
		case(2):printf("demande d'amitie refusee");break;
		case(3):printf("nouveau message");break;
		case(4):printf("nouveau msg inondation");break;
		case(5):printf("nouveau msg promoteur");break;
	}
	printf("\nnotification non lues :%d\n",nb_notif);	
}
void protocole_prom(char * msg){
	if(strncmp(msg,"PROM ", strlen("PROM "))==0){
		printf("message promoteur :\n");
		int i = 0;
		while(*(msg+strlen("PROM ")+i)!='#' && (i < strlen(msg+strlen("PROM ")))){
			printf("%c",*(msg+strlen("PROM ")+i));i++;
		}printf("\n");
		//printf("message promoteur :\n%s",(msg+strlen("PROM ")));
	}else {printf("error protocole\n");}
}

int connect_accepted(char * msg){
#if VERBOSE >1
	printf("verify %d==0\n",strncmp(msg,"WELCO+++",8));
	printf("verify %d==0\n",strncmp(msg,"GOBYE+++",8));
#endif
	if(strncmp(msg,"WELCO+++",8)==0){return RESULT_OK;}
	else if(strncmp(msg,"GOBYE+++",8)==0){return RESULT_FAIL;}
	return RESULT_UNKNOW;
}
//int to little endian
void print_bytes(void *p, size_t len)
{
    size_t i;
    printf("(");
    for (i = 0; i < len; ++i)
        printf("%02X", ((unsigned char*)p)[i]);
    printf(")");
}
char * convert(int value){
	short int x = (short int) value;

	char * c=malloc(2);
	char *z=c;
//	printf("%p  %p\n",c,z);
	*c++ = x & 0xFF;
//	printf("%p\n",c);
	*c++ = (x >> 8) & 0xFF;
//	printf("%p\n",c);
//	print_bytes(&x,sizeof(short));
//printf("size %d\n",sizeof(short int));
//	print_bytes(z,sizeof(short));
	return z;
}

//------------------generation de message-------------------------
char * inscription(char * id, char * port,int mdp){
	int size =12+8+strlen(port)+sizeof(int);
	char * msg = malloc(size*sizeof(char));
	strcpy(msg,"REGIS ");
	strcat(msg,id);
	int i;
	for( i= 0; i<(8-strlen(id));i++){
		strcat(msg," ");
	}
	strcat(msg," ");
	strcat(msg,port);
	strcat(msg," ");
	char *m=convert(mdp);
	sprintf(msg,"%s%c%c",msg,*m, *(m+1));
//	printf("test%02X%02X\n",*m, *(m+1));
	free(m);
//	strcat(msg,mdp);
	strcat(msg,"+++");
	//msg[size-1]='\0';
//printf("%s\n",msg);
	return msg;
}
char * connect_msg(char * id,int mdp){
	char *buff = malloc((8+sizeof(int)+11)*sizeof(char));
	strcpy(buff,"CONNE ");
	strcat(buff,id);
	int i;
	for( i= 0; i<(8-strlen(id));i++){
		strcat(buff," ");
	}
	strcat(buff," ");
	char *m=convert(mdp);
	sprintf(buff,"%s%c%c",buff,*m, *(m+1));
//	printf("test%02X%02X\n",*m, *(m+1));
	free(m);	//strcat(buff,mdp);
//	sprintf(buff,"%s%hu",buff,mdp);
	strcat(buff,"+++");
//printf("%s\n",buff);
	return buff;
}
char * deconnect_msg(){
	char *buff = malloc(8*sizeof(char));
	strcpy(buff,"IQUIT+++");
	return buff;
}
char * setfriend_msg(char * id){
	char *buff = malloc((strlen(id)+20)*sizeof(char));
	strcpy(buff,"FRIE? ");
	strcat(buff,id);
	int i;
	for( i= 0; i<(8-strlen(id));i++){
		strcat(buff," ");
	}
	strcat(buff,"+++");
	return buff;
}
char ** send_msg(char ** tbuff, char * id,int s){
	//tbuff[0] = malloc((250)*sizeof(char));
	char ** tbn = malloc(2*(s+1)*sizeof(void*));
	tbn[0] = malloc((250)*sizeof(char));
	strcpy(tbn[0],"MESS? ");
	strcat(tbn[0],id);
	int i;
	for( i= 0; i<(8-strlen(id));i++){
		strcat(tbn[0]," ");
	}
	strcat(tbn[0]," ");
	char * str = malloc(2*200*sizeof(char));
	sprintf(str, "%04d\0", s);
	strcat(tbn[0],str);
	strcat(tbn[0],"+++\0");
	char * buff;
	for(i=0;i<s;i++){
		buff = malloc(4*(250)*sizeof(char));
		strcpy(buff,"MENUM \0");		
		sprintf(str, "%04d %s+++\0", i,tbuff[i+1]);
		strcat(buff,str);
		free(tbuff[i+1]);
		tbn[i+1]=malloc((strlen(buff)+1)*sizeof(char));
		strcpy(tbn[i+1],buff);
		free(buff);
	}
	free(str);
/*
printf("send_msg fin\n");
for(len=0;len<=s;len++){
printf("tbn[%d]=%s\n",len,tbn[len]);}
*/
	return tbn;
}
int send_msg_old(char ** tbuff, char * id,int s){
	//tbuff[0] = malloc((250)*sizeof(char));
	tbuff[0] = malloc((250)*sizeof(char));
	strcpy(tbuff[0],"MESS? ");
	strcat(tbuff[0],id);
	int i;
	for( i= 0; i<(8-strlen(id));i++){
		strcat(tbuff[0]," ");
	}
	strcat(tbuff[0]," ");
	char * str = malloc(200*sizeof(char));
	sprintf(str, "%04d", s);
	strcat(tbuff[0],str);
	strcat(tbuff[0],"+++");
	char * buff;
	for(i=0;i<s;i++){
		buff = malloc(2*(250)*sizeof(char));
		strcpy(buff,"MENUM ");		
		sprintf(str, "%04d %s+++", i,tbuff[i+1]);
		strcat(buff,str);
		free(tbuff[i+1]);
		tbuff[i+1]=malloc((strlen(buff)+1)*sizeof(char));
		strcpy(tbuff[i+1],buff);
		free(buff);
	}
	free(str);
int a;for(a=0;a<=s;a++){
printf("tbuff[%d]=%s\n",a,tbuff[a]);}
	return i+1;
}
/*
int send_msg_old(char ** tbuff, char * id,char * msg){
	int s =strlen(msg);
	s=s%200 +1;
	tbuff=malloc((s+1)*sizeof(char *));
	tbuff[0] = malloc((20+strlen(id))*sizeof(char));
	strcpy(tbuff[0],"MESS? ");
	strcat(tbuff[0],id);
	int i;
	for( i= 0; i<(8-strlen(id));i++){
		strcat(tbuff[0]," ");
	}
	strcat(tbuff[0]," ");
	char str[10];
	sprintf(str, "%d", s);
	strcat(tbuff[0],str);
	strcat(tbuff[0],"+++");
	for(i=0;i<=s;i++){
		tbuff[i+1] = malloc((220)*sizeof(char));
		strcpy(tbuff[i+1],"MENUM ");		
		sprintf(str, "%d", i);
		strcat(tbuff[i+1],str);
		strcat(tbuff[i+1]," ");
		strncat(tbuff[i+1],msg+(i*200),200);
		strcat(tbuff[i+1],"+++");
	}
	return i+1;
}*/
char * floo_msg(char * msg){
	char *buff = malloc((strlen(msg)+10)*sizeof(char));
	strcpy(buff,"FLOO? ");
	strcat(buff,msg);
	strcat(buff,"+++");
//	printf("%s\n",buff);
	return buff;
}
char * list_msg(){
	char *buff = malloc(8*sizeof(char));
	strcpy(buff,"LIST?+++");
	return buff;
}
char * consult_msg(){
	char * buff = malloc(2*8*sizeof(char));
	strcpy(buff,"CONSU+++");
	return buff;
}
char * okfriend_msg(){
	char *buff = malloc(8*sizeof(char));
	strcpy(buff,"OKIRF+++");
	return buff;
}
char * nofriend_msg(){
	char *buff = malloc(8*sizeof(char));
	strcpy(buff,"NOKRF+++");
	return buff;
}
