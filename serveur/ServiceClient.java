import java.net.*;
import java.util.Scanner;
import java.io.*;
import java.lang.*;

public class ServiceClient implements Runnable{
	
	public Socket socClient;

	
    public ServiceClient(Socket socClient) {
    	this.socClient=socClient;
    }
    
	public void run() {
		System.out.println("Nouvelle Connexion");
		try{
				//BufferedReader br=new BufferedReader(new InputStreamReader(socClient.getInputStream(),"ASCII"));
				//PrintWriter pw=new PrintWriter(new OutputStreamWriter(socClient.getOutputStream(),"ASCII"));
				//InputStreamReader br=new InputStreamReader(socClient.getInputStream(),"ASCII");
				//OutputStreamWriter pw=new OutputStreamWriter(socClient.getOutputStream(),"ASCII");
				InputStream br=socClient.getInputStream();
				OutputStream pw=socClient.getOutputStream();
				
				System.out.println("L'adresse IP du client est "+ socClient.getInetAddress().getHostAddress());
				String adrip=socClient.getInetAddress().getHostAddress();
				//System.out.println("adresse = "+adrip);
				int portTCP=socClient.getPort();
				System.out.println("le port TCP du Client est= "+portTCP);
				while(true) {
					String messRecu="";
					int c;
					while(true){
						//String mess=br.readLine();
						//messRecu+=mess;
						c=br.read();
						messRecu+=(char)c;
						int l=messRecu.length();
						if (l>=3 && messRecu.substring(l-3,l).equals("+++")) {
								break;
						}
						//if (l>=3 && messRecu.charAt(l-1)=='+' 
						//		&& messRecu.charAt(l-2)=='+' && messRecu.charAt(l-3)=='+') {
						//		break;
						//}
					}
					System.out.println("j'ai recu: "+messRecu+" (message de longueur "+messRecu.length()+")");
					//if (messRecu.charAt(0)=='X') break;
					int res=0;
					try{
					res=TraiterMessRecusClients.traiterMessRecu(messRecu,adrip,portTCP,br,pw);
					}catch(Exception e){
						//e.printStackTrace();
						System.out.println("probleme lors du traitement du message: "+messRecu);
						System.out.println("mais le service au client continue...");
					}
					if (res==-1) break;
				}
				System.out.println("je ferme la connexion Client");
				br.close();
				pw.close();
				socClient.close();
		}
		catch(Exception e){
			System.out.println("ServiceClient, Probleme de connexion "+e);
			//e.printStackTrace();
		}
			
		
	}

}
