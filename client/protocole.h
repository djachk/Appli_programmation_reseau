#ifndef RESULT_OK
#define RESULT_OK 0
#endif
#ifndef RESULT_FAIL
#define RESULT_FAIL 1
#endif
#ifndef RESULT_UNKNOW
#define RESULT_UNKNOW 2
#endif

#define ISPROM 10
#define ISNOTIF 11


enum msg_client_type {REGIS,CONNE,setFRIE,setMESS,MENUM,setFLOO,setLIST,CONSU,OKIRF,NOKRF};
enum msg_serv_type {WELCO,GOBYE,HELLO,okFRIE,noFRIE,okMESS,noMESS,okFLOO,RLIST,LINUM,okSSEM,MUNEM,okOOLF,okEIRF,ACKRF,FRIEN,NOFRI,okLBUP,NOCON,IQUIT};

//traitement des messages udp --------------

//2eme securité pour determine si le paquet vient du serveur ou d'un promoteur
int protocole_udp();
//analyse la notification et l'affiche
void protocole_notif();
//affiche le message du promoteur
void protocole_prom();

//traitement des messages tcp --------------
//inscription accepté ou refusée
int connect_accepted(char * msg);
//analyse le type du message
int match_r(char * );
int match_c(char * );

//traitement du canal d'entrée du client
//crée le message d'inscription
char * inscription(char * id, char * port,int mdp);
//
//generation de message
//int send_msg_old(char ** tbuff, char * id,char * msg);
int send_msg_old(char ** tbuff, char * id,int s);
char ** send_msg(char ** tbuff, char * id,int s);
char * okfriend_msg();
char * nofriend_msg();
char * floo_msg(char * msg);
//char * connect_msg(char * id,char * mdp);
char * connect_msg(char * id,int mdp);
char * deconnect_msg();
char * setfriend_msg(char * id);
char * list_msg();
char * consult_msg();
