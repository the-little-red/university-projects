import java.util.*;
import java.io.*;
public class Aluno implements Serializable{
	private String nome;
	private String grr;
	private Vector<Matricula> matriculas;
	public Aluno(){
		matriculas =  new Vector<Matricula>();
	}
	public String getNome(){
		return this.nome;
	}
	public String getGrr(){
		return this.grr;
	}
	public Vector<Matricula> getMatriculas(){
		return this.matriculas;
	}
	public void setNome (String nome){
		this.nome = nome;
	}
	public void setGrr (String grr){
		this.grr = grr;
	}
	public void setMatriculas ( Vector<Matricula> matriculas ){
		this.matriculas = matriculas;
	}
	//Verifica se o GRR existe
	public boolean busca(String grr, Aluno contido){
		if (grr.equals(contido.getGrr()))
			return true;
		else
			return false;
	}
}
