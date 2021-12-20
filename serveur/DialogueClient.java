import java.io.*;
import java.net.*;
import java.nio.charset.Charset;


public class DialogueClient {
	
	    static String intEnChaine3(int ind) {
	    	int unite=ind%10;
	    	int dizaine=(ind%100)/10;
	    	int centaine=ind/100;
	    	String res="";
	    	return res + (char) ('0'+centaine) + (char) ('0'+dizaine) + (char) ('0'+unite);
	    }
	   
	    static String intEnChaine4(int ind) {
	    	int unite=ind%10;
	    	int dizaine=(ind%100)/10;
	    	int centaine=(ind%1000)/100;
	    	int millier=ind/1000;
	    	String res="";
	    	return res + (char) ('0'+millier) + (char) ('0'+centaine) + (char) ('0'+dizaine) + (char) ('0'+unite);
	    }
	    
	    static String encoderAdrIPMulti(String adrip) {
	    	String adrIPMulti="";
	    	for (int i=0;i<15;i++) {
	    		if (i<adrip.length()) {
	    			adrIPMulti+=adrip.charAt(i);
	    		}else {
	    			adrIPMulti+='#';
	    		}
	    	}
	    	return adrIPMulti;
	    }	
	    
	    
	    static void ecrirepw(OutputStream pw, String st) {
	    	System.out.println("j envoie le message: "+st);
	    	byte[] b = st.getBytes(Charset.forName("ASCII"));
	    	try {
				pw.write(b);
				pw.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    }
	    
	    static void ecrire(int numClient, String st) {
	    	String identite=LesClients.lesClients[numClient].identite;
	    	System.out.println("j envoie le message: "+st+" au client: "+identite);
	    	OutputStream pw=LesClients.lesClients[numClient].pw;
	    	byte[] b = st.getBytes(Charset.forName("ASCII"));
	    	try {
				pw.write(b);
				pw.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    }
	    
	    static int envoyerDatagramme(int numDest, int type) {
	    	try{
			      DatagramSocket dso=new DatagramSocket();
			      String st;
			      byte[] data=new byte[3];
			      switch (type){
			      	case 0: data[0]=(byte)0; break;
			      	case 1: data[0]=(byte)1; break;
			      	case 2: data[0]=(byte)2; break;
			      	case 3: data[0]=(byte)3; break;
			      	case 4: data[0]=(byte)4; break;
			      	case 5: data[0]=(byte)5; break;
			      	default: data[0]=(byte)0;
			      }
			      int total=LesClients.lesClients[numDest].sommeFlux();
			      int entier1=total%256;
			      int entier2=total/256;
			      data[1]=(byte)entier1;
			      data[2]=(byte)entier2;
			      //byte[] data=st.getBytes("ASCII");     
			      String adrIP=LesClients.lesClients[numDest].adresseIp;
			      int portUDP=LesClients.lesClients[numDest].portUDP;
			      InetSocketAddress ia=new InetSocketAddress(adrIP,portUDP);
			      DatagramPacket paquet=new DatagramPacket(data,data.length,ia);
			      dso.send(paquet);
			      String identite=LesClients.lesClients[numDest].identite;
			      String mess=new String(data,"ASCII");
			      //System.out.println("longueur du string UDP:"+mess.length());
			      System.out.println("j'envoie un UDP de type "+type+" au client: "+identite+ " ,attention aux caracteres non imprimables..., de contenu "+mess );
			    }
			    catch(Exception e){
			      System.out.println("pb a l envoi UDP");	
			      e.printStackTrace();
			      return 0;
			    }
	    	return 1;
	    }

	    static String recevoirMess(int numClient) {
	    	InputStream br=LesClients.lesClients[numClient].br;
	    	String messRecu="";
			int c;
			while(true){
				try {
					c=br.read();
				} catch (IOException e) {
					System.out.println("erreur en lecture du message");
					e.printStackTrace();
					return null;
				}
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
			String identite=LesClients.lesClients[numClient].identite;
			System.out.println("j'ai recu le message: "+messRecu+" du client: "+identite);
			int l=messRecu.length();
			return messRecu.substring(0,l-3);
	    }
	    
	    static String recevoirMessPartiel(int numClient) {
	    	String messRecu=recevoirMess(numClient);
	    	if (messRecu==null) return null;
	    	if (!messRecu.substring(0,5).equals("MENUM")) {
	    		System.out.println("message MENUM attendu");
	    		return null;
	    	}
	    	String portionMessRecue=messRecu.substring(11,messRecu.length());
			System.out.println("j'ai recu la portion de message: "+portionMessRecue);
			return portionMessRecue;
	    }
	
	    
		static void demandeAmitie(int numClient,int numDest) {
			OutputStream pw=LesClients.lesClients[numClient].pw;
			if (numDest==-1) {
				System.out.println("destinataire inconnu");
				ecrire(numClient,"FRIE<+++");
				return;
			}else {
				int res=LesClients.lesClients[numDest].nouvelleInvit(numClient);
				if (res==1) {
					int env=envoyerDatagramme(numDest,0); //correction bug 0 et non 1
					if (env==1) {ecrire(numClient,"FRIE>+++");} else {ecrire(numClient,"FRIE<+++");}
				}else {
					ecrire(numClient,"FRIE<+++");
				}
			}
			//juste pour test!!!!
			//LesClients.lesClients[numClient].ajouterAmi(numDest);
			//LesClients.lesClients[numDest].ajouterAmi(numClient);
		}
		
		static String recepMessage(int numClient,int nbreMess) {
			OutputStream pw=LesClients.lesClients[numClient].pw;
			String messageRecu="";
			for (int i=0; i<nbreMess;i++){
				String portionMessRecue=recevoirMessPartiel(numClient);
				if (portionMessRecue==null) {
					ecrire(numClient,"MESS<+++");
					break;
				}
				messageRecu+=portionMessRecue;
			}
			System.out.println("j ai recu le message total :"+messageRecu);
			//ecrire(numClient,"MESS>+++");
			//int res=LesClients.lesClients[numDest].nouveauMessRecu(numClient,messageRecu);
			//int env=envoyerDatagramme(numDest, 3);
			return messageRecu;
			
		}
		
		static void inonder(int initiateur, int numClient,String messInondation) {
			String identite=LesClients.lesClients[numClient].identite;
			for (int i=0; i<LesClients.lesClients[numClient].nbreAmis;i++) {
				System.out.println("le client "+identite+" a " + LesClients.lesClients[numClient].nbreAmis+ " amis");
				int ami=LesClients.lesClients[numClient].amis[i];
				if (LesClients.lesClients[initiateur].dejaInonde(ami)==false) {
					System.out.println("j inonde client " + ami);
					LesClients.lesClients[initiateur].setInonde(ami);
					int res=LesClients.lesClients[ami].nouveauMessInondation(initiateur, messInondation);
					if (res==1) {envoyerDatagramme(ami, 4);}
					inonder(initiateur,ami,messInondation);
				}
			}
			
		}
		
		static void envoyerListe(int numClient) {
			OutputStream pw=LesClients.lesClients[numClient].pw;
			int numItem=LesClients.nbreClients;
			String stringNumItem=intEnChaine3(numItem);
			String envoi="RLIST " +stringNumItem+"+++";
			ecrire(numClient,envoi);
			for (int i=0;i<numItem;i++) {
				String idClient=LesClients.lesClients[i].identite;
				String envoi1="LINUM " +idClient+"+++";
				ecrire(numClient,envoi1);
			}
		}
		
		static void repondreConsu(int numClient) {
			OutputStream pw=LesClients.lesClients[numClient].pw;
			if (LesClients.lesClients[numClient].sommeFlux()==0) {
				ecrire(numClient,"NOCON+++" );
				return;
			}
			for (int i=0; i<Communs.nbreMaxFlux; i++) {
				//flux invitations
				if (LesClients.lesClients[numClient].fluxInvitRecues[i].aLire) {
					int numExp=LesClients.lesClients[numClient].fluxInvitRecues[i].expediteur;
					String expediteur=LesClients.lesClients[numExp].identite;
					ecrire(numClient,"EIRF> "+expediteur+"+++");
					String reponse=recevoirMess(numClient);
					ecrire(numClient,"ACKRF+++");
					if (reponse.substring(0,5).equals("OKIRF")) {
						LesClients.lesClients[numExp].nouvelleRepInvitation(numClient,true);
						LesClients.lesClients[numClient].ajouterAmi(numExp);
						LesClients.lesClients[numExp].ajouterAmi(numClient);
						envoyerDatagramme(numExp,1);
						
					} else if (reponse.substring(0,5).equals("NOKRF")) {
						LesClients.lesClients[numExp].nouvelleRepInvitation(numClient,false);
						envoyerDatagramme(numExp,2);
					}
					
					LesClients.lesClients[numClient].fluxInvitRecues[i].aLire=false;
				}
			 }
			
				//flux reponses invitations
			for (int i=0; i<Communs.nbreMaxFlux; i++) {
				if (LesClients.lesClients[numClient].fluxRepInvitations[i].aLire) {
					int numExp=LesClients.lesClients[numClient].fluxRepInvitations[i].expediteur;
					String expediteur=LesClients.lesClients[numExp].identite;
					if (LesClients.lesClients[numClient].fluxRepInvitations[i].accept) {
						ecrire(numClient,"FRIEN "+expediteur+"+++");

					} else {
						ecrire(numClient,"NOFRI "+expediteur+"+++");
					}
					LesClients.lesClients[numClient].fluxRepInvitations[i].aLire=false;
				}
			}	
		       
			
			
			//	flux Messages Inondations;
			for (int i=0; i<Communs.nbreMaxFlux; i++) {
				if (LesClients.lesClients[numClient].fluxMessInondations[i].aLire) {
					int numExp=LesClients.lesClients[numClient].fluxMessInondations[i].expediteur;
					String expediteur=LesClients.lesClients[numExp].identite;
					String contenu=LesClients.lesClients[numClient].fluxMessInondations[i].contenu;
					ecrire(numClient,"OOLF> "+expediteur+" "+contenu+"+++");
					
					LesClients.lesClients[numClient].fluxMessInondations[i].aLire=false;
				}
			}
			
			// flux Messages recus
			for (int i=0; i<Communs.nbreMaxFlux; i++) {
				if (LesClients.lesClients[numClient].fluxMessRecus[i].aLire) {
					int numExp=LesClients.lesClients[numClient].fluxMessRecus[i].expediteur;
					String expediteur=LesClients.lesClients[numExp].identite;
					String contenu=LesClients.lesClients[numClient].fluxMessRecus[i].contenu;
					int nbrefois=(contenu.length()/Communs.longMaxMessage) + 1;
					int curseur=0, debut=0,fin=0;
					ecrire(numClient,"SSEM> "+expediteur+" "+intEnChaine4(nbrefois)+"+++");
					for (int j=0;j<nbrefois-1;j++) {
						debut=j*Communs.longMaxMessage;
						fin=(j+1)*Communs.longMaxMessage;
						String envoi=contenu.substring(debut,fin);
						ecrire(numClient,"MUNEM "+intEnChaine4(j)+" "+envoi+"+++");
						curseur=fin;
					}
					int l=contenu.length();
					String envoi1=contenu.substring(fin,l);
					ecrire(numClient,"MUNEM "+intEnChaine4(nbrefois-1)+" "+envoi1+"+++");
					
					LesClients.lesClients[numClient].fluxMessRecus[i].aLire=false;
				}
			}
			
			// flux Messages Promoteurs
			for (int i=0; i<Communs.nbreMaxFlux; i++) {
				if (LesClients.lesClients[numClient].fluxMessPromoteurs[i].aLire) {
				String adrIPMulti=encoderAdrIPMulti(LesClients.lesClients[numClient].fluxMessPromoteurs[i].adrIPMulti);
				int portMulti=LesClients.lesClients[numClient].fluxMessPromoteurs[i].portMulti;
				String messPub=LesClients.lesClients[numClient].fluxMessPromoteurs[i].messPub;
				
				ecrire(numClient,"LBUP> "+adrIPMulti+" "+intEnChaine4(portMulti)+" "+messPub+"+++");
				LesClients.lesClients[numClient].fluxMessPromoteurs[i].aLire=false;
				}
			}
			
			ecrire(numClient,"NOCON+++" );
		}

}
