#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#include "connexion.h"
#include "protocole.h"

#define VERBOSE 0

#define MAX(a,b) (a>b)?a:b
#define ISDECO -1 

int init_sock_UDP(char * port){
	int sock=socket(PF_INET,SOCK_DGRAM,0);
	sock=socket(PF_INET,SOCK_DGRAM,0);
	struct sockaddr_in address_sock;
	address_sock.sin_family=AF_INET;
	address_sock.sin_port=htons(strtol(port,NULL,10));
	address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
	int r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
	if (r!= 0){return RESULT_FAIL;}
	return sock;
}
int init_sock_TCP(char * address, char * port){
	struct addrinfo hints;
	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype=SOCK_STREAM;

	//recuperation info
	struct addrinfo * addrLu;
	int r = getaddrinfo(address,port,&hints,&addrLu);
	if (r!=0){return RESULT_FAIL;}

	//recup ip et affichage
	struct sockaddr_in *addressin=(struct sockaddr_in *) (addrLu->ai_addr);
	int sock=socket(PF_INET,SOCK_STREAM,0);
	//connection
	r=connect(sock,(struct sockaddr *)addressin,(socklen_t)sizeof(struct sockaddr_in));
	if(r==-1){return RESULT_FAIL;}

        return sock;
}
int fromServer(struct sockaddr_in emet,char * address, char * port){
#if VERBOSE > 1
	printf("ip server _%s_ : ip receive _%s_ result %d\n",address,inet_ntoa(emet.sin_addr),(strncmp(inet_ntoa(emet.sin_addr),address,strlen(address))==0));
	printf("port server _%d_ : port receive _%d_ result %d\n",strtol(port,NULL,10),ntohs(emet.sin_port),(ntohs(emet.sin_port) == strtol(port,NULL,10)));
#endif
	if ((ntohs(emet.sin_port) == strtol(port,NULL,10)) && strncmp(inet_ntoa(emet.sin_addr),address,strlen(address))==0) {return RESULT_OK;}
	return RESULT_FAIL;
}
void receive_udp(int sock,char * address, char * port){
	struct sockaddr_in emet;
	socklen_t t=sizeof(emet);
	char * buff=malloc((MAX_BUFF_UDP +1)*sizeof(char));
	int rec=recvfrom(sock,buff,MAX_BUFF_UDP,0,(struct sockaddr *)&emet,&t);
	buff[rec]='\0';
	int a = protocole_udp(buff);
	int b =fromServer(emet,address,port);
#if VERBOSE >= 1
	printf("Message recu : %s\n",buff);
#endif
#if VERBOSE > 1
	printf("(ISNOTIF %d, ISPROM %d, RESULT_OK %d; RESULT_FAIL %d)\nprotocole_udp (buff) : %d\n fromServer : %d\n",ISNOTIF,ISPROM,RESULT_OK,RESULT_FAIL,a,b);
#endif
	if((a==ISNOTIF) ){//&& (b==RESULT_OK)){
		protocole_notif(buff);
	}else if((a==ISPROM) && (b==RESULT_FAIL)){
		//char * buff_suite=malloc((MAX_BUFF_UDP - MIN_BUFF_UDP +1)*sizeof(char));
		//char * msg =malloc((MAX_BUFF_UDP +1)*sizeof(char));
		//rec=recv(sock,buff_suite,(MAX_BUFF_UDP-MIN_BUFF_UDP),0);
		//buff_suite[rec]='\0';
		//strcpy(msg,buff);
		//strcat(msg,buff_suite);
		protocole_prom(buff);
		//free(buff_suite);
		//free(msg);
	}else{printf("error protocole\n");}
	free(buff);
}
char * receive_TCP(int sock,int n){
	char * recpt=malloc((n+1)*sizeof(char));
	int s=recv(sock,recpt,n,0);
	recpt[s]='\0';
#if VERBOSE > 1
	printf("recu : %s\n",recpt);
#endif
	return recpt;
}
char * get_text(int n){
	char * buff=malloc((n+1)*sizeof(char));
	fgets(buff, n+1,stdin);
	buff[strlen(buff)]='\0';
	return buff;
}
void send_TCP(int sock,char * buff){
	int i=strlen(buff);
	if(send(sock,buff,i,0)<0){
		printf("error send %s\n",buff);
	}
#if VERBOSE >= 1
	printf("envoi : %s\n",buff);
#endif
	free(buff);
}

