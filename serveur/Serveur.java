import java.net.ServerSocket;
import java.net.Socket;

public class Serveur {
 
	
	
	public static void main(String[] args) {
		int portClients=8000, portPromoteurs=9000;
		if (args.length > 0) {portClients=Integer.parseInt(args[0]);}
		if (args.length > 1) {portPromoteurs=Integer.parseInt(args[1]);}
		try{
			ServerSocket socServeurC=new ServerSocket(portClients);
			System.out.println("Le port local pour les clients est "+ socServeurC.getLocalPort());
			ServerSocket socServeurP=new ServerSocket(portPromoteurs);
			System.out.println("Le port local pour les promoteurs est "+ socServeurP.getLocalPort());
			
			//System.out.println("la machine locale est " + socServeurC.getHostName());
    
			ServClients servC=new ServClients(socServeurC);
			Thread threadservC=new Thread(servC);
			threadservC.start();
			
			ServPromoteurs servP=new ServPromoteurs(socServeurP);
			Thread threadservP=new Thread(servP);
			threadservP.start();
			
		}
		catch(Exception e){
			System.out.println("probleme serveur "+e);
			e.printStackTrace();
		}

	}

}
