import java.util.*;
import java.io.*;
public class DiscOfertadasReader{
	private Scanner in;
	public DiscOfertadasReader(String file){
		try{
			this.in = new Scanner (new FileReader(file)).useDelimiter(",|\n");
		}
		catch(IOException e){
			System.out.println(e);
		}
	}
	//Retorna um Vector de matérias ofertadas
	public Vector<Materia> read(){
		Vector<Materia> ofertadas = new Vector<Materia>();
		this.in.nextLine();
		this.in.nextLine();
    int cont =0;
    String t = "";
    String c = "";
    String auxx = "";
    String auxxx = "";
    int ct = 0;
    Materia disc = new Materia();
		while (this.in.hasNext()){
			for (int i=0; i<20; i++){
				if(this.in.hasNext()){
					String aux = this.in.next();
            if (i==0 || i==1 || i==2 || i==4 || i==5 || i==6){
                if (i==0)
                    auxx = aux;
                if (i==1)
                    auxxx = aux;
                if((i==2  && (!(aux.equals(t)) || !(auxx.equals(c))))){
                        if (ct != 0)
                            ofertadas.add(disc);
                        disc = new Materia();
                        disc.nome = auxxx;
                        disc.turma = aux;
                        disc.codigo = auxx;
                        t = aux;
                        c = auxx;
                        ct = 1;
                }
                if(i==4 && disc.dia1==null)
						        disc.dia1 = aux;
                else
                    if(i==4)
                        disc.dia2 = aux;
                if(i==5 && disc.horario_ini1==null)
						        disc.horario_ini1 = aux;
                else
                    if (i==5)
                        disc.horario_ini2 = aux;
                if(i==6 && disc.horario_fim1==null){
						        disc.horario_fim1 = aux;
                }else
                    if (i==6)
                        disc.horario_fim2 = aux;
            }
          }
        }
    }
    ofertadas.add(disc);
		return ofertadas;
	}
}
