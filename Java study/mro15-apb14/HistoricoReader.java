import java.util.*;
import java.io.*;
/* Lê o histórico do aluno e retorna um Aluno*/
public class HistoricoReader{
	private Scanner in;
	public HistoricoReader(String file){
		try{
			this.in = new Scanner (new FileReader(file)).useDelimiter(",|\n");
		}
		catch(IOException e){
			System.out.println(e);
		}
	}
	//Retorna o aluno lido
	public Aluno read(){
		int cont =0;
		this.in.nextLine();
		this.in.nextLine();
		Aluno ret = new Aluno();
		Vector<Matricula> auxmm = new Vector<Matricula>();
		Aluno alu = new Aluno();
    Matricula auxm = new Matricula();
		while (this.in.hasNext()){
			for (int i=0; i<27; i++){
				if(this.in.hasNext()){
					String aux =  this.in.next();
					if ( cont==0 && i==3)
						alu.setNome(aux);
					if ( cont==0 && i==1)
				 		alu.setGrr(aux);
					if (i ==13 || i==14 || i==15 || i==26){
            if (i==13){
		            auxm = new Matricula();
                auxm.setStatus(aux);
						}else{
                if (i==14){
							      auxm.setCodigo(aux);
						    }else{
							      if(i==15){
								        auxm.setMateria(aux);
						 	      }else{
								        if(i==26){
									          auxmm.add(auxm);
			                      cont++;
        					      }
							      }
						    }
					  }
				  }
			  }
		}
    }
		ret.setMatriculas(auxmm);
		ret.setNome(alu.getNome());
		ret.setGrr(alu.getGrr());
		return ret;
	}
}
