import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;


public class TraiterMessRecusClients {
	
	static boolean valideClient(int numClient) {
		return (numClient!=-1 && LesClients.lesClients[numClient].connecte);
	}
	
	static int traiterMessRecu(String messRecu, String adrip,int porttcp, InputStream br,OutputStream pw) {
		//System.out.println("j entre dans traitermessrecu, le message est: "+messRecu);
		
		if (messRecu.length()<8) {
			System.out.println("message recu de moins de 8 caracteres!");
			return 0;
		}
		if (messRecu.substring(0, 5).equals("REGIS")) {  ////// REGIS
			//System.out.println("je traite REGIS");
			String id=messRecu.substring(6,14);
			System.out.println("identite recue= "+id);
			int p=Integer.parseInt(messRecu.substring(15,19));
			//int mdp=Integer.parseInt(messRecu.substring(20,22));  //lecture du mdp pas correcte pour l'instant
			int entier1=(int)messRecu.charAt(20);
			int entier2=(int)messRecu.charAt(21);
			int mdp=entier2*256 + entier1;
			//System.out.println("le mot de passe est: "+mdp);
			if (LesClients.existeClient(id)||LesClients.existeClient2(adrip,porttcp)) {
				System.out.println("client existe déja");
				DialogueClient.ecrirepw(pw,"GOBYE+++");
				return -1;
			}
			int res=LesClients.creerClient(id, adrip, porttcp, p, mdp, br, pw);
			if (res==1) {
				int numClient=LesClients.numClientByadrIpPortTCP(adrip,porttcp);
				DialogueClient.ecrire(numClient,"WELCO+++");
				return 1;
			}else{
				DialogueClient.ecrirepw(pw,"GOBYE+++");
				return -1;
			}
			
		}else if (messRecu.substring(0, 5).equals("CONNE")) {   ///// CONNE
			//System.out.println("je traite CONNE");
			String id=messRecu.substring(6,14);
			//int mdp=Integer.parseInt(messRecu.substring(15,17),2);
			int entier1=(int)messRecu.charAt(15);
			int entier2=(int)messRecu.charAt(16);
			int mdp=entier2*256 + entier1;
			boolean ok=LesClients.verifierClient(id, mdp);
			if (ok) {
				int numClient=LesClients.numClientById(id);
				LesClients.lesClients[numClient].setAdrip(adrip);
				LesClients.lesClients[numClient].setPortTCP(porttcp);
				LesClients.lesClients[numClient].setInput(br);
				LesClients.lesClients[numClient].setOutput(pw);
				LesClients.lesClients[numClient].setConnecteClient();
				DialogueClient.ecrire(numClient,"HELLO+++");
				return 1;
			}else{
				System.out.println("client inconnu");
				DialogueClient.ecrirepw(pw,"GOBYE+++");
				return -1;
			}
			
		}else if (messRecu.substring(0, 5).equals("FRIE?")) {     ///// FRIE?
			//System.out.println("je traite FRIE?");
			int numClient=LesClients.numClientByadrIpPortTCP(adrip,porttcp);
			if (!valideClient(numClient)) {
				System.out.println("client non connecte");
				return 0;
			}
			String identite=LesClients.lesClients[numClient].identite;
			System.out.println("le Client demandeur est: "+identite);

			String idDest=messRecu.substring(6,14);   
			int numDest=LesClients.numClientById(idDest);
			System.out.println("j envoie la demande d amitie");
			DialogueClient.demandeAmitie(numClient,numDest);			
		
		}else if (messRecu.substring(0, 5).equals("MESS?")) {     ///// MESS?
			//System.out.println("je traite MESS");
			int numClient=LesClients.numClientByadrIpPortTCP(adrip,porttcp);
			if (!valideClient(numClient)) {
				System.out.println("client non connecte");
				return 0;
			}
			String identite=LesClients.lesClients[numClient].identite;
			System.out.println("le Client demandeur est: "+identite);
			String st_num_mess=messRecu.substring(15,19);
			int nbreMess=Integer.parseInt(st_num_mess);
			if (nbreMess<0) {
				System.out.println("nombre de messages indique incorrect");
				//DialogueClient.ecrire(numClient,"MESS<+++");
				return 0;
			}else{
				System.out.println("j attends "+nbreMess+" messages");
			}
			String messageRecu=DialogueClient.recepMessage(numClient,nbreMess);
			String idDest=messRecu.substring(6,14);
			if (!LesClients.existeClient(idDest)) {
				System.out.println("destinataire inconnu");
				DialogueClient.ecrire(numClient,"MESS<+++");
				return 0;
			}
			int numDest=LesClients.numClientById(idDest);
			if (!LesClients.lesClients[numClient].ami(numDest)) {
				System.out.println("ce client n'est pas un ami");
				DialogueClient.ecrire(numClient,"MESS<+++");
				return 0;
			}
			DialogueClient.ecrire(numClient,"MESS>+++");
			int res=LesClients.lesClients[numDest].nouveauMessRecu(numClient,messageRecu);
			int env=DialogueClient.envoyerDatagramme(numDest, 3);
			
		}else if (messRecu.substring(0, 5).equals("FLOO?")) {       ///// FLOO?
			//System.out.println("je traite FLOO");
			int numClient=LesClients.numClientByadrIpPortTCP(adrip,porttcp);
			if (!valideClient(numClient)) {
				System.out.println("client non connecte");
				return 0;
			}

			String identite=LesClients.lesClients[numClient].identite;
			System.out.println("le Client demandeur est: "+identite);

			int l=messRecu.length();
			String messInondation=messRecu.substring(6,l-3);
			LesClients.lesClients[numClient].initialiserTableauInondation();
			DialogueClient.inonder(numClient, numClient,messInondation);
			DialogueClient.ecrire(numClient,"FLOO>+++");
			
		}else if (messRecu.substring(0, 5).equals("LIST?")) {        ///// LIST?
			//System.out.println("je traite LIST");
			int numClient=LesClients.numClientByadrIpPortTCP(adrip,porttcp);
			if (!valideClient(numClient)) {
				System.out.println("client non connecte");
				return 0;
			}
			String identite=LesClients.lesClients[numClient].identite;
			System.out.println("le Client demandeur est: "+identite);

			DialogueClient.envoyerListe(numClient);
		
		}else if (messRecu.substring(0, 5).equals("CONSU")) {   ///// CONSU
			//System.out.println("je traite CONSU");
			int numClient=LesClients.numClientByadrIpPortTCP(adrip,porttcp);
			if (!valideClient(numClient)) {
				System.out.println("client non connecte");
				return 0;
			}
			String identite=LesClients.lesClients[numClient].identite;
			System.out.println("le Client demandeur est: "+identite);

			DialogueClient.repondreConsu(numClient);
		
		}else if (messRecu.substring(0, 5).equals("IQUIT")) {
			//System.out.println("je traite IQUIT");
			int numClient=LesClients.numClientByadrIpPortTCP(adrip,porttcp);
			if (!valideClient(numClient)) {
				System.out.println("client non connecte");
				return 0;
			}
			String identite=LesClients.lesClients[numClient].identite;
			System.out.println("le Client demandeur est: "+identite);

			DialogueClient.ecrire(numClient,"GOBYE+++");
			return -1;
		}
			
		else {
			System.out.println("message recu incorrect: "+messRecu);
		}
		return 1;
	}

}
