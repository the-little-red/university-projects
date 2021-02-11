import java.util.*;
import java.io.*;
/*Grava o estado dos vetores de matérias ofertadas
  e que o aluno quer se matricular para possível
	ajuste posterior*/
@SuppressWarnings("unchecked")

public class Escrita{

	public void escreveAluno(Vector<Matricula> matriculado){
		try{
			//Gera o arquivo para armazenar o objeto
			FileOutputStream arquivoGrav =  new FileOutputStream("aluno");
			//Classe responsavel por inserir os objetos
			ObjectOutputStream objGravar = new ObjectOutputStream(arquivoGrav);
			//Grava o objeto no arquivo
			objGravar.writeObject(matriculado);
			objGravar.flush();
			objGravar.close();
			arquivoGrav.flush();
			arquivoGrav.close();
		}
		catch( Exception e ){
			System.out.println("Informações do Aluno não puderam ser salvas");
			e.printStackTrace( );
		}
	}
	public Vector<Matricula> leAluno(Vector<Matricula> matriculado){
		try{
			//Carrega o arquivo
			FileInputStream arquivoLeitura = new FileInputStream("aluno");
			//Classe responsavel por recuperar os objetos do arquivo
			ObjectInputStream objLeitura = new ObjectInputStream(arquivoLeitura);
			matriculado = (Vector<Matricula>) objLeitura.readObject();
			objLeitura.close();
			arquivoLeitura.close();
		}
		catch( Exception e ){
			System.out.println("Erro na leitura!");
			e.printStackTrace( );
		}
		return matriculado;
	}
	public void escreveOfertas(Vector<Materia> ofertada){
	 	try{
			//Gera o arquivo para armazenar o objeto
	 		FileOutputStream arquivoGrav =  new FileOutputStream("ofertas");
	 		//Classe responsavel por inserir os objetos
	 		ObjectOutputStream objGravar = new ObjectOutputStream(arquivoGrav);
	 		//Grava o objeto no arquivo
			objGravar.writeObject(ofertada);
	 		objGravar.flush();
	 		objGravar.close();
			arquivoGrav.flush();
	 		arquivoGrav.close();
 		}
		catch( Exception e ){
	 		System.out.println("Informações não puderam ser salvas");
	 		e.printStackTrace( );
	 	}
 	}
	public Vector<Materia> leOfertadas(Vector<Materia> ofertada){
		try{
			//Carrega o arquivo
			FileInputStream arquivoLeitura = new FileInputStream("ofertas");
 			//Classe responsavel por recuperar os objetos do arquivo
			ObjectInputStream objLeitura = new ObjectInputStream(arquivoLeitura);
			ofertada = (Vector<Materia>) objLeitura.readObject();
			objLeitura.close();
			arquivoLeitura.close();
 		}
		catch( Exception e ){
			System.out.println("Erro na leitura!");
	 		e.printStackTrace( );
		}
	 	return ofertada;
 	}
}
