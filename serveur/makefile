
all: Serveur.class 

Serveur.class: Serveur.java ServClients.class Communs.class ServPromoteurs.class 
	javac $<

ServClients.class: ServClients.java ServiceClient.class TraiterMessRecusClients.class Communs.class
	javac $<

ServiceClient.class: ServiceClient.java TraiterMessRecusClients.class Communs.class
	javac $<



ServPromoteurs.class: ServPromoteurs.java ServicePromoteur.class TraiterMessRecusPromoteurs.class Communs.class
	javac $<

ServicePromoteur.class: ServicePromoteur.java TraiterMessRecusPromoteurs.class Communs.class
	javac $<


TraiterMessRecusClients.class: TraiterMessRecusClients.java DialogueClient.class LesClients.class Client.class Flux.class Communs.class
	javac $<

TraiterMessRecusPromoteurs.class: TraiterMessRecusPromoteurs.java DialogueClient.class LesClients.class Client.class Flux.class Communs.class
	javac $<

%.class: %.java
	javac $<



