import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;


public class TraiterMessRecusPromoteurs {
	
	static String decoderAdrMulti(String chaineAdrIP) {
		String adrIPMulti="";
		for(int i=0; i<chaineAdrIP.length();i++) {
			char c=chaineAdrIP.charAt(i);
			if (c!='#') {adrIPMulti+=c;}
		}
		return adrIPMulti;
	}
	
	static int traiterMessRecuPromot(String messRecu, String adrip,int porttcp, InputStream br,OutputStream pw) {
		//System.out.println("j entre dans traitermessrecu, le message est: "+messRecu);
		
		if (messRecu.length()<8) {
			System.out.println("message recu d'un promoteur de moins de 8 caracteres!");
			return 0;
		}
		if (messRecu.substring(0, 5).equals("PUBL?")) {  ////// PUBL?
			//System.out.println("je traite PUBL?");
			String chaineAdrIP=messRecu.substring(6,21);
			String adrIPMulti=decoderAdrMulti(chaineAdrIP);
			int portMulti=Integer.parseInt(messRecu.substring(22,26));
			String messPub=messRecu.substring(27, messRecu.length()-3);
			for(int i=0;i<LesClients.nbreClients;i++) {
				LesClients.lesClients[i].nouveauMessPub(adrIPMulti, portMulti, messPub);
				DialogueClient.envoyerDatagramme(i, 5);
			}
			DialogueClient.ecrirepw(pw,"PUBL>+++");
			
		} else {
			System.out.println("message recu incorrect: "+messRecu);
		}
		return 1;
	}
}
	