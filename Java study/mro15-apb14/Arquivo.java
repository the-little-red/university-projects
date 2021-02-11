import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;
public class Arquivo{
  public Arquivo(){

  }
  public void escreve (Vector<Matricula> anterior, Vector<Matricula> matriculado, Aluno aluno) throws IOException {
    Boolean esta = false;
    FileWriter arq = new FileWriter("ajuste.txt");
    PrintWriter gravarArq = new PrintWriter(arq);
    gravarArq.printf("Nome: %s Grr: %s\n", aluno.getNome(), aluno.getGrr());
    //Está no anterior e não no matriculado
    for (int i=0; i<anterior.size(); i++){
      for (int j=0; j<matriculado.size(); j++){
        if(anterior.get(i).getCodigo().equals(matriculado.get(j).getCodigo()) && anterior.get(i).getTurma().equals(matriculado.get(j).getTurma()))
          esta = true;
      }
      if(!esta){
        gravarArq.printf("- %s %s\n", anterior.get(i).getCodigo(), anterior.get(i).getTurma());
      }
      esta = false;
    }
    esta = false;
    //Está no matriculado e não no anterior
    for (int i=0; i<matriculado.size(); i++){
      for (int j=0; j<anterior.size(); j++){
        if(matriculado.get(i).getCodigo().equals(anterior.get(j).getCodigo()) && matriculado.get(i).getTurma().equals(anterior.get(j).getTurma()))
          esta = true;
      }
      if(!esta){
        gravarArq.printf("+ %s %s\n", matriculado.get(i).getCodigo(), matriculado.get(i).getTurma());
      }
      esta = false;
    }
    arq.close();
  }
}
