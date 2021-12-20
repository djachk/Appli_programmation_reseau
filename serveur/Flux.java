
public class Flux {
	
	public static class InvitRecue {
		boolean aLire;
		int expediteur;;
		
		public InvitRecue (){
			this.aLire=false;
		}
	}

	public static class MessRecu {
		boolean aLire;
		int expediteur;
		String contenu;
		
		public MessRecu (){
			this.aLire=false;
		}
	}
	
	public static class MessInondation {
		boolean aLire;
		int expediteur;
		String contenu;
		
		public MessInondation (){
			this.aLire=false;
		}	
	}
	
	public static class RepInvitation {
		boolean aLire;
		int expediteur;
		boolean accept;
		
		public RepInvitation (){
			this.aLire=false;
			this.accept=false;
		}	
	}
	
	public static class MessPromoteur {
		String adrIPMulti;
		int portMulti;
		String messPub;
		boolean aLire;
		
		public MessPromoteur() {
			this.aLire=false;
		}
		
	}
}
