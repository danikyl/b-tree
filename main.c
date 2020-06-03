/*  
    Hugo Fellipe Lage Alvarães da Cruz  10903872
    Daniel De Marco Fucci               11218639
*/
#include "b-tree.h"
#include <stdlib.h>

int tree_size;

int main (int argc, char *argv[]) {
	int comando= 06;
	int nUspToSearch;

	FILE *df = fopen("dados.txt", "ab+");
	fclose(df);

	FILE *primIndex_stream = fopen("index_prim.dat", "ab+");
	fclose(primIndex_stream);

	FILE *secIndex_stream = fopen("index_sec.dat", "ab+");
	fclose(secIndex_stream);

	FILE *invertedList_stream = fopen("inverted_list.dat", "ab+");
	fclose(secIndex_stream);

    FILE *bTree_stream = fopen("btree.dat", "ab+");
    fclose(bTree_stream);



	INDEX_RECORD_LIST *listPrim = (INDEX_RECORD_LIST *) calloc(1, sizeof(INDEX_RECORD_LIST));

	INDEX_SEC_RECORD_LIST *listSec = (INDEX_SEC_RECORD_LIST *) calloc(1, sizeof(INDEX_SEC_RECORD_LIST));

    PAGE *bTree = (PAGE *) calloc(1,sizeof(PAGE));

	tree_size = 0;

	loadIndexPrim(listPrim);

	loadIndexSec(listSec);

    loadBTree(bTree);

	do {
		comando = 6;
		printf("Operacoes:\n- 1: Gravar\n- 2: Pesquisar por nUSP\n- 3: Pesquisar por sobrenome\n- 4: Mostrar todos\n- 5: Eliminar registro por nUSP\n- 6: Eliminar registro por sobrenome\n- 0: Sair\n");
		scanf ("%d", &comando);
		switch (comando) {
			case 1:
				generateStudents();
				//addToBTree(bTree);
				//addToBTree(bTree);
				//readToWriteRecord(listPrim, listSec);
				break;
			case 2:
				printf("Digite o numero USP para buscar: \n");
				scanf ("%d", &nUspToSearch);
				searchRecordByNUSP(listPrim, nUspToSearch);
				break;
			case 3:
				searchRecordByLastName(listSec, listPrim);
				break;
			case 4:
				showAll();
				break;
			case 5:
				printf("Digite o numero USP para excluir: \n");
				scanf ("%d", &nUspToSearch);
				deleteRecord(listPrim, listSec, nUspToSearch);
				break;
			case 6:
				deleteAllRecordsByLastName(listPrim, listSec);
				break;
				
		}





	} while(comando> 0);
	exitAndSave(listPrim, listSec);


return 0;

}
