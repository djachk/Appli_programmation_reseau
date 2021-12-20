import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class ServPromoteurs implements Runnable {

	public ServerSocket socServeurC;
	
	public ServPromoteurs (ServerSocket socServeurC){
		this.socServeurC=socServeurC;
	}
	
	public void run() {
		System.out.println("Lancement du Service Promoteurs");
		try{
			while(true){	
			Socket socPromoteur=socServeurC.accept();
			ServicePromoteur serviceP=new ServicePromoteur(socPromoteur);
			Thread t=new Thread(serviceP);
			t.start();
					
			
	       }//fin while
		 }
		catch(Exception e){
			System.out.println("probleme servPromoteur"+e);
			e.printStackTrace();
		}
		}
	
}