void read_msg(int s_t){
	char *a,*b,*c;
	a=receive_TCP(s_t,1);
	b=receive_TCP(s_t,1);
	c=receive_TCP(s_t,1);
	while(*a!='+' ||*b!='+' ||*c!='+'){
		printf("%c",*a);
		*a=*b;
		*b=*c;
		free(c);
		c=receive_TCP(s_t,1);
	}
}
void add_prom(int * proms, int *nbprom, char * addr, char * port){
//---------------- reallocation memoire tableau proms
	int * buff = realloc(proms, (*nbprom+1)*sizeof(int));
	proms = buff;
	*nbprom=*nbprom +1;
	

//----------------creation socket --------------------------
	int sock=socket(PF_INET,SOCK_DGRAM,0);
        int ok=1;
        int r=setsockopt(sock,SOL_SOCKET,SO_REUSEPORT,&ok,sizeof(ok));
        struct sockaddr_in address_sock;
        address_sock.sin_family=AF_INET;
        address_sock.sin_port=htons(strtol(port,NULL,10));
        address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
        r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr=inet_addr(addr);
        mreq.imr_interface.s_addr=htonl(INADDR_ANY);
        r=setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
	r=setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&ok,sizeof(ok));

//----------------add new sock to the list-------------
	proms[*nbprom-1]=sock;

}

