/*  
	Daniel De Marco Fucci
    Hugo Fellipe Lage Alvarães da Cruz 
*/
#include "b-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define TAM 16


B_TREE_HEADER* loadBTreeHeader() {
	FILE *index_stream= fopen("index.dat", "r+");
	B_TREE_HEADER *header = (B_TREE_HEADER *) malloc(sizeof(B_TREE_HEADER));

	//Checking if file is empty
	fseek(index_stream, 0, SEEK_END);
	if (ftell(index_stream) == 0) {
		header->nodesNumber = 0;
		header->order = B_TREE_ORDER;
		header->rootNodeRRN = -1;
		fwrite(header, sizeof(B_TREE_HEADER), 1, index_stream);
	}

	else {
		fseek(index_stream, 0, SEEK_SET);
		fread(header, sizeof(B_TREE_HEADER), 1, index_stream);
	}
	
<<<<<<< HEAD
=======
}

int add_to_inverted_list(INDEX_SEC_RECORD* record, int nUsp) {
	INVERTED_LIST_RECORD *inverted_list_record = (INVERTED_LIST_RECORD *) malloc(sizeof(INVERTED_LIST_RECORD));
	INVERTED_LIST_RECORD *inverted_list_record_iterator = (INVERTED_LIST_RECORD *) calloc(1, sizeof(INVERTED_LIST_RECORD));
	INVERTED_LIST_RECORD *inverted_list_record_iterator2 = (INVERTED_LIST_RECORD *) calloc(1, sizeof(INVERTED_LIST_RECORD));
	FILE *inverted_list_file_stream= fopen("inverted_list.dat", "r+");
	int positionBackingTrack=-1;
	int currentPosition=-1;

	inverted_list_record->nUsp=nUsp;
	inverted_list_record->nxtValuePosition = -1;

	//Case lastName is already on List
	if (record->headPosition != -1) {
		fseek(inverted_list_file_stream, record->headPosition, SEEK_SET);
		fread(inverted_list_record_iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
		currentPosition=record->headPosition;
		//Case when the first element is the only one on chained list for provided lastname
		if (inverted_list_record_iterator->nxtValuePosition == -1) {
			//Unique list element NUSP is bigger than the new record about to be written on file
			if (inverted_list_record_iterator->nUsp > nUsp) {
				inverted_list_record->nxtValuePosition=record->headPosition;
				fseek(inverted_list_file_stream, 0, SEEK_END);
				record->headPosition=ftell(inverted_list_file_stream);
				fwrite(inverted_list_record, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
			}
			//Case unique list element Nusp is lower than NUSP of new record about to be written on file
			else {
				fseek(inverted_list_file_stream, 0, SEEK_END);
				inverted_list_record_iterator->nxtValuePosition = ftell(inverted_list_file_stream);
				fwrite(inverted_list_record, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);

				fseek(inverted_list_file_stream, record->headPosition, SEEK_SET);
				fwrite(inverted_list_record_iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
				
			}
		}
		//Case the chained list has more values for provided lastname
		else {
			while (inverted_list_record_iterator->nxtValuePosition != -1 && inverted_list_record_iterator->nUsp < nUsp) {
				positionBackingTrack=currentPosition;
				currentPosition=inverted_list_record_iterator->nxtValuePosition;


				inverted_list_record_iterator2->nUsp = inverted_list_record_iterator->nUsp;
				inverted_list_record_iterator2->nxtValuePosition = inverted_list_record_iterator->nxtValuePosition;
				fseek(inverted_list_file_stream, currentPosition, SEEK_SET);
				fread(inverted_list_record_iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
			}
			if (inverted_list_record_iterator->nUsp < nUsp) {
				fseek(inverted_list_file_stream, 0, SEEK_END);
				inverted_list_record_iterator->nxtValuePosition=ftell(inverted_list_file_stream);
				fwrite(inverted_list_record, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);

				fseek(inverted_list_file_stream, currentPosition, SEEK_SET);
				fwrite(inverted_list_record_iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
			}
			//In this case the new record should be added between two records
			else {
				fseek(inverted_list_file_stream, 0, SEEK_END);
				inverted_list_record_iterator2->nxtValuePosition=ftell(inverted_list_file_stream);

				inverted_list_record->nxtValuePosition=currentPosition;
				fwrite(inverted_list_record, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);

				fseek(inverted_list_file_stream, positionBackingTrack, SEEK_SET);
				fwrite(inverted_list_record_iterator2, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
			}
		}
		
		
	}
	//Case lastname is not on the chained list yet
	else {
		fseek(inverted_list_file_stream, 0, SEEK_END);
		fwrite(inverted_list_record, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
		printf("Added to inverted_list: %d %d\n", inverted_list_record->nUsp, inverted_list_record->nxtValuePosition);
		record->headPosition = ftell(inverted_list_file_stream) - sizeof(INVERTED_LIST_RECORD);
	}
	


	fclose(inverted_list_file_stream);
	free(inverted_list_record);
	free(inverted_list_record_iterator);
	free(inverted_list_record_iterator2);

	return record->headPosition;
	
}

void loadBTree(PAGE *pageList) {
  printf("entrou na btree \n");
  FILE *index_stream= fopen("btree.dat", "r+");
  fseek(index_stream, 0, 0);
  PAGE *page = (PAGE *) malloc(sizeof(PAGE));
  
  while (fread(index, sizeof(INDEX_RECORD), 1, index_stream)) {
    pageList = (PAGE *) realloc(pageList, sizeof(PAGE)*(tree_size+1));
    int j;
    for(j=0;j<order-1;j++)
    {
      pageList[tree_size].keys[j].nusp = page->keys[j].nusp;
      pageList[tree_size].keys[j].rrn = page->keys[j].rrn;
    }
    for(j=0;j<order;j++)
    {
      pageList[tree_size].sons[j] = page->sons[j];
    }

    pageList[tree_size].leaf = page->leaf;
	tree_size++;
  }
  
  //free(index);
  fclose(index_stream);
}

void loadIndexPrim(INDEX_RECORD_LIST *list) {
	FILE *index_stream= fopen("index_prim.dat", "r+");
	fseek(index_stream, 0, 0);
	INDEX_RECORD *index = (INDEX_RECORD *) malloc(sizeof(INDEX_RECORD));

	while (fread(index, sizeof(INDEX_RECORD), 1, index_stream)) {
		add_index_element(list, index);
	}
	
	free(index);
	fclose(index_stream);
}

void loadIndexSec(INDEX_SEC_RECORD_LIST *list) {
	FILE *index_stream= fopen("index_sec.dat", "r+");
	fseek(index_stream, 0, 0);
	INDEX_SEC_RECORD *index = (INDEX_SEC_RECORD *) malloc(sizeof(INDEX_SEC_RECORD));

	while (fread(index, sizeof(INDEX_SEC_RECORD), 1, index_stream)) {
		add_index_sec_element(list, index, 0, -1);
	}
	
	free(index);
	fclose(index_stream);
}
void exitAndSave(INDEX_RECORD_LIST *listPrim, INDEX_SEC_RECORD_LIST *listSec) {
	FILE *index_stream= fopen("index_prim.dat", "w+");
	FILE *index_sec_stream= fopen("index_sec.dat", "w+");

	INDEX_LIST_ELEMENT **iterator = listPrim->start;
	INDEX_SEC_RECORD **iterator2 = listSec->start;
	int i;
	//Writing primary index file and releasing memory
	for (i=0; i<listPrim->size; i++) {
		if(iterator[i]->record) {
			fwrite(iterator[i]->record, sizeof(INDEX_RECORD), 1, index_stream);
			free(iterator[i]->record);
			free(iterator[i]);
		}
		
	}
	//Writing secondary index file and releasing memory
	for (i=0; i<listSec->size; i++) {
		fwrite(iterator2[i], sizeof(INDEX_SEC_RECORD), 1, index_sec_stream);
		free(iterator2[i]);
	}


	free(listPrim->start);
	free(listPrim);

	free(listSec->start);
	free(listSec);

	fclose(index_stream);
	fclose(index_sec_stream);

}

void print_index_file() {
	FILE *index_stream= fopen("index_prim.dat", "r+");
	INDEX_RECORD *iterator = (INDEX_RECORD *) malloc(sizeof(INDEX_RECORD));

	printf("PRINTING INDEX FILE: \n");
	while (fread(iterator, sizeof(INDEX_RECORD), 1, index_stream)) {
		printf("FILE: USP N: %d, POSTIION: %d\n", iterator->nUsp, iterator->position);
	}	
	
>>>>>>> 6923a4d3b21f90457d13113a4411d866a51cc216
	fclose(index_stream);
	return header;
}

void showAll() {
	FILE *data_file_stream= fopen("dados.txt", "r+");
	fseek(data_file_stream, 0, 0);
	typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));

	while (fread(student, sizeof(typeStudent), 1, data_file_stream)) {
		if (!student->isDeleted) {
			printf ("%d %s %s %s %.1f\n", student->numUSP, student->name, student->lastName, student->course, student->score);
		}	
	}
	
	free(student);
	fclose(data_file_stream);
}
int findUspNumber(int uspNumberToSearch, B_TREE_HEADER *header) {
	int i;
	int nxtChildrenPos = header->rootNodeRRN;
	int foundPosition = ERR_NOT_FOUND;
	FILE *index_file_stream= fopen("index.dat", "r+");
	B_TREE_NODE *node_iter = (B_TREE_NODE *) malloc(sizeof(B_TREE_NODE));


	while (nxtChildrenPos != -1) {
		fseek(index_file_stream, nxtChildrenPos, SEEK_SET);
		fread(node_iter, sizeof(B_TREE_NODE), 1, index_file_stream);
		for (i=0; i<header->order-1; i++) {
			if (node_iter->keyVector[i] == uspNumberToSearch) {
				fclose(index_file_stream);
				foundPosition = node_iter->dataRrnVector[i];
				free (node_iter);
				return foundPosition;
			} 
			else if ((node_iter->keyVector[i] > uspNumberToSearch) || (node_iter->keyVector[i] == -1)) {
				nxtChildrenPos = node_iter->childrenVector[i];
				break;
			}
		}
		if (i >= header->order-1) {
			nxtChildrenPos = node_iter->childrenVector[header->order-1];
		}
	}
	
	fclose(index_file_stream);
	free (node_iter);
    return ERR_NOT_FOUND; 
}

void searchRecordByNUSP(B_TREE_HEADER *header, int nUspToSearch) {
	int recordPositionOnFile;
	recordPositionOnFile=findUspNumber(nUspToSearch, header);
	if (recordPositionOnFile == ERR_NOT_FOUND) {
		printf("Aluno nao cadastrado! nUSP: %d\n", nUspToSearch);
		
	}
	else {
		FILE *data_file_stream= fopen("dados.txt", "r+");
		typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));

		fseek(data_file_stream, recordPositionOnFile, SEEK_SET);
		fread(student, sizeof(typeStudent), 1, data_file_stream);

		printf ("%d %s %s %s %.1f\n", student->numUSP, student->name, student->lastName, student->course, student->score);

		free(student);
		fclose(data_file_stream);
	}
}

int alreadyInUse (int nUsp, B_TREE_HEADER *header) {
	int inUse = findUspNumber(nUsp, header) == ERR_NOT_FOUND ? 0 : 1;
	return inUse;
} 
void readToWriteRecord(B_TREE_HEADER *header) {
	typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));

	printf("Digite o numero USP: \n");
	scanf ("%d", &student->numUSP);
	printf("Digite o nome: \n");
	scanf (" %[^\n]s", student->name);
	printf("Digite o sobrenome: \n");
	scanf (" %[^\n]s", student->lastName);
	printf("Digite o curso: \n");
	scanf (" %[^\n]s", student->course);
	printf("Digite a nota: \n");
	scanf ("%f", &student->score);
	printf ("%d %s %s %s %.1f\n", student->numUSP, student->name, student->lastName, student->course, student->score);

	student->isDeleted=0;

	//Checking if nUsp is already in use
	int isInUse= alreadyInUse(student->numUSP, header);
	if (isInUse) printf("Erro! Aluno já cadastrado.\n");
	else writeRecord(header, student);

	free(student);
}
void writeRecord(B_TREE_HEADER *header, typeStudent *student) {

	int position_data;
	int i;
	int half;
	FILE *data_file_stream= fopen("dados.txt", "a+");
	FILE *index_file_stream= fopen("index.dat", "r+");
	//Writing to data file
	fwrite(student, sizeof(typeStudent), 1, data_file_stream);
	position_data = ftell(data_file_stream) - sizeof(typeStudent);
	


	//Going through B-Tree
	//**************************B-Tree is empty!!!!**************************
	if(header->rootNodeRRN == -1)
	{
		B_TREE_NODE *root_node = createNewNode(header);
		root_node->nodeSize++;
		root_node->dataRrnVector[0]=position_data;
		root_node->keyVector[0]=student->numUSP;

		//Writing root node to index file
		fseek(index_file_stream, sizeof(B_TREE_HEADER), SEEK_SET);
		header->rootNodeRRN = ftell(index_file_stream);
		fwrite(root_node, sizeof(B_TREE_NODE), 1, index_file_stream);

		//Writing header to index file
		fseek(index_file_stream, 0, SEEK_SET);
		fwrite(header, sizeof(B_TREE_HEADER), 1, index_file_stream);

		
		free(root_node);
	}
	//**************************ROOT IS A LEAF!!!**************************
	else if(header->nodesNumber == 1)
	{	
		//Reading root node from file
		B_TREE_NODE *root_node = (B_TREE_NODE *) malloc(sizeof(B_TREE_NODE));
		fseek(index_file_stream, header->rootNodeRRN, SEEK_SET);
		fread(root_node, sizeof(B_TREE_NODE), 1, index_file_stream);

		//**************************There is no overflow yet**************************
		if (root_node->nodeSize < header->order-1) {
			root_node->nodeSize++;
			i=root_node->nodeSize-2;
			while(i >= 0 && (root_node->keyVector[i]) > (student->numUSP)) {
				root_node->keyVector[i + 1] = root_node->keyVector[i];
				i--;
			}
			root_node->keyVector[i+1]=student->numUSP;
			root_node->dataRrnVector[i+1]=position_data;
			
			
			

<<<<<<< HEAD
			//Updating root node on index file
			fseek(index_file_stream, header->rootNodeRRN, SEEK_SET);
			fwrite(root_node, sizeof(B_TREE_NODE), 1, index_file_stream);


		}
		//**************************OVER FLOW IN ROOT NODE**************************
		else {
			B_TREE_NODE *higher_child_node = createNewNode(header);
			B_TREE_NODE *lower_child_node = createNewNode(header);
			long int keyVectorAux[B_TREE_ORDER];
			long int dataRrnVectorAux[B_TREE_ORDER];

			//Using aux vectors to be able to neadly include the new element before spliting vectors
			int k=0;
			int newElementInserted = 0;
			for (i=0; i<B_TREE_ORDER-1; i++) {
				if (root_node->keyVector[i] > student->numUSP) {
					keyVectorAux[k] = student->numUSP;
					dataRrnVectorAux[k] = position_data;
					k++;
					keyVectorAux[k] = root_node->keyVector[i];
					dataRrnVectorAux[k] = root_node->dataRrnVector[i];
					newElementInserted = 1;
				}
				else {
					keyVectorAux[k] = root_node->keyVector[i];
					dataRrnVectorAux[k] = root_node->dataRrnVector[i];
				}
				//Cleaning up the root node, so it will keep as root
				root_node->keyVector[i]=-1;
				root_node->dataRrnVector[i]=-1;
				k++;
			}
			if (!newElementInserted) {
				keyVectorAux[B_TREE_ORDER-1] = student->numUSP;
				dataRrnVectorAux[B_TREE_ORDER-1] = position_data;
			}

			//root node will have only one element now, the first one of the second half
			root_node->nodeSize=1;

			//Spliting in two new nodes
			half = B_TREE_ORDER/2;
			for (i=0; i<half; i++) {
				lower_child_node->nodeSize++;
				lower_child_node->keyVector[i] = keyVectorAux[i];
				lower_child_node->dataRrnVector[i] = dataRrnVectorAux[i];
			}
			root_node->keyVector[0] = keyVectorAux[half];
			root_node->dataRrnVector[0] = dataRrnVectorAux[half];
			k=0;
			for (i=half+1; i<B_TREE_ORDER; i++) {
				higher_child_node->nodeSize++;
				higher_child_node->keyVector[k] = keyVectorAux[i];
				higher_child_node->dataRrnVector[k] = dataRrnVectorAux[i];
				k++;
=======
void generateStudents()
{
	char firstNames[50][20] = {"Rogelio","Ernie","Debera","Yvonne","Guillermo","Annalisa","Muriel","Julianne","Octavia","Noelia","Providencia","Liliana","Lorriane","Hee","Arica","Brooke","Mandy","Lesli","Reinaldo","Reginald","Ricarda","Cassondra","Tonita","Janean","Nannette","Annelle","Mitzie","Gayla","Alexis","Laureen","Santa","Quinton","Vinnie","Vikki","Maryam","Gus","Saturnina","Marissa","Duncan","Russel","Eugenie","Sherise","Tammara","Maile","Dodie","Moses","Brianna","Enda","Tiera","Terisa"};
	char lastNames[50][10] = {"Robertson","Santana","Harmon","Collier","Vance","Cabrera","Murphy","Tanner","Valencia","Perez","Pope","Powell","Casey","Fisher","Floyd","Conway","Avila","Gibson","Harrell","Prince","Patel","Glenn","Knapp","Gross","Bush","Estrada","Shannon","Huang","Saunders","Coleman","Mata","Parker","Austin","Richmond","Gentry","Harrison","Sparks","Fritz","Barrett","Norton","Lucas","Salas","Carroll","Shaw","Wade","Howe","Lynch","Krause","Ayala","Silva"};
	int i;
	for(i=0;i<3;i++)
	{
		int indexFirstName = rand() % 50;
		int indexLastName = rand() % 50;
		int nUSP = rand() % 100000;
		typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));
		student->numUSP = nUSP;
		strcpy(student->name, firstNames[indexFirstName]);
		strcpy(student->lastName, lastNames[indexLastName]);
		strcpy(student->course, "BSI");
		student->score = 8.0;
		student->isDeleted = 0;
		//writeRecord(listPrim, listSec, student, index_record, index_sec_record);
		printf("Student inserted: %d\n",nUSP);
		printf("Name: %s %s\n",firstNames[indexFirstName],lastNames[indexLastName]);
		free(student);
	}
}

void addToBTree(PAGE *btree) {
  
	typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));
	int nUSP = rand() % 100000;
	student->numUSP = nUSP;
	strcpy(student->name, "Hugo");
	strcpy(student->lastName, "Cruz");
	strcpy(student->course, "BSI");
	student->score = 8.0;
	student->isDeleted = 0;
	int rrn = writeRecord(student);
	printf("Student inserted: %d\n",nUSP);
	printf("RRN: %d\n",rrn);
	
  //caso 1: árvore vazia
  if(btree[0].keys[0].nusp == 0)
  {
  	printf("caso 1 satisfeito\n");
  	printf("nusp %d\n",btree[0].keys[0].nusp);
  	btree[0].keys[0].nusp = student->numUSP;
  	btree[0].keys[0].rrn = rrn;
  	int j;
    for(j=0;j<order;j++)
    {
      btree[0].sons[j] = -1;
    }
    btree[0].leaf = 1;
  }else{
	//caso 2 só tem raiz, sem overflow
	//checar se todos os ponteiros são -1
	if(checkSons(btree[0].sons)==0 &&  btree[0].keys[order-2].nusp == 0)
	{
		printf("caso 2 satisfeito\n");
		int i;
		insertOrdered(btree[0].keys,student->numUSP,rrn);
		
		
		//caso 3, inserção no nó raiz com overflow
		if(checkSons(btree[0].sons)==0 &&  btree[0].keys[order-2].nusp != 0)
		{
			searchKey pivot;
			pivot = (searchKey){.nusp = btree[0].keys[(order-1)/2].nusp, .rrn =btree[0].keys[(order-1)/2].rrn};
			btree = (PAGE *) realloc(btree, sizeof(PAGE)*(3));
			setLeafSons(&btree[1]);
			setLeafSons(&btree[2]);


			for(int i=0;i<order-1;i++)
			{
				if(btree[0].keys[i].nusp<pivot.nusp)
				{
					insertOrdered(btree[1].keys,student->numUSP,rrn);
				}
				if(btree[0].keys[i].nusp>pivot.nusp)
				{
					insertOrdered(btree[2].keys,student->numUSP,rrn);
				}
				if(btree[0].keys[i].nusp>pivot.nusp)
				{
					btree[0].keys[0].nusp = btree[0].keys[i].nusp;
					btree[0].keys[0].rrn = btree[0].keys[i].nusp;
				}
				btree[0].keys[i].nusp = 0;
				btree[0].keys[i].rrn = 0;
				btree[0].sons[1] = 1;
				btree[0].sons[1] = 2;
				btree[0].leaf = 0;
>>>>>>> 6923a4d3b21f90457d13113a4411d866a51cc216
			}

			//Writing new nodes to file and getting rrn
			fseek(index_file_stream, 0, SEEK_END);
			root_node->childrenVector[0] = ftell(index_file_stream);
			fwrite(lower_child_node, sizeof(B_TREE_NODE), 1, index_file_stream);
			root_node->childrenVector[1] = ftell(index_file_stream);
			fwrite(higher_child_node, sizeof(B_TREE_NODE), 1, index_file_stream);




			//Updating root_node on file
			fseek(index_file_stream, header->rootNodeRRN, SEEK_SET);
			fwrite(root_node, sizeof(B_TREE_NODE), 1, index_file_stream);


			free(higher_child_node);
			free(lower_child_node);
		}

<<<<<<< HEAD
		free(root_node);


	}

	

	//**************************WE HAVE MORE LEAFS!!!**************************
	else if(header->nodesNumber != 1) {
		//Need to find the correct leaf to insert

		
		int nxtChildrenPos = header->rootNodeRRN;
		FILE *index_file_stream= fopen("index.dat", "r+");
		B_TREE_NODE *node_iter = (B_TREE_NODE *) malloc(sizeof(B_TREE_NODE));

		//Going through nodes
		while (nxtChildrenPos != -1) {
			fseek(index_file_stream, nxtChildrenPos, SEEK_SET);
			fread(node_iter, sizeof(B_TREE_NODE), 1, index_file_stream);
			

			//*******************************************FOUND THE CORRECT LEAF!!*********************************************
			if (node_iter->childrenVector[0] == -1) {

				//**************************There is no overflow yet**************************
				if (node_iter->nodeSize < header->order-1) {
					node_iter->nodeSize++;
					i=node_iter->nodeSize-2;
					while(i >= 0 && (node_iter->keyVector[i]) > (student->numUSP)) {
						node_iter->keyVector[i + 1] = node_iter->keyVector[i];
						i--;
					}
					node_iter->keyVector[i+1]=student->numUSP;
					node_iter->dataRrnVector[i+1]=position_data;
					//Updating leaf node on index file
					fseek(index_file_stream, nxtChildrenPos, SEEK_SET);
					fwrite(node_iter, sizeof(B_TREE_NODE), 1, index_file_stream);
				}
				//**************************Leaf overflow!**************************
				else {
					B_TREE_NODE *higher_child_node = createNewNode(header);
					B_TREE_NODE *lower_child_node = createNewNode(header);
					long int keyVectorAux[B_TREE_ORDER];
					long int dataRrnVectorAux[B_TREE_ORDER];

					//Using aux vectors to be able to neadly include the new element before spliting vectors
					int k=0;
					int newElementInserted = 0;
					for (i=0; i<B_TREE_ORDER-1; i++) {
						if (node_iter->keyVector[i] > student->numUSP) {
							keyVectorAux[k] = student->numUSP;
							dataRrnVectorAux[k] = position_data;
							k++;
							keyVectorAux[k] = node_iter->keyVector[i];
							dataRrnVectorAux[k] = node_iter->dataRrnVector[i];
							newElementInserted = 1;
						}
						else {
							keyVectorAux[k] = node_iter->keyVector[i];
							dataRrnVectorAux[k] = node_iter->dataRrnVector[i];
						}
						//Cleaning up the father node
						node_iter->keyVector[i]=-1;
						node_iter->dataRrnVector[i]=-1;
						k++;
					}
					if (!newElementInserted) {
						keyVectorAux[B_TREE_ORDER-1] = student->numUSP;
						dataRrnVectorAux[B_TREE_ORDER-1] = position_data;
					}

					//father node will have only one element now, the first one of the second half
					node_iter->nodeSize=1;

					//Spliting in two new nodes
					half = B_TREE_ORDER/2;
					for (i=0; i<half; i++) {
						lower_child_node->nodeSize++;
						lower_child_node->keyVector[i] = keyVectorAux[i];
						lower_child_node->dataRrnVector[i] = dataRrnVectorAux[i];
					}
					node_iter->keyVector[0] = keyVectorAux[half];
					node_iter->dataRrnVector[0] = dataRrnVectorAux[half];
					k=0;
					for (i=half+1; i<B_TREE_ORDER; i++) {
						higher_child_node->nodeSize++;
						higher_child_node->keyVector[k] = keyVectorAux[i];
						higher_child_node->dataRrnVector[k] = dataRrnVectorAux[i];
						k++;
					}

					//Writing new nodes to file and getting rrn
					fseek(index_file_stream, 0, SEEK_END);
					node_iter->childrenVector[0] = ftell(index_file_stream);
					fwrite(lower_child_node, sizeof(B_TREE_NODE), 1, index_file_stream);
					node_iter->childrenVector[1] = ftell(index_file_stream);
					fwrite(higher_child_node, sizeof(B_TREE_NODE), 1, index_file_stream);




					//Updating node_iter on file
					fseek(index_file_stream, nxtChildrenPos, SEEK_SET);
					fwrite(node_iter, sizeof(B_TREE_NODE), 1, index_file_stream);


					free(higher_child_node);
					free(lower_child_node);
				}
				nxtChildrenPos=-1;

			} 
			else {
				//*******************************************IT'S NOT A LEAF!!*********************************************
				for (i=0; i<header->order-1; i++) {
					if ((node_iter->keyVector[i] > student->numUSP) || (node_iter->keyVector[i] == -1)) {
						nxtChildrenPos = node_iter->childrenVector[i];
						break;
					}
				}
				if (i >= header->order-1) {
					nxtChildrenPos = node_iter->childrenVector[header->order-1];
				}
			}


			
		}

		fclose(index_file_stream);
		free(node_iter);
=======
		for(int a=0;a<3;a++)
		{
			printf("\npagina: %d\n",a);
			for(int j=0;j<order-1;j++)
			{
				printf("\n");
				printf("nusp: %d\n",btree[a].keys[j].nusp);
				printf("rrn: %d\n",btree[a].keys[j].rrn);
			}
		}
		
>>>>>>> 6923a4d3b21f90457d13113a4411d866a51cc216
	}

	
	fclose(index_file_stream);
	fclose(data_file_stream);
	

}



void printNodeSize() {
	printf("Node Size: %ld\n", sizeof(B_TREE_NODE));
}

B_TREE_NODE * createNewNode(B_TREE_HEADER *header) {
	int i;
	//Allocating memory for the node
	B_TREE_NODE *new_node = (B_TREE_NODE *) malloc(sizeof(B_TREE_NODE));
	header->nodesNumber++;
	new_node->nodeSize = 0;
	
	//Setting null values (-1) to all keys in brand new node
	for (i=0; i < (header->order-1); i++) {
		new_node->keyVector[i] = (long int) -1;
		new_node->dataRrnVector[i] = (long int) -1;
		new_node->childrenVector[i] = (long int) -1;
	}
	new_node->childrenVector[header->order-1] = (long int) -1;

	return new_node;
}

void freeNode(B_TREE_NODE *node) {
	free(node);
}
void generateStudents(int student_quantity, B_TREE_HEADER *header)
{
	char firstNames[50][20] = {"Rogelio","Ernie","Debera","Yvonne","Guillermo","Annalisa","Muriel","Julianne","Octavia","Noelia","Providencia","Liliana","Lorriane","Hee","Arica","Brooke","Mandy","Lesli","Reinaldo","Reginald","Ricarda","Cassondra","Tonita","Janean","Nannette","Annelle","Mitzie","Gayla","Alexis","Laureen","Santa","Quinton","Vinnie","Vikki","Maryam","Gus","Saturnina","Marissa","Duncan","Russel","Eugenie","Sherise","Tammara","Maile","Dodie","Moses","Brianna","Enda","Tiera","Terisa"};
	char lastNames[50][10] = {"Robertson","Santana","Harmon","Collier","Vance","Cabrera","Murphy","Tanner","Valencia","Perez","Pope","Powell","Casey","Fisher","Floyd","Conway","Avila","Gibson","Harrell","Prince","Patel","Glenn","Knapp","Gross","Bush","Estrada","Shannon","Huang","Saunders","Coleman","Mata","Parker","Austin","Richmond","Gentry","Harrison","Sparks","Fritz","Barrett","Norton","Lucas","Salas","Carroll","Shaw","Wade","Howe","Lynch","Krause","Ayala","Silva"};
	int i;
	for(i=1;i<=student_quantity;i++)
	{
		int indexFirstName = rand() % 50;
		int indexLastName = rand() % 50;
		int nUSP = i;
		typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));
		student->numUSP = nUSP;
		strcpy(student->name, firstNames[indexFirstName]);
		strcpy(student->lastName, lastNames[indexLastName]);
		strcpy(student->course, "BSI");
		student->score = 8.0;
		student->isDeleted = 0;


		//Checking if nUsp is already in use
		int isInUse= alreadyInUse(student->numUSP, header);
		if (isInUse) printf("Erro! Aluno já cadastrado.\n");
		else {
			writeRecord(header, student);
			printf("Student inserted: %d\n",nUSP);
			printf("Name: %s %s\n",firstNames[indexFirstName],lastNames[indexLastName]);
		}
		free(student);
	}
<<<<<<< HEAD
}
=======
	return isEmpty;
}

void swap(int* x, int* y) 
{ 
    int z = *x; 
    *x = *y; 
    *y = z; 
} 

void insertOrdered(searchKey *keys,int nusp,int rrn)
{
	int i;
	for(i=0;i<order-1;i++)
	{
		if(keys[i].nusp == 0)
		{
			keys[i].nusp = nusp;
			keys[i].rrn = rrn;
			break;
		}
		if(keys[i].nusp > nusp)
		{
			swap(&nusp,&keys[i].nusp);
			swap(&rrn,&keys[i].rrn);
		}
	}
}

void setLeafSons(PAGE *page)
{
	for(int i=0;i<order;i++)
	{
		page->sons[i] = -1;
	}
	page->leaf = 1;
}
>>>>>>> 6923a4d3b21f90457d13113a4411d866a51cc216
