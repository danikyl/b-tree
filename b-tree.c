/*  
    Hugo Fellipe Lage Alvarães da Cruz  10903872
    Daniel De Marco Fucci               11218639
*/
#include "b-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


void btree() {
	FILE *df;
	do {
	  df= fopen("alunos.dat", "r+");
	  if (df== NULL) criaArquivo(df, "alunos.dat","w+");
	} while(df== NULL);

	printf("created file\n");
	geradorAlunos(df);
}



void criaArquivo(FILE *df, char *name, char *permissions) {
    //Função geral para criação de arquivos, serve para qualquer índice
    if (df== NULL) {
        df= fopen(name, permissions);        /* Se nao existir, cria o arquivo de dados */
        fclose(df);
        df= NULL;
    }
    
}

void insereRegistro(FILE *df,tipoAluno aluno) {
  /* Neste método devem sem implementadas as operações de inserção registro no arquivo, lembrando que o índice deve se manter consistente com o arquivo */
  int rrn;
  if(df != NULL)
  {
    df= fopen("alunos.dat", "a+");
    fseek(df, 1, SEEK_END);
    rrn = ftell(df)/sizeof(tipoAluno);
    printf("ftell registro: %ld\n",ftell(df));
    fwrite (&aluno, sizeof(tipoAluno), 1, df);
    fclose(df);
  }
  //return rrn;
}

void geradorAlunos(FILE *df)
{
	int i;
	for(i=0;i<data_size;i++)
	{
		int nUSP = rand() % 100000;
		tipoAluno aluno = {nUSP,"Hugo","Cruz","BSI",8.0};
		insereRegistro(df,aluno);
		printf("Inseriu aluno: %d\n",nUSP);
	}
}