int receive_msg(int s_t,int * proms, int *nbprom){
	char *type =receive_TCP(s_t,5);
	//analyse du type
	int r =match_r(type);
	//traitement par type
	int i,j;
	char * id;
	char * item;
	char * num;
	char * res;
	char * answer;
	int stop =1;
	switch(r){
		case(WELCO):
			receive_TCP(s_t,3);
			printf("inscrit\n");
			break;
		case(GOBYE):
			receive_TCP(s_t,3);
			printf("deconnected\n");
			close(s_t);
			return ISDECO;
			break;
		case(HELLO):
			receive_TCP(s_t,3);
			printf("connected\n");
			break;
		case(okFRIE):
			receive_TCP(s_t,3);
			printf("demande transmise\n");			
			break;
		case(noFRIE):
			receive_TCP(s_t,3);
			printf("id non connus\n");			
			break;
		case(okMESS):
			receive_TCP(s_t,3);
			printf("message transmis\n");
			break;
		case(noMESS):
			receive_TCP(s_t,3);
			printf("message NON transmis\n");
			break;
		case(okFLOO):
			receive_TCP(s_t,3);
			printf("message inondation transmis\n");
			break;
		case(RLIST):
			receive_TCP(s_t,1);//espace
			item=receive_TCP(s_t,3);
			receive_TCP(s_t,3);
			printf("nombre de clients : %s\n",item);
			i = strtol(item,NULL,10);
			free(item);
			for(j=0;j<i;j++){
				receive_msg(s_t,proms,nbprom);
			}
			break;
		case(LINUM):
			receive_TCP(s_t,1);//espace
			id =receive_TCP(s_t,8);//id
			receive_TCP(s_t,3);
			printf("client : %s\n",id);
			free(id);
			break;
		case(okSSEM):
			receive_TCP(s_t,1);//espace
			id=receive_TCP(s_t,8);
			receive_TCP(s_t,1);//espace
			num = receive_TCP(s_t,4);
			printf("message de %s\n",id);
			i = strtol(num,NULL,10);
			free(num);free(id);
			receive_TCP(s_t,3);
			for(j=0;j<i;j++){
				receive_msg(s_t,proms,nbprom);
			}
			break;
		case(MUNEM):
			receive_TCP(s_t,1);//espace
			num =receive_TCP(s_t,4);//numero msg
			receive_TCP(s_t,1);//espace
			read_msg(s_t);printf("\n");
			free(num);
			break;
		case(okOOLF):
			receive_TCP(s_t,1);//espace
			id =receive_TCP(s_t,8);//id
			receive_TCP(s_t,1);
			printf("message inondation de : %s\n",id);
			free(id);
			read_msg(s_t);
			break;
		case(okEIRF):
			receive_TCP(s_t,1);//espace
			id =receive_TCP(s_t,8);//id
			receive_TCP(s_t,3);//+++
			printf("demande d'ami de %s\n",id);
			free(id);
			printf("accepter la demande ?(yes/no)\n");
			while(stop==1){
				res=get_text(3);
				if(strncmp(res,"yes",3)==0){
					stop = 0;
					answer=okfriend_msg();
				}
				else if(strncmp(res,"no",2)==0){
					stop = 0;
					answer=nofriend_msg();
				}
				free(res);
			}send_TCP(s_t,answer);
			receive_msg(s_t,proms,nbprom);
			break;
		case(ACKRF):
			receive_TCP(s_t,3);//+++
			break;
		case(FRIEN):
			receive_TCP(s_t,1);//espace
			id =receive_TCP(s_t,8);//id
			receive_TCP(s_t,3);//+++
			printf("%s a accepté la demande d'ami\n",id);
			free(id);
			break;
		case(NOFRI):
			receive_TCP(s_t,1);//espace
			id =receive_TCP(s_t,8);//id
			receive_TCP(s_t,3);//+++
			printf("%s a refusé la demande d'ami\n",id);
			free(id);
			break;
		case(okLBUP):
			receive_TCP(s_t,1);//espace
			char * addr =receive_TCP(s_t,15);//addresse
			receive_TCP(s_t,1);
			char * port =receive_TCP(s_t,4);//port
			receive_TCP(s_t,1);
			printf("publicité address %s port %s\n",addr,port);
			read_msg(s_t);
			printf("\n voulez vous vous inscrire ?(yes/no)\n");
			while(stop==1){
				res=get_text(3);
				if(strncmp(res,"yes",3)==0){
					stop = 0;
					add_prom(proms,nbprom,addr,port);
				}
				else if(strncmp(res,"no",2)==0){
					stop = 0;
				}
				free(res);
			}
			free(addr);
			free(port);
			break;
		case(NOCON):
			receive_TCP(s_t,3);//+++
			printf("pas de notifications\n");
			break;
		default:printf("error\n");
	}
	return s_t;
}
/*
char * contenu_msg_old(){
	char * res = malloc(201*sizeof(char));
	int cpt=0; int i=0;
	char c=fgetc(stdin);
	do{
		while(c!='\n' || cpt<200){
			c=fgetc(stdin);
printf("!! %d %c %d\n",cpt,c,c);
			*(res+cpt)=c;
			cpt++;
		}
		if(c!='\n' || (int) c !=10){
			realloc(res, (200*(i+1) +1 )*sizeof(char));
		}
		i++;
	}while(c!='\n' || (int) c !=10);
	return res;
}*/
char ** contenu_msg(char ** res,char * id,int * r){
	//res = malloc(sizeof(char*));
	int i=1;int len;int end =0;
	char ** t;
	do{
		end = 0;
		t=realloc(res,2*(i+1)*sizeof(char*));
		res=t;
		res[i] = malloc(2*201*sizeof(char));
		res[i] = fgets(res[i],201,stdin);
/*		if(i==1 && strncmp(res[i],'\n',1)==0){
			res[i] = fgets(res[i],201,stdin);
		}
*/		len=strlen(res[i]);
		if(len==200 && res[i][len-1]=='\n'){end=1;}
		res[i][len-1]='\0';
		i++;
	}while(len>=200 && end==0);
	*r=i-1;
	return send_msg(res,id,i-1);
}
int saisie_client(int s_t,char * address, char * port){
	char * buff = get_text(5);
	char * id = malloc(9*sizeof(char));
	char *msg = malloc(250*sizeof(char));;
	int mdp;
	int r = match_c(buff);
	char ** tbuff;
//	char ** tbn;

	//free(buff);
	switch(r){
		case(CONNE):
			s_t=init_sock_TCP(address,port);
			if(s_t !=-1){
				free(get_text(1));
				buff=get_text(MAX_BUFF_TCP);
				sscanf(buff,"%s %d\n",id,&mdp);//rajouter little endian
				printf("%s, %d\n",id,mdp);
				msg=connect_msg(id,mdp);
				send_TCP(s_t,msg);
				free(buff);
			}
			break;
		case(setFRIE):
			free(get_text(1));
			buff=get_text(MAX_BUFF_TCP);
			sscanf(buff,"%s\n",id);
			msg=setfriend_msg(id);
			send_TCP(s_t,msg);
			free(buff);
			break;
		case(setMESS):
			free(get_text(1));
			buff=get_text(9);
			sscanf(buff,"%s\n",id);//rajouter little endian
			//free(get_text(MAX_BUFF_TCP));
			free(buff);
			//free(get_text(1));
			printf("Entrer le message\n");
			tbuff=malloc(sizeof(char*));
			//tbuff[0] = malloc((250)*sizeof(char));
			tbuff=contenu_msg(tbuff,id,&r);
			//send_msg(tbuff,tbn,id,r);
			//free(buff);
			for(mdp = 0;mdp<r+1;mdp++){
//printf("tbuff[%d] %s\n",mdp,tbuff[mdp]);
				send_TCP(s_t,tbuff[mdp]);
//				send_TCP(s_t,tbuff[mdp]);
				//free(tbuff[mdp]);
			}
			break;
		case(setFLOO):
			//free(get_text(MAX_BUFF_TCP));
			//printf("\nEntrer le message (200 caractères)\n");
			buff=get_text(201);
			msg=floo_msg(buff);
			send_TCP(s_t,msg);
			free(buff);
			break;
		case(setLIST):
			msg=list_msg();
			send_TCP(s_t,msg);
			break;
		case(CONSU):
			msg=consult_msg();
			send_TCP(s_t,msg);
			break;
		case(IQUIT):
			msg=deconnect_msg();
			send_TCP(s_t,msg);
			break;
	}
#if VERBOSE >= 1
	printf("%s\n",msg);
#endif
	free(id);
	return s_t;
}

