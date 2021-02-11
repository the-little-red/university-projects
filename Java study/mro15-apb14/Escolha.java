import java.util.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
/* Classe de escolha entre adicionar ou remover turmas*/
public class Escolha extends javax.swing.JFrame {

    public Escolha(Aluno aluno, Vector<String> oferta, Vector<Materia> ofertadas, Vector<Matricula> matriculado, Vector<String> lmat, Vector<Materia> requisitos) {
        initComponents(aluno, oferta, ofertadas, matriculado, lmat, requisitos);
    }

    @SuppressWarnings("unchecked")
    private void initComponents(final Aluno aluno,final Vector<String> oferta, final Vector<Materia> ofertadas, final Vector<Matricula> matriculado, final Vector<String> lmat, final Vector<Materia> requisitos) {

        jPanel1 = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        jList1 = new javax.swing.JList<String>(oferta);
        jLabel1 = new javax.swing.JLabel();
        jTextField1 = new javax.swing.JTextField();
        jTextField2 = new javax.swing.JTextField();
        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jScrollPane2 = new javax.swing.JScrollPane();
        jList2 = new javax.swing.JList(lmat);
        jLabel4 = new javax.swing.JLabel();
        jButton1 = new javax.swing.JButton();
        jButton2 = new javax.swing.JButton();
        jButton4 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        atualizajList(ofertadas);
        atualizajList1(matriculado);
        jScrollPane1.setViewportView(jList1);
        jScrollPane2.setViewportView(jList2);

        jLabel1.setText("MATÉRIAS OFERTADAS");

        jTextField1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField1ActionPerformed(evt);
            }
        });

        jTextField2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jTextField2ActionPerformed(evt);
            }
        });

        jList1.addMouseListener(new MouseAdapter() {
             public void mouseClicked(MouseEvent evt) {
                    showDetails(ofertadas, matriculado, evt);
            }
        });

        jList2.addMouseListener(new MouseAdapter() {
             public void mouseClicked(MouseEvent evt) {
                    showDetails1(ofertadas, matriculado, evt);
            }
        });

        jLabel2.setText("TURMA");

        jLabel3.setText("CÓDIGO");

        jLabel4.setText("MATÉRIAS MATRICULADO");

        jButton1.setText("DESMATRICULAR");
        jButton1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton1ActionPerformed(evt, ofertadas, matriculado);
            }
        });

        jButton2.setText("MATRICULAR");
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt, ofertadas, matriculado, requisitos, aluno);
            }
        });

        jButton4.setText("VER");
        jButton4.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton4ActionPerformed(evt, matriculado);
            }
        });

        jButton5.setText("SAIR");
        jButton5.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton5ActionPerformed(evt, ofertadas, matriculado);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 265, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(106, 106, 106))
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 264, Short.MAX_VALUE)
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(jPanel1Layout.createSequentialGroup()
                                        .addGap(28, 28, 28)
                                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, 56, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addComponent(jLabel2)
                                            .addComponent(jButton4, javax.swing.GroupLayout.PREFERRED_SIZE, 61, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                    .addGroup(jPanel1Layout.createSequentialGroup()
                                        .addGap(18, 18, 18)
                                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                            .addGroup(jPanel1Layout.createSequentialGroup()
                                                .addComponent(jButton5, javax.swing.GroupLayout.PREFERRED_SIZE, 82, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED))
                                            .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                                .addGroup(jPanel1Layout.createSequentialGroup()
                                                    .addComponent(jLabel3)
                                                    .addGap(25, 25, 25))
                                                .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                                                    .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, 71, javax.swing.GroupLayout.PREFERRED_SIZE)
                                                    .addGap(18, 18, 18)))))))))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(40, 40, 40)
                        .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 187, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel4, javax.swing.GroupLayout.PREFERRED_SIZE, 224, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, 270, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(48, 48, 48)
                        .addComponent(jButton1, javax.swing.GroupLayout.PREFERRED_SIZE, 183, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(37, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, 38, Short.MAX_VALUE)
                    .addComponent(jLabel4, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 332, Short.MAX_VALUE)
                            .addComponent(jScrollPane1))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel1Layout.createSequentialGroup()
                                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(jButton1)
                                    .addComponent(jButton5))
                                .addGap(6, 6, 6))
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                                .addComponent(jButton2)
                                .addContainerGap())))
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addGap(21, 21, 21)
                        .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 27, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jTextField1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 27, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jTextField2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(53, 53, 53)
                        .addComponent(jButton4)
                        .addContainerGap())))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );

        pack();
    }

    //Código da matéria
    private void jTextField1ActionPerformed(java.awt.event.ActionEvent evt) {

    }
    //Turma
    private void jTextField2ActionPerformed(java.awt.event.ActionEvent evt) {
    }
    //Adiconar nova matéria
    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt, Vector<Materia> ofertadas, Vector<Matricula> matriculado, Vector<Materia> requisitos, Aluno aluno) {
      String codigo = jTextField1.getText();
      String turma = jTextField2.getText();
      Materia mat = new Materia();
      Matricula matr = new Matricula();
      int esc;
      if (mat.jaCursou(aluno, codigo)){
        mat.buscaRequisitos(requisitos, codigo, aluno);
        esc = mat.buscaConflitos(matriculado, ofertadas, codigo, turma);
        if (esc ==0){
          //pergunta se quer realmente se matricular
          Object[] options = { "Sim", "Não" };
          //int dialogButton = JOptionPane.YES_NO_OPTION;
          int dialogResult = JOptionPane.showOptionDialog(null, "Quer fazer o ajuste mesmo assim?","Aviso", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[0]);
          if(dialogResult == JOptionPane.YES_OPTION){
            matriculado = matr.adicionaMatricula(ofertadas, matriculado, codigo, turma);
            atualizajList(ofertadas);
            atualizajList1(matriculado);
            jTextField1.setText("");
            jTextField2.setText("");
          }else{
            jTextField1.setText("");
            jTextField2.setText("");
          }
        }else{
          if (esc == 1){
            //Não há conflito faz ajuste direto
            matriculado = matr.adicionaMatricula(ofertadas, matriculado, codigo, turma);
            atualizajList(ofertadas);
            atualizajList1(matriculado);
            jTextField1.setText("");
            jTextField2.setText("");
          }else{
            jTextField1.setText("");
            jTextField2.setText("");
          }
        }
      }else{
        jTextField1.setText("");
        jTextField2.setText("");
      }
    }
    //Remover matéria
    private void jButton1ActionPerformed(java.awt.event.ActionEvent evt, Vector<Materia> ofertadas, Vector<Matricula> matriculado) {
      String codigo = jTextField1.getText();
      String turma = jTextField2.getText();
      Matricula mat = new Matricula();
      ofertadas = mat.adicionaOferta(ofertadas, matriculado, codigo, turma);
      atualizajList(ofertadas);
      atualizajList1(matriculado);
      jTextField1.setText("");
      jTextField2.setText("");
    }
    //Detalhes de matérias ofertadas
    private void showDetails(Vector<Materia> oferta, Vector<Matricula> matriculado, MouseEvent evt){
                JList list = (JList)evt.getSource();
                if (evt.getClickCount() == 2) {
                    int index = list.locationToIndex(evt.getPoint());
                    JOptionPane.showMessageDialog(null,oferta.get(index).nome+" - "+oferta.get(index).codigo+" - "+oferta.get(index).turma+"\n"
                                                +oferta.get(index).dia1+": "+oferta.get(index).horario_ini1+" - "+oferta.get(index).horario_fim1+"\n"
                                                +oferta.get(index).dia2+": "+oferta.get(index).horario_ini2+" - "+oferta.get(index).horario_fim2+"\n");
                }
    }
    //Detalhes de matéria matriculado/ajustado
    private void showDetails1(Vector<Materia> oferta, Vector<Matricula> matriculado, MouseEvent evt){
                JList list = (JList)evt.getSource();
                if (evt.getClickCount() == 2) {
                    int index = list.locationToIndex(evt.getPoint());
                    JOptionPane.showMessageDialog(null,matriculado.get(index).getMateria()+" - "+matriculado.get(index).getCodigo()+" - "+matriculado.get(index).getTurma()+"\n"
                                                +matriculado.get(index).dia1+": "+matriculado.get(index).horario_ini1+" - "+matriculado.get(index).horario_fim1+"\n"
                                                +matriculado.get(index).dia2+": "+matriculado.get(index).horario_ini2+" - "+matriculado.get(index).horario_fim2+"\n");
                }
    }
    //Atualiza a jList de matérias ofertadas
    private void atualizajList(Vector<Materia> ofertadas){
      Vector<String> att = new Vector<String>();
      DefaultListModel listModel=new DefaultListModel();
      for(int i=0; i< ofertadas.size(); i++){
          att.add(ofertadas.get(i).nome);
          if (att.get(i)!= ""){
          }
      }
      jList1.setListData(att);
    }
    //Atualiza jList de matéria matriculado/ajustado
    private void atualizajList1(Vector<Matricula> matriculado){
      Vector<String> att = new Vector<String>();
      DefaultListModel listModel=new DefaultListModel();
      for(int i=0; i< matriculado.size(); i++){
          att.add(matriculado.get(i).getMateria());
          if (att.get(i)!= ""){
          }
      }
      jList2.setListData(att);
    }
    //Ao sair, grava o estado do vector de ofertadas e de matriculado
    private void jButton5ActionPerformed(java.awt.event.ActionEvent evt, Vector<Materia> ofertadas, Vector<Matricula> matriculado){
      Escrita save = new Escrita();
      save.escreveOfertas(ofertadas);
      save.escreveAluno(matriculado);
      this.dispose();
    }
    //Mostra a grade horária modificada
    private void jButton4ActionPerformed(java.awt.event.ActionEvent evt, Vector<Matricula> matriculado) {
      String grade = "";
      for (int i=0; i<matriculado.size(); i++){
          grade = (grade + matriculado.get(i).getMateria() + "\n" + matriculado.get(i).dia1 + ": "+
                  matriculado.get(i).horario_ini1 + " - " + matriculado.get(i).horario_fim1 + "\n"+
                  matriculado.get(i).dia2 +": "+ matriculado.get(i).horario_ini2 + " - " + matriculado.get(i).horario_fim2 + "\n");
    	}
        JOptionPane.showMessageDialog(null,"Grade horária ajustada:\n"+grade);
    }

    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JList<String> jList1;
    private javax.swing.JList<String> jList2;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTextField jTextField1;
    private javax.swing.JTextField jTextField2;
}
