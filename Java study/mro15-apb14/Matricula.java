import java.util.*;
import java.io.*;
import javax.swing.*;
/*Classe que realiza as operações com a matrícula*/
public class Matricula extends Materia implements Serializable{
	private String materia;
	private String codigo;
	private String turma;
  private String status;
	public void setMateria(String materia){
		this.materia = materia;
	}
	public void setStatus(String status){
		this.status = status;
	}
	public void setCodigo(String codigo){
		this.codigo = codigo;
	}
	public void setTurma(String turma){
		this.turma = turma;
	}
	public String getMateria(){
		return this.materia;
	}
	public String getCodigo(){
		return this.codigo;
	}
	public String getTurma(){
		return this.turma;
	}
	public String getStatus(){
		return this.status;
	}
	//Começa o ajuste
  public Vector<Matricula> ajuste(Aluno aluno, Vector<Matricula> matriculado, Vector<Materia> pode, Vector<Materia> requisitos){
		Vector<Matricula> escolhidas = new Vector<Matricula>();
		Vector<String> materias = new Vector<String>();
		Vector<String> matrString = new Vector<String>();
		Materia aux = new Materia();
		materias = aux.transformaString(pode);
		// Recupera vector de ofertadas e matriculado
		Escrita save = new Escrita();
		matriculado = save.leAluno(matriculado);
	  pode = save.leOfertadas(pode);
		matrString = transformaString1(matriculado);
		//Chama a tela de escolha
		Escolha tela = new Escolha(aluno, materias, pode, matriculado, matrString, requisitos);
		tela.setVisible(true);
		return escolhidas;
  }
	//Inclui o horario e dia no vector de Matricula
	public Vector<Matricula> incluiHorario(Vector<Materia> ofertada, Vector<Matricula> matriculado){
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
				if(verifica.codigo.equals(verifica2.codigo) && verifica.turma.equals(verifica2.turma) ){
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
	//Retorna um vector de string com o nome das matérias matriculado
	public Vector<String> transformaString1(Vector<Matricula>matriculado){
		Vector<String> materias = new Vector<String>();
		for(int i=0; i< matriculado.size(); i++){
			materias.add(matriculado.get(i).getMateria());
		}
		return materias;
	}
	//Adicona uma nova Matricula ao vector de matriculado e remove do ofertadas
	public Vector<Matricula> adicionaMatricula(Vector<Materia> ofertada, Vector<Matricula> matriculado, String codigo, String turma1){
		Vector<Matricula> novamatriculas = new Vector<Matricula>();
		novamatriculas = matriculado;
		Iterator itr = ofertada.iterator();
		Materia verifica = new Materia();
		Materia valores = new Materia();
	  while(itr.hasNext()){
	    verifica =(Materia) itr.next();
	    if(codigo.equals(verifica.codigo) && turma1.equals(verifica.turma)){
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
			return novamatriculas;
	}
	//Adiociona uma nova oferta ao vetor de ofertadas e remove do vetor de matriculado
	public Vector<Materia> adicionaOferta(Vector<Materia> ofertada, Vector<Matricula> matriculado, String codigo, String turma1){
		Vector<Materia> novaoferta = new Vector<Materia>();
		novaoferta = ofertada;
		Iterator itr = matriculado.iterator();
		Matricula verifica = new Matricula();
		Boolean matricula = buscaCodigoMatricula(matriculado,codigo,turma1);
		if(!matricula){
			JOptionPane.showMessageDialog(null, "Você não estava matriculado nessa materia!");
		}else{
			while(itr.hasNext()){
					verifica =(Matricula) itr.next();
					if(codigo.equals(verifica.getCodigo()) && turma1.equals(verifica.getTurma())){
						Materia nova = new Materia();
						nova.turma = verifica.getTurma();
						nova.codigo = verifica.getCodigo();
						nova.nome = verifica.getMateria();
						nova.horario_ini1 = verifica.horario_ini1;
						nova.horario_fim1 = verifica.horario_fim1;
						nova.dia1 = verifica.dia1;
						nova.dia2 = verifica.dia2;
						nova.horario_ini2 = verifica.horario_ini2;
						nova.horario_fim2 = verifica.horario_fim2;
						novaoferta.add(nova);
						itr.remove();
					}
				}
		}
		return novaoferta;
	}
	//Verifica se o código e a turma existem no vector de matricula, para então exclui-los
	public Boolean buscaCodigoMatricula(Vector<Matricula> matriculado, String codigo, String turma1){
		for (int i=0; i< matriculado.size(); i++){
			if (matriculado.get(i).codigo.equals(codigo) && matriculado.get(i).turma.equals(turma1))
				return true;
		}
		return false;
	}
}
