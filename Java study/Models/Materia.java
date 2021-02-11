import java.util.*;
import java.io.*;
import java.io.Serializable;
public class Materia implements Serializable{
	public String codigo;
  	public String nome;
	public String horario_ini1;
	public String horario_fim1;
  	public String horario_ini2;
  	public String horario_fim2;
	public Vector<Materia> preReq;
	public String dia1;
	public String dia2;
	public String turma;

	public Materia(){
		preReq = new Vector<Materia>();
	}

public Boolean buscaCodigoOferta(Vector<Materia> ofertada, String codigo, String turma1){
		Iterator itr = ofertada.iterator();
		Materia verifica = new Materia();
		Boolean excodigo = false;
		while(itr.hasNext()){
			verifica =(Materia) itr.next();
			if(codigo.equals(verifica.codigo) && turma1.equals(verifica.turma))
				excodigo = true;
		}
		return excodigo;
	}
	public Boolean buscaCodigoMatricula(Vector<Matricula> matriculado, String codigo, String turma1){
		Iterator itr = matriculado.iterator();
		Matricula verifica = new Matricula();
		Boolean exmatricula = false;
		while(itr.hasNext()){
			verifica =(Matricula) itr.next();
			if(codigo.equals(verifica.getCodigo()) && turma1.equals(verifica.getTurma()) ){
			exmatricula = true;
			}
		}
	 	return exmatricula;
	}
	public Vector<Matricula> horarioDia(Vector<Materia> ofertada, Vector<Matricula> matriculado){
		Vector<Matricula> matriculahora = new Vector<Matricula>();
		Iterator itr = ofertada.iterator();
		Iterator itr2 = matriculado.iterator();
		Materia verifica = new Materia(); //recebe materias para comparar
		Matricula verifica2 = new Matricula(); //recebe matriculas para comparar


		while(itr2.hasNext()){
			verifica2 = (Matricula) itr2.next();
			itr = ofertada.iterator();
			while(itr.hasNext()){
				verifica = (Materia) itr.next();
				if(verifica.codigo.equals(verifica2.getCodigo()) && verifica.turma.equals(verifica2.getTurma()) ){
					Matricula nova = new Matricula();
					nova.setCodigo(verifica2.getCodigo());
					nova.setMateria(verifica2.getMateria()); //adiciona codigo da matriculaverificaiona as infos que n tem na matricula
					nova.setTurma(verifica.turma);
					nova.horario_ini1 = verifica.horario_ini1;
					nova.horario_ini2 = verifica.horario_ini2;
					nova.horario_fim1 = verifica.horario_fim1;
					nova.horario_fim2 = verifica.horario_fim2;
					nova.dia1 = verifica.dia1;
					nova.dia2 = verifica.dia2;
					matriculahora.add(nova); //adiciona as matriculas com horario e infos novas em um vetor de matriculas
					itr.remove(); //remove a materia do ofertadas ja que o aluno esta matriculado nela
				}
			}
		}
		return matriculahora; //retorna as matriculas com as infos completas
	}

	public Vector<Matricula> adicionaMatricula(Vector<Materia> ofertada, Vector<Matricula> matriculado, String codigo, String turma1){
		Vector<Matricula> novamatriculas = new Vector<Matricula>();
		novamatriculas = matriculado;
		Iterator itr = ofertada.iterator();
		Materia verifica = new Materia();
		Materia valores = new Materia();
    	Boolean existe = valores.buscaCodigoOferta(ofertada,codigo, turma1);
		Boolean matricula = valores.buscaCodigoMatricula(matriculado,codigo,turma1);

		if(!existe){
	  		System.out.println("Codigo de materia ou turma não existem!");
	  	}else if(matricula){
			System.out.println("Aluno ja esta matriculado na materia e turma escolhidos!");
		}else if(existe && !matricula){
	    	while(itr.hasNext()){
	        	verifica =(Materia) itr.next();
	        	if(codigo.equals(verifica.codigo) && turma1.equals(verifica.turma)){
	        		System.out.println("Matriculou");
					Matricula nova = new Matricula();
					nova.setTurma(verifica.turma);
					nova.setCodigo(verifica.codigo);
					nova.setMateria(verifica.nome);
					nova.horario_ini1 = verifica.horario_ini1;
					nova.horario_ini2 = verifica.horario_ini2;
					nova.horario_fim1 = verifica.horario_fim1;
					nova.horario_fim2 = verifica.horario_fim2;
					nova.dia1 = verifica.dia1;
					nova.dia2 = verifica.dia2;
					novamatriculas.add(nova); //adiciona matricula nova
	          		itr.remove(); //remove a materia dos ofertadas
	        	}
	    	}
	  	}
		return novamatriculas;
	}
	public Vector<Materia> adicionaOferta(Vector<Materia> ofertada, Vector<Matricula> matriculado, String codigo, String turma1){
		Vector<Materia> novaoferta = new Vector<Materia>();
		novaoferta = ofertada;
		Iterator itr = matriculado.iterator();
		Matricula verifica = new Matricula();
		Materia valores = new Materia();
		Boolean existe = valores.buscaCodigoMatricula(matriculado,codigo,turma1);

		if(!existe){
			System.out.println("Aluno nao esta matriculado nessa materia ou turma!");
		}else if(existe){
			while(itr.hasNext()){
		    	verifica =(Matricula) itr.next();
		    	if(codigo.equals(verifica.getCodigo()) && turma1.equals(verifica.getTurma())){
		    		System.out.println("Desmatriculou");
					Materia nova = new Materia();
					nova.turma = verifica.getTurma();
					nova.codigo = verifica.getCodigo();
					nova.nome = verifica.getMateria();
					nova.horario_ini1 = verifica.horario_ini1;
					nova.horario_ini2 = verifica.horario_ini2;
					nova.horario_fim1 = verifica.horario_fim1;
					nova.horario_fim2 = verifica.horario_fim2;
					nova.dia1 = verifica.dia1;
					nova.dia2 = verifica.dia2;
					novaoferta.add(nova); //adiciona ofertada
		    		itr.remove(); //remove a materia dos matriculados
		  		}
		  	}
		}
				return novaoferta;
	}
		
}
