import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class ServClients implements Runnable {

	public ServerSocket socServeurC;
	
	public ServClients (ServerSocket socServeurC){
		this.socServeurC=socServeurC;
	}
	
	public void run() {
		System.out.println("Lancement du Service Clients");
		try{
			while(true){	
			Socket socClient=socServeurC.accept();
			ServiceClient serviceC=new ServiceClient(socClient);
			Thread t=new Thread(serviceC);
			t.start();
					
			
	       }//fin while
		 }
		catch(Exception e){
			System.out.println("probleme servClient"+e);
			e.printStackTrace();
		}
		}
	
}
