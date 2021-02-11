import java.util.*;
import javax.swing.*;
import java.io.IOException;
/*Mostra pro aluno o que ele pode fazer no ajustinator*/
public class Operacoes extends javax.swing.JFrame {
  private javax.swing.JButton jButton1;
  private javax.swing.JButton jButton2;
  private javax.swing.JButton jButton3;
  private javax.swing.JButton jButton4;
  private javax.swing.JLabel jLabel1;
  private javax.swing.JPanel jPanel1;
  private javax.swing.JToggleButton jToggleButton1;

    public Operacoes(Aluno aluno, Vector<Matricula> matriculado, Vector<Materia> pode, Vector<Materia> requisitos, Vector<Matricula> anterior ){
        initComponents(aluno, matriculado, pode, requisitos, anterior);
    }

    @SuppressWarnings("unchecked")

    private void initComponents(final Aluno aluno, final Vector<Matricula> matriculado, final Vector<Materia> pode, final Vector<Materia> requisitos, final Vector<Matricula> anterior ){
        //colocar componentes aqui

        jToggleButton1 = new javax.swing.JToggleButton();
        jPanel1 = new javax.swing.JPanel();
        jLabel1 = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jButton3 = new javax.swing.JButton();
        jButton4 = new javax.swing.JButton();

        jToggleButton1.setText("jToggleButton1");

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jLabel1.setText("       ESCOLHA UMA OPERAÇÃO");

        jButton1.setText("FAZER AJUSTE");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt, aluno, matriculado, pode, requisitos);
            }
        });

        jButton2.setText("LISTAR HISTÓRICO");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt, aluno);
            }
        });

        jButton3.setText("VER GRADE HORÁRIA");
        jButton3.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton3ActionPerformed(evt, anterior);
            }
        });

        jButton4.setText("SAIR");
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt){
                jButton4ActionPerformed(evt, anterior, matriculado, aluno);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(203, 203, 203)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, 246, Short.MAX_VALUE)
                    .addComponent(jButton1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jButton2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jButton3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jButton4, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap(228, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(29, 29, 29)
                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(37, 37, 37)
                .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(38, 38, 38)
                .addComponent(jButton3, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(40, 40, 40)
                .addComponent(jButton4, javax.swing.GroupLayout.PREFERRED_SIZE, 42, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(48, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        pack();
    }
    //Ajuste de matricula
    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt, Aluno aluno, Vector<Matricula> matriculado,
                                          Vector<Materia> pode, Vector<Materia> requisitos) {
        //Chama o ajuste
        Matricula re = new Matricula();
        Vector<Matricula> ajustada = new Vector<Matricula>();
        Escrita save = new Escrita();
        pode = save.leOfertadas(pode);
        matriculado = save.leAluno(matriculado);
        ajustada = re.ajuste(aluno, matriculado, pode, requisitos);
    }
    //Exibe o histórico escolar de apenas matérias aprovadas
    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt, Aluno aluno) {
      String aux = "ALUNO : ";
      aux = aux + aluno.getNome() + " " + "GRR: " + aluno.getGrr() + "\n";
      for (int i=0; i<aluno.getMatriculas().size(); i++){
          if (aluno.getMatriculas().get(i).getStatus().equals("Aprovado"))
            aux = (aux + aluno.getMatriculas().get(i).getMateria() + " - " + aluno.getMatriculas().get(i).getCodigo() + "\n");
      }
      JOptionPane.showMessageDialog(null,"HISTÓRICO ESCOLAR\n"+aux);
    }
    //Mostra a grade horária original
    private void jButton3ActionPerformed(java.awt.event.ActionEvent evt, Vector<Matricula> anterior) {
      String grade = "";
      for (int i=0; i<anterior.size(); i++){
          grade = (grade + anterior.get(i).getMateria() + "\n" + anterior.get(i).dia1 + ": "+
                  anterior.get(i).horario_ini1 + " - " + anterior.get(i).horario_fim1 + "\n"+
                  anterior.get(i).dia2 +": "+ anterior.get(i).horario_ini2 + " - " + anterior.get(i).horario_fim2 + "\n");
    	}
        JOptionPane.showMessageDialog(null,"Grade horária original:\n"+grade);
    }
    //Antes de sair e encerrar o programa, gera o arquivo de ajuste
    private void jButton4ActionPerformed(java.awt.event.ActionEvent evt, Vector<Matricula> anterior, Vector<Matricula> matriculado, Aluno aluno){
      Escrita save = new Escrita();
      matriculado = save.leAluno(matriculado);
      Arquivo ar = new Arquivo();
      try{
        ar.escreve(anterior, matriculado, aluno);
      }catch(IOException e){
        System.out.println(e);
      }
      System.exit(0);
    }
}
