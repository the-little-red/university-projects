import java.util.*;
import java.io.*;
/*Lê o csv de matrícula do aluno*/
public class MatriculasReader{
	private Scanner in;
	public MatriculasReader(String file){
		try{
			this.in = new Scanner(new FileReader(file)).useDelimiter(",|\n");
		}
		catch(IOException e){
			System.out.println(e);
		}
	}
	//Retorna um vector com as matriculas do aluno
	public Vector<Matricula> read(){
		Vector<Matricula> matriculas = new Vector<Matricula>();
		this.in.nextLine(); //Le a primeira linha do arquivo e não guarda.
		while (this.in.hasNext()){
			Matricula mat = new Matricula();
			for(int i=0; i<13; i++){
				if (this.in.hasNext()){
					String aux = this.in.next();
					if (i==6)
						mat.setCodigo(aux);
					if (i==7)
						mat.setMateria(aux);
					if (i==11)
						mat.setTurma(aux);
				}
			}
			if (mat.getCodigo()!=null)
				matriculas.add(mat);
		}
		return matriculas;
	}
}
