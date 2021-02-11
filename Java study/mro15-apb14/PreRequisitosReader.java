import java.io.*;
import java.util.*;
/*Leitor de pré-requisitos*/
public class PreRequisitosReader{
	private Scanner in;
	public PreRequisitosReader(String file){
		try{
			this.in = new Scanner (new FileReader(file)).useDelimiter(",|\n");
		}
		catch(IOException e){
			System.out.println(e);
		}
	}
	//Lê o csv de pré-requisitos e retorna um Vector de matérias e seus
	//respectivos pré-recsitos
	public Vector<Materia> read(){
		Vector<Materia> requisitos = new Vector<Materia>();
		this.in.nextLine();
		this.in.nextLine();
		Materia disc= new Materia();
		String ultimaMateria = "";
		while (this.in.hasNext()){
			for (int i=0; i<13; i++){
				if(this.in.hasNext()){
					String aux = this.in.next();
					if(i==4){
						if(!(aux.equals(ultimaMateria))){
							ultimaMateria = aux;
							if (disc.codigo!=null){
								requisitos.add(disc);
							}
							disc= new Materia();
							disc.codigo = aux;
						}
					}
					if(i==6){
						Materia aux1 = new Materia();
						aux1.codigo = aux;
						disc.preReq.add(aux1);
					}
				}
			}
		}
		return requisitos;
	}
}
