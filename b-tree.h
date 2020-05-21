/*  
    Hugo                      ????????
    Daniel De Marco Fucci     11218639
*/
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#define TAM 15
#define ordem 8
#define data_size 128
#include <stdio.h>

typedef struct tipoAluno {
	int nusp;
	char nome[TAM];
	char sobrenome[TAM];
	char curso[TAM];
	float nota;
} tipoAluno;

typedef struct pagina {
	int contador;
	char chaves[ordem-1]; //assumindo chaves char
	int filhos[ordem]; //armazena o RRN dos filhos
	int folha;
} PAGINA;

void btree();
void criaArquivo(FILE *, char *, char *);
void geradorAlunos(FILE *);

#endif