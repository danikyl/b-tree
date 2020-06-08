/*  
	Daniel De Marco Fucci
    Hugo Fellipe Lage Alvarães da Cruz
*/
#include "b-tree.h"
#include <stdlib.h>

int main (int argc, char *argv[]) {
	int teste2;

	int comando= 8;
	int nUspToSearch;
	int students_quantity;

	FILE *df = fopen("dados.txt", "ab+");
	fclose(df);

	FILE *index_stream = fopen("index.dat", "ab+");
	fclose(index_stream);


	B_TREE_HEADER *header = loadBTreeHeader();

	do {
		comando = 8;
		printf("Operacoes:\n- 1: Gravar\n- 2: Pesquisar por nUSP\n- 3: Mostrar todos\n- 4: Gerar massa de dados\n- 0: Sair\n");
		scanf ("%d", &comando);
		switch (comando) {
			case 1:
				readToWriteRecord(header);
				break;
			case 2:
				printf("Digite o numero USP para buscar: \n");
				scanf ("%d", &nUspToSearch);
				searchRecordByNUSP(header, nUspToSearch);
				break;
			case 3:
				showAll();
				break;
			case 4:
				printf("Digite a quantidade de alunos para inserir na base: \n");
				scanf ("%d", &students_quantity);
				generateStudents(students_quantity, header);
				break;
				
		}





	} while(comando> 0);
	//exitAndSave(header);
	free(header);


return 0;

}