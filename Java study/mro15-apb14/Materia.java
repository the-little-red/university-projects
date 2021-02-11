import java.util.*;
import java.io.*;
import java.io.Serializable;
import javax.swing.*;
/*Classe que realiza as operações com as matérias*/
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
	//Retorna um vector de Strings com as matérias ofertadas
	public Vector<String> transformaString(Vector<Materia>pode){
		Vector<String> materias = new Vector<String>();
		for(int i=0; i< pode.size(); i++){
				materias.add(pode.get(i).nome);
		}
		return materias;
	}
	//Verifica se o aluno já cursou a matéria que esta tentando se matricular
	public Boolean jaCursou(Aluno aluno, String codigo){
		for(int i=0; i< aluno.getMatriculas().size(); i++){
				if(aluno.getMatriculas().get(i).getCodigo().equals(codigo)){
					if (aluno.getMatriculas().get(i).getStatus().equals("Aprovado")){
						JOptionPane.showMessageDialog(null, "Você já cursou esta matéria.");
						return false;
					}
				}
		}
		return true;
	}
	//Verifica se há pré-requisitos para a matéria que o aluno está tentando se matricular
	//Se tiver mostra os mesmos, senão prossegue com o ajuste
	public void buscaRequisitos(Vector<Materia> requisitos, String codigo, Aluno aluno){
		for(int i=0; i< requisitos.size(); i++){
				if(requisitos.get(i).codigo.equals(codigo)){
					Vector<String> req = new Vector<String>();
					for(int j=0; j< requisitos.get(i).preReq.size(); j++){
						req.add(requisitos.get(i).preReq.get(j).codigo);
					}
					display(req);
				}
		}
	}
	//Verifica conflitos de horário e dia
	public int buscaConflitos(Vector<Matricula> matriculado, Vector<Materia> ofertadas, String codigo, String turma){
		String dia1 = "", dia2 = "", horario_ini1 = "", horario_ini2 = "", horario_fim1 = "", horario_fim2 = "";
		Boolean encontrado = false, conf = false;
		int j = 0;
		for(int i=0; i< ofertadas.size(); i++){
				if (ofertadas.get(i).codigo.equals(codigo) && ofertadas.get(i).turma.equals(turma)){
					dia1 = ofertadas.get(i).dia1;
					dia2 = ofertadas.get(i).dia2;
					horario_ini1 = ofertadas.get(i).horario_ini1;
					horario_fim1 = ofertadas.get(i).horario_fim1;
					horario_ini2 = ofertadas.get(i).horario_ini2;
					horario_fim2 = ofertadas.get(i).horario_fim2;
					encontrado = true;
					i = ofertadas.size();
				}
		}
		if (encontrado){
			for (int i=0; i< matriculado.size(); i++){
				if(conflitoDia(dia1, dia2, matriculado.get(i)) && conflitoHorario(horario_ini1, horario_fim1, horario_ini2, horario_fim2, matriculado.get(i))){
					conf = true;
					j = i;
				}
			}
			if (conf){
				JOptionPane.showMessageDialog(null, "Está matéria tem conflito de horário com:"+"\n"+
				 matriculado.get(j).getCodigo() +" "+ matriculado.get(j).getMateria() +"\n"+ matriculado.get(j).dia1 +
				 " : " + matriculado.get(j).horario_ini1 + "-" + matriculado.get(j).horario_fim1 + "\n" + matriculado.get(j).dia2 +
				 " : " + matriculado.get(j).horario_ini2 + "-" + matriculado.get(j).horario_fim2);
				 return 0; //Há conflito
			}
			return 1; //Não há conflito
		}
		else{
			JOptionPane.showMessageDialog(null, "Código ou turma não existem, ou matéria não está em oferta!");
			return 2; //Matéria solicitada errada
		}
	}
	//Mostra um JOptionPane com os pré requisitos da matéria solicitada
	private void display(Vector<String>requisitos) {
		String req = "";
		for (int i = 0; i<requisitos.size(); i++){
			req = (req+requisitos.get(i)+"\n");
		}
		JOptionPane.showMessageDialog(null,"Esta matéria tem os seguintes pré-requisitos\n"+req);
	}
	//Verifica conflito de dia
	private Boolean conflitoDia(String dia1, String dia2, Matricula matriculado){
		if (dia1.equals(matriculado.dia1) || dia2.equals(matriculado.dia2))
			return true;
		else
			return false;
	}
	//Verifica conflito de horário
	private Boolean conflitoHorario(String horario_ini1, String horario_fim1,  String horario_ini2, String horario_fim2, Matricula matriculado){
		if ((horario_ini1.equals(matriculado.horario_ini1) && horario_fim1.equals(matriculado.horario_fim1)) ||
				(horario_ini2.equals(matriculado.horario_ini1) && horario_fim2.equals(matriculado.horario_fim1)) ||
				(horario_ini2.equals(matriculado.horario_ini2) && horario_fim1.equals(matriculado.horario_fim2)))
			return true;
		else
			return false;
	}
}
