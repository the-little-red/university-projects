import java.util.*;
import java.io.*;
public class Matricula implements Serializable{

	private String materia;
	private String codigo;
	private String turma;
  private String status;

	public String dia1;
	public String dia2;
	public String horario_ini1;
	public String horario_fim1;
	public String horario_ini2;
	public String horario_fim2;

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

    public Vector<Matricula> ajuste(Aluno aluno, Vector<Matricula> matriculado){
        Vector<Matricula> escolhidas = new Vector<Matricula>();
        Vector<Materia> pode = new Vector<Materia>();
        Vector<String> materias = new Vector<String>();
        System.out.println("Ajuste");
        System.out.println(aluno.getNome());
        System.out.println("MateriasCursadas");
        for (int i=0; i< aluno.getMatriculas().size(); i++){
            System.out.println(aluno.getMatriculas().get(i).getMateria());
        }
        System.out.println("MateriasOfertadas");
        DiscOfertadasReader dis = new DiscOfertadasReader("oferta_disciplinas-2016-1.csv");
        pode= dis.read();
        for(int i=0; i< pode.size(); i++){
            materias.add(pode.get(i).nome);
        }
        Escolha tela = new Escolha(materias, pode, matriculado);
        tela.setVisible(true);
        return escolhidas;
    }
}
