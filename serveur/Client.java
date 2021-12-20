import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;


public class Client {
	
	String identite;
	String adresseIp;
	int portTCP;
	int portUDP;
	int motDePasse;
	int[] amis;
	int nbreAmis;
	boolean connecte;
	InputStream br;
	OutputStream pw;
	//BufferedReader br;
	//PrintWriter pw;
	Flux.InvitRecue[] fluxInvitRecues;
	int nbreInvitaLire;
	Flux.MessRecu[] fluxMessRecus;
	int nbreMessaLire;
	Flux.MessInondation[] fluxMessInondations;
	int nbreMessInondaLire;
	Flux.RepInvitation[] fluxRepInvitations;
	int nbreRepInvitaLire;
	Flux.MessPromoteur[] fluxMessPromoteurs;
	int nbreMessPromoteurs;

	boolean[] tabInondation;
	
	public Client(String identite, String adresseIp, int portTCP, int portUDP, int motDePasse,InputStream br, OutputStream pw){
		this.identite=identite;
		this.adresseIp=adresseIp;
		this.portTCP=portTCP;
		this.portUDP=portUDP;
		this.motDePasse=motDePasse;
		this.br=br;
		this.pw=pw;
		amis=new int[Communs.nbreMaxClients];
		nbreAmis=0;
		connecte=true;
		tabInondation=new boolean[Communs.nbreMaxClients];
		for (int i=0;i<tabInondation.length;i++) {
			tabInondation[i]=false;
		}		
		fluxInvitRecues=new Flux.InvitRecue[Communs.nbreMaxFlux];
		for (int i=0;i<fluxInvitRecues.length;i++) {
			fluxInvitRecues[i]=new Flux.InvitRecue();
		}
		nbreInvitaLire=0;
		
		fluxMessRecus=new Flux.MessRecu[Communs.nbreMaxFlux];
		for (int i=0;i<fluxMessRecus.length;i++) {
			fluxMessRecus[i]=new Flux.MessRecu();
		}
		nbreMessaLire=0;
		
		fluxMessInondations=new Flux.MessInondation[Communs.nbreMaxFlux];
		for (int i=0;i<fluxMessInondations.length;i++) {
			fluxMessInondations[i]=new Flux.MessInondation();
		}
		nbreMessInondaLire=0;
		
		fluxRepInvitations=new Flux.RepInvitation[Communs.nbreMaxFlux];
		for (int i=0;i<fluxRepInvitations.length;i++) {
			fluxRepInvitations[i]=new Flux.RepInvitation();
		}
		nbreRepInvitaLire=0;
		
		fluxMessPromoteurs=new Flux.MessPromoteur[Communs.nbreMaxFlux];
		for (int i=0;i<fluxMessPromoteurs.length;i++) {
			fluxMessPromoteurs[i]=new Flux.MessPromoteur();
		}
		nbreMessPromoteurs=0;
		
	}
	
	public boolean connecteClient() {
		return connecte;
	}

	public void setConnecteClient() {
		connecte=true;
	}
	
	public void setDeconnecteClient() {
		connecte=false;
	}	
	
	public void setAdrip(String adrIp){
		adresseIp=adrIp;
	}

	public void setPortTCP(int porttcp){
		portTCP=porttcp;
	}
	
	public void setOutput(OutputStream pw) {
		this.pw=pw;
	}
	
	public void setInput(InputStream br) {
		this.br=br;
	}
	
	
	public boolean ami(int id) {
		boolean res=false;
		for (int i=0;i<nbreAmis;i++) {
			if (amis[i]==id) {res=true; break;}
		}
		return res;
	}
	
	public synchronized void ajouterAmi(int num) {
		if (ami(num)) {return;}
		amis[nbreAmis]=num;
		nbreAmis++;
	}
	
	public int caseLibreInvitRecue() {
		int res=-1;
		for (int i=0;i<fluxInvitRecues.length;i++) {
			if (fluxInvitRecues[i].aLire==false) {res=i; break;}
		}
		return res;
	}
	
	public synchronized int nouvelleInvit(int numInvitant) {
		int caseLibre=caseLibreInvitRecue();
		if (caseLibre==-1) {return 0;}
		fluxInvitRecues[caseLibre].aLire=true;
		fluxInvitRecues[caseLibre].expediteur=numInvitant;
		//nbreInvitaLire++;
		return 1;
	}
	
