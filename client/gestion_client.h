#ifndef RESULT_OK
#define RESULT_OK 0
#endif
#ifndef RESULT_FAIL
#define RESULT_FAIL 1
#endif

#define MAXPSEUDO 8
#define MAXMDP 65535

struct client {
	char * identifiant;
	int port;
	int mdp;
};

typedef struct client client;

int createClient(client * c);
void freeClient(client * c);
void setPort(client * c, int p);
