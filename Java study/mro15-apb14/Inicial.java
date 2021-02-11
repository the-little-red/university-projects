import javax.swing.JFrame;
import javax.swing.JOptionPane;
import java.util.*;
/*Interface inicial, de abertura do software */
public class Inicial extends javax.swing.JFrame {

  private javax.swing.JButton jButton1;
  private javax.swing.JButton jButton2;
  private javax.swing.JLabel jLabel1;
  private javax.swing.JPanel jPanel1;
  private javax.swing.JTextField jTextField2;
  public Inicial() {
    initComponents();
  }
  @SuppressWarnings("unchecked")
  private void initComponents() {

    jPanel1 = new javax.swing.JPanel();
    jButton1 = new javax.swing.JButton();
    jTextField2 = new javax.swing.JTextField();
    jLabel1 = new javax.swing.JLabel();
    jButton2 = new javax.swing.JButton();

    setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

    jButton1.setText("Login");
    jButton1.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(java.awt.event.ActionEvent evt) {
        jButton1ActionPerformed(evt);
      }
    });

    jTextField2.setText("GRR");
    jTextField2.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(java.awt.event.ActionEvent evt) {
        jTextField2ActionPerformed(evt);
      }
    });

    jLabel1.setText("          AJUSTINATOR");

    jButton2.setText("FECHAR");
    jButton2.addActionListener(new java.awt.event.ActionListener() {
      public void actionPerformed(java.awt.event.ActionEvent evt) {
        jButton2ActionPerformed(evt);
      }
    });

    javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
    jPanel1.setLayout(jPanel1Layout);
    jPanel1Layout.setHorizontalGroup(
    jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
    .addGroup(jPanel1Layout.createSequentialGroup()
    .addContainerGap(226, Short.MAX_VALUE)
    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
    .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, 175, Short.MAX_VALUE)
    .addComponent(jTextField2))
    .addContainerGap(226, Short.MAX_VALUE))
    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
    .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
    .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
    .addComponent(jButton1, javax.swing.GroupLayout.DEFAULT_SIZE, 81, Short.MAX_VALUE)
    .addComponent(jButton2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
    .addGap(270, 270, 270))
    );
    jPanel1Layout.setVerticalGroup(
    jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
    .addContainerGap()
    .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 54, javax.swing.GroupLayout.PREFERRED_SIZE)
    .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 101, Short.MAX_VALUE)
    .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
    .addGap(76, 76, 76)
    .addComponent(jButton1)
    .addGap(48, 48, 48)
    .addComponent(jButton2)
    .addGap(47, 47, 47))
    );

    javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
    getContentPane().setLayout(layout);
    layout.setHorizontalGroup(
    layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
    .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
    );
    layout.setVerticalGroup(
    layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
    .addComponent(jPanel1, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
    );

    pack();
  }
  //Botão de login
  private void jButton1ActionPerformed(java.awt.event.ActionEvent evt) {
    String grr = jTextField2.getText();
    HistoricoReader historico = new HistoricoReader("historico.csv");
    Aluno aluno = new Aluno();
    aluno = historico.read();
    if (aluno.busca(grr, aluno)){
      JOptionPane.showMessageDialog(null, "GRR encontrado");
        //Se aluno foi encontrado ja manda ler a matricula dele
        Matricula aux = new Matricula();
        Vector<Matricula> matriculado = new Vector<Matricula>();
        Vector<Materia> pode = new Vector<Materia>();
        Vector<Materia> requisitos = new Vector<Materia>();
        Vector<Matricula> original = new Vector<Matricula>();
        requisitos = requisitos();
        matriculado = matricula();
        pode = oferta();
        matriculado = aux.incluiHorario(pode, matriculado);
        original = matriculado;
        Escrita save = new Escrita();
        save.escreveAluno(matriculado);
        save.escreveOfertas(pode);
        //Chama as operações que o aluno pode fazer no ajustinator
        new Operacoes(aluno, matriculado, pode, requisitos, original).setVisible(true); //Chama a tela de opções
        this.dispose();
    }
    else{
      //GRR não foi encontrado
      JOptionPane.showMessageDialog(null, "GRR não encontrado");
      jTextField2.setText("GRR");
    }

  }
  //TextField GRR
  private void jTextField2ActionPerformed(java.awt.event.ActionEvent evt) {
  }
  //Botão de sair
  private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {
    System.exit(0);
  }
  //Lê e retorna um Vector com a matrícula do aluno
  private Vector<Matricula> matricula(){
    MatriculasReader matricula = new MatriculasReader("matricula.csv");
    Vector<Matricula> matriculado = new Vector<Matricula>();
    matriculado = matricula.read();
    return matriculado;
  }
  //Lê e retorna um Vector com as disciplinas ofertadas
  private Vector<Materia> oferta(){
    DiscOfertadasReader dis = new DiscOfertadasReader("oferta_disciplinas-2016-1.csv");
    Vector<Materia> pode = new Vector<Materia>();
    pode = dis.read();
    return pode;
  }
  //Lê e retorna um Vector com as matérias e seus respectivos pré-requisitos
  private Vector<Materia> requisitos(){
    PreRequisitosReader pre = new PreRequisitosReader("pre-requisitos.csv");
    Vector<Materia> requisitos = new Vector<Materia>();
    requisitos = pre.read();
    return requisitos;
  }
}
