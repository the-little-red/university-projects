JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Aluno.java \
	DiscOfertadasReader.java \
	Escolha.java \
	Escrita.java \
	HistoricoReader.java \
	Materia.java \
	Matricula.java \
	MatriculasReader.java \
	Operacoes.java \
	PreRequisitosReader.java \
	Arquivo.java \
	Inicial.java \
	Main.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