void run(int s_u, int s_t,char * address, char * port) {
	int j = 0;int i;
	fd_set rfds;
	//att deconnexion
	int * proms=malloc(1*sizeof(int));
	int nbprom=0;
	while(j==0){
		FD_ZERO(&rfds);
		FD_SET(s_u, &rfds);
		FD_SET(s_t, &rfds);
		for(i=0;i<nbprom;i++){
			if(proms[i]!=-1){
//				printf("proms[%d]=%d\n",i,proms[i]);
				FD_SET(proms[i], &rfds);
			}
		}
		FD_SET(STDIN_FILENO, &rfds);
		int max=MAX(s_u,s_t);
//printf("s_u=%d\n",s_u);
//printf("s_t=%d\n",s_t);
		for(i=0;i<nbprom;i++){
			max=MAX(max,proms[i]);
		}
//printf("max=%d   nbprom=%d\n",max,nbprom);
//		if(select(MAX(max,MAX(s_u,s_t))+1, &rfds, NULL, NULL, NULL) ==-1){
		if(select(max+1, &rfds, NULL, NULL, NULL) ==-1){
			//error
		}
		for(i=0;i<nbprom;i++){
			if(proms[i]!=-1){
				if(FD_ISSET(proms[i], &rfds)){
//					printf("proms[%d]=%d\n",i,proms[i]);
					receive_udp(proms[i],address, port);
				}
			}
		}
		if(FD_ISSET(s_t, &rfds)){
#if VERBOSE >= 1
			printf("------------TCP------------\n");
#endif
			s_t=receive_msg(s_t,proms,&nbprom);

		}if(FD_ISSET(s_u, &rfds)){
#if VERBOSE >= 1
                        printf("------------UDP------------\n");
#endif
			receive_udp(s_u,address, port);
                }if(FD_ISSET(STDIN_FILENO, &rfds)){
#if VERBOSE >= 1
                        printf("------------STDIN------------\n");
#endif
			s_t=saisie_client(s_t,address,port);
                }
	}
}
int inscript(int sock,char * id, char * port,char * mdp){
	char * msg = inscription(id,port,atoi(mdp));
	send_TCP(sock,msg);
	return connect_accepted(receive_TCP(sock,8));
}
