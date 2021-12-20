import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;


public class LesClients {

	static Client[] lesClients=new Client[Communs.nbreMaxClients];
	static int nbreClients=0;
	
	
	static int numClientByadrIpPortTCP(String adrIp, int portTcp) {
		int res=-1;
		for(int i=0;i<nbreClients;i++) {
			if (lesClients[i].adresseIp.equals(adrIp) && lesClients[i].portTCP==portTcp) {
				res=i;
				break;
			}
		}
		return res;
	}
	
	static int numClientById(String id) {
		int res=-1;
		for(int i=0;i<nbreClients;i++) {
			if (lesClients[i].identite.equals(id)) {
				res=i;
				break;
			}
		}
		return res;
	}
	
	static boolean existeClient(String id) {
		int num=numClientById(id);
		return (num!=-1);
	}
	
	static boolean existeClient2(String adrip, int porttcp) {
		int num=numClientByadrIpPortTCP(adrip,porttcp);
		return (num!=-1);
	}
	
	
	static boolean verifierClient(String id, int mdp) {
		boolean res=false;
		int num=numClientById(id);
		if ((num!=-1) && (lesClients[num].motDePasse==mdp)) {
			res=true;
		}
		return res;
	}
	
	static void afficherClients(){
		for(int i=0;i<nbreClients;i++) {
			System.out.println("num de client: "+i);
			System.out.println("identite: "+lesClients[i].identite);
			System.out.println("adrip: "+lesClients[i].adresseIp);
			System.out.println("portTCP: "+lesClients[i].portTCP);
			System.out.println("portUdp: "+lesClients[i].portUDP);
			System.out.println("mot de passe: "+lesClients[i].motDePasse);
			System.out.println(" ");
		}
	}
	
	static void afficherClient(int numClient){
			System.out.println("num de client: "+numClient);
			System.out.println("identite: "+lesClients[numClient].identite);
			System.out.println("adrip: "+lesClients[numClient].adresseIp);
			System.out.println("portTCP: "+lesClients[numClient].portTCP);
			System.out.println("portUdp: "+lesClients[numClient].portUDP);
			System.out.println("mot de passe: "+lesClients[numClient].motDePasse);
			System.out.println(" ");
		
	}
	
	static int creerClient(String identite, String adresseIp, int portTCP, int portUDP, int motDePasse, InputStream br, OutputStream pw){
		if ((nbreClients==Communs.nbreMaxClients) || existeClient(identite)) {
			return 0;
		}
		lesClients[nbreClients]=new Client(identite,adresseIp,portTCP, portUDP,motDePasse, br, pw);
		System.out.println("Creation d un nouveau client");
		afficherClient(nbreClients);
		nbreClients++;
		//afficherClients();
		return 1;
	}
	
	
	
}