	public int caseLibreMessRecu() {
		int res=-1;
		for (int i=0;i<fluxMessRecus.length;i++) {
			if (fluxMessRecus[i].aLire==false) {res=i; break;}
		}
		return res;
	}
	
	public synchronized int nouveauMessRecu(int numExp, String contenu) {
		int caseLibre=caseLibreMessRecu();
		if (caseLibre==-1) {return 0;}
		fluxMessRecus[caseLibre].aLire=true;
		fluxMessRecus[caseLibre].expediteur=numExp;
		fluxMessRecus[caseLibre].contenu=contenu;
		//nbreMessaLire++;
		return 1;
	}
	
	public int caseLibreMessInondation() {
		int res=-1;
		for (int i=0;i<fluxMessInondations.length;i++) {
			if (fluxMessInondations[i].aLire==false) {res=i; break;}
		}
		return res;
	}
	
	public synchronized int nouveauMessInondation(int numExp, String contenu) {
		int caseLibre=caseLibreMessInondation();
		if (caseLibre==-1) {return 0;}
		fluxMessInondations[caseLibre].aLire=true;
		fluxMessInondations[caseLibre].expediteur=numExp;
		fluxMessInondations[caseLibre].contenu=contenu;
		//nbreMessInondaLire++;
		return 1;
	}
	
	public int caseLibreRepInvitation() {
		int res=-1;
		for (int i=0;i<fluxRepInvitations.length;i++) {
			if (fluxRepInvitations[i].aLire==false) {res=i; break;}
		}
		return res;
	}
	
	public int caseLibreMessPromot() {
		int res=-1;
		for (int i=0;i<fluxMessPromoteurs.length;i++) {
			if (fluxMessPromoteurs[i].aLire==false) {res=i; break;}
		}
		return res;
	}	
	
	
	public synchronized int nouvelleRepInvitation(int numExp, boolean accept) {
		int caseLibre=caseLibreRepInvitation();
		if (caseLibre==-1) {return 0;}
		fluxRepInvitations[caseLibre].aLire=true;
		fluxRepInvitations[caseLibre].expediteur=numExp;
		fluxRepInvitations[caseLibre].accept=accept;
		//nbreRepInvitaLire++;
		return 1;
	}
	
	public synchronized int nouveauMessPub(String adripmulti, int portmulti, String mess) {
		int caseLibre=caseLibreMessPromot();
		if (caseLibre==-1) {return 0;}
		fluxMessPromoteurs[caseLibre].aLire=true;
		fluxMessPromoteurs[caseLibre].adrIPMulti=adripmulti;
		fluxMessPromoteurs[caseLibre].portMulti=portmulti;
		fluxMessPromoteurs[caseLibre].messPub=mess;
		//nbreMessPromoteurs++;
		return 1;
	}	
	
	public void initialiserTableauInondation() {
		for (int i=0;i<tabInondation.length;i++) {
			tabInondation[i]=false;
		}		
	}
	
	public boolean dejaInonde(int num) {
		return tabInondation[num];
	}
	
	public void setInonde(int num) {
		tabInondation[num]=true;
	}	
	
	public synchronized int sommeFlux() {
		nbreInvitaLire=0;
		for (int i=0;i<fluxInvitRecues.length;i++) {
			if (fluxInvitRecues[i].aLire==true) {nbreInvitaLire++;}
		}
		nbreMessaLire=0;
		for (int i=0;i<fluxMessRecus.length;i++) {
			if (fluxMessRecus[i].aLire==true) {nbreMessaLire++;}
		}
		nbreMessInondaLire=0;
		for (int i=0;i<fluxMessInondations.length;i++) {
			if (fluxMessInondations[i].aLire==true) {nbreMessInondaLire++;}
		}
		nbreRepInvitaLire=0;
		for (int i=0;i<fluxRepInvitations.length;i++) {
			if (fluxRepInvitations[i].aLire==true) {nbreRepInvitaLire++;}
		}
		nbreMessPromoteurs=0;
		for (int i=0;i<fluxMessPromoteurs.length;i++) {
			if (fluxMessPromoteurs[i].aLire==true) {nbreMessPromoteurs++;}
		}
		return nbreInvitaLire + nbreMessaLire + nbreMessInondaLire + nbreRepInvitaLire + nbreMessPromoteurs;
	}
	
}
