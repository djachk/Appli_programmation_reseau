#ifndef RESULT_OK
#define RESULT_OK 0
#endif
#ifndef RESULT_FAIL
#define RESULT_FAIL 1
#endif
#ifndef RESULT_UNKNOW
#define RESULT_UNKNOW 2
#endif

#define MAX_BUFF_TCP 1024
#define MAX_BUFF_UDP 300
#define MIN_BUFF_UDP 3

int init_sock_UDP(char *);
int init_sock_TCP(char *, char *);

void receive_udp(int sock,char * address, char * port);
void send_TCP(int,char *);
void run(int,int,char * address, char * port);

int inscript(int sock,char * id, char * port,char * mdp);
