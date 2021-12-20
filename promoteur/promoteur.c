#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#define LM_UDP 300
#define LM_TCP 200

/*
int init_socket_multicast(char *adr, char *port){
	int sock=socket(PF_INET,SOCK_DGRAM,0);
	struct addrinfo *first_info;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype=SOCK_DGRAM;
	int r=getaddrinfo(adr,port,NULL,&first_info);
	if(r==0){
		if(first_info!=NULL){
			return sock;
		}
	}
	return null;
}
void send_udp(char* msg,int sock,){
}


			struct sockaddr *saddr=first_info->ai_addr;
			char tampon[100];
			int i=0;
			for(i=0;i<=10;i++){
				strcpy(tampon,"MESSAGE ");
				char entier[3];
				sprintf(entier,"%d",i);
				strcat(tampon,entier);
				sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));

*/
int init_sock_tcp(char *address, char *port){
	struct addrinfo hints;
	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype=SOCK_STREAM;

	//recuperation info
	struct addrinfo * addrLu;
	int r = getaddrinfo(address,port,&hints,&addrLu);
	if (r!=0){return 1;}

	//recup ip et affichage
	struct sockaddr_in *addressin=(struct sockaddr_in *) (addrLu->ai_addr);
	int sock=socket(PF_INET,SOCK_STREAM,0);
	//connection
	r=connect(sock,(struct sockaddr *)addressin,(socklen_t)sizeof(struct sockaddr_in));
	if(r==-1){return 1;}

     return sock;
}
void envoi_tcp(int sock,char * buff){
	int i=strlen(buff);
printf("envoyé : %s\n",buff);
	if(send(sock,buff,i,0)<0){
		printf("error send %s\n",buff);
	}
}

char * traitement_udp(char* msg){
	char * msg_final = malloc((50+LM_UDP)*sizeof(char));
	strcpy(msg_final,"PROM ");
	strcat(msg_final,msg);
	int i,n = strlen(msg);
	for(i = 0;i<300-n;i++){
		strcat(msg_final,"#");
	}
	return msg_final;
}
char * traitement_tcp(char * msg,char * addr,char * port){

	char * msg_final = malloc((50+LM_TCP)*sizeof(char));
	strcpy(msg_final,"PUBL? ");
	strcat(msg_final,addr);
	strcat(msg_final," ");
	strcat(msg_final,port);
	strcat(msg_final," ");
	strcat(msg_final,msg);
	strcat(msg_final,"+++");

	return msg_final;

}


char * saisie_message(int * res){
	char * type = malloc(5*sizeof(char));
	fgets(type, 5, stdin);
	int size = 0;
	if (strncmp(type,"prom",strlen("prom"))==0){
		size=LM_UDP;
	}
	if (strncmp(type,"publ",strlen("publ"))==0){
		size=LM_TCP;
	}
	free(type);
	char * a = malloc(1*sizeof(char));
	fgets(a, 1, stdin);
	free(a);

	char *msg = malloc((10+size)*sizeof(char));
	fgets(msg, size, stdin);
	*(msg+strlen(msg)-1)='\0';
	*res=size;
	return msg;
}


int main (int args, char * argv[]){
//argument 1 : adresse serveur
//argument 2 : port tcp serveur
//argument 3 : adresse multidifusion
//argument 4 : port multidifusion

//------------------------Initialisation Socket---------------------------
	int sock_tcp = init_sock_tcp(argv[1],argv[2]);
	int sock_udp = socket(PF_INET,SOCK_DGRAM,0);
	struct addrinfo *first_info;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype=SOCK_DGRAM;
	int r=getaddrinfo(argv[3],argv[4],NULL,&first_info);
	struct sockaddr *saddr=first_info->ai_addr;

//------------------------Boucle Saisie-Envoi---------------------------
	fd_set rfds;
	char * msg;
	char * msg_final;
	int res;
	while(1){
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);
		if(select(STDIN_FILENO+1, &rfds, NULL, NULL, NULL) ==-1){
			//error
		}
		if(FD_ISSET(STDIN_FILENO, &rfds)){
		//saisie du message
			msg = saisie_message(&res);
		//traitement
			switch(res){
			case(LM_TCP):
				msg_final=traitement_tcp(msg,argv[3], argv[4]);
				envoi_tcp(sock_tcp,msg_final);
				free(msg_final);
//				msg_final=traitement_udp(msg);
//				sendto(sock_udp,msg_final,strlen(msg_final),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
//				free(msg_final);
				free(msg);
				break;
			case(LM_UDP):
				msg_final=traitement_udp(msg);
//printf("envoyé : %s\n",msg_final);
				r=sendto(sock_udp,msg_final,strlen(msg_final),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
//printf("r=%d",r);
				free(msg_final);
				free(msg);
				break;
			default:
				printf("erreur saisie :\n%s\n",msg);
				free(msg);
			}
                }
	}
	return 0;
}
