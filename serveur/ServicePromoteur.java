import java.net.*;
import java.util.Scanner;
import java.io.*;
import java.lang.*;

public class ServicePromoteur implements Runnable{
	
	public Socket socPromoteur;

	
    public ServicePromoteur(Socket socPromoteur) {
    	this.socPromoteur=socPromoteur;
    }
    
	public void run() {
		System.out.println("Nouvelle Connexion Promoteur");
		try{
				InputStream br=socPromoteur.getInputStream();
				OutputStream pw=socPromoteur.getOutputStream();
				
				System.out.println("L'adresse IP du promoteur est "+ socPromoteur.getInetAddress().getHostAddress());
				String adrip=socPromoteur.getInetAddress().getHostAddress();
				int portTCP=socPromoteur.getPort();
				System.out.println("le port TCP du Promoteur est= "+portTCP);
				while(true) {
					String messRecu="";
					int c;
					while(true){
							c=br.read();
						messRecu+=(char)c;
						int l=messRecu.length();
						if (l>=3 && messRecu.substring(l-3,l).equals("+++")) {
								break;
						}
					}
					System.out.println("j'ai recu: "+messRecu+" (message de longueur "+messRecu.length()+")");
					int res=0;
					try{
					res=TraiterMessRecusPromoteurs.traiterMessRecuPromot(messRecu,adrip,portTCP,br,pw);
					}catch(Exception e){
						//e.printStackTrace();
						System.out.println("probleme lors du traitement du message: "+messRecu);
						System.out.println("mais le service au promoteur continue...");
					}
					if (res==-1) break;
				}
				System.out.println("je ferme la connexion Promoteur");
				br.close();
				pw.close();
				socPromoteur.close();
		}
		catch(Exception e){
			System.out.println("Ici ServicePromoteur, Probleme de connexion"+e);
			//e.printStackTrace();
		}
			
		
	}

}

