/*  
    Hugo Fellipe Lage Alvarães da Cruz  10903872
    Daniel De Marco Fucci               11218639
*/
#include "b-tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define TAM 16


void add_index_element(INDEX_RECORD_LIST *list, INDEX_RECORD* record) {
	int i;
	INDEX_LIST_ELEMENT *newElement;
	INDEX_LIST_ELEMENT **iterator;

	list->size++;
	list->start = (INDEX_LIST_ELEMENT **) realloc(list->start,sizeof(INDEX_LIST_ELEMENT *) * (list->size));
	
	newElement = (INDEX_LIST_ELEMENT *) malloc(sizeof(INDEX_LIST_ELEMENT));
	newElement->record = (INDEX_RECORD*) malloc(sizeof(INDEX_RECORD));
	newElement->record->nUsp=record->nUsp;
	newElement->record->position=record->position;

	iterator = list->start;
	i=list->size-2;
	while(i >= 0 && (iterator[i]->record->nUsp) > (newElement->record->nUsp)) {
		iterator[i + 1] = iterator[i];
		i--;
	}
	iterator[i+1]=newElement;
	
}

void print_sec_index_list(INDEX_SEC_RECORD_LIST *list) {
	INDEX_SEC_RECORD **start = list->start;
	FILE *inverted_list_file_stream= fopen("inverted_list.dat", "r+");
	INVERTED_LIST_RECORD *iterator = (INVERTED_LIST_RECORD *) malloc(sizeof(INVERTED_LIST_RECORD));

	printf("PRINTING SECUNDARY INDEX LIST IN MEMORY: \n");
	printf("LIST_SIZE: %d\n", list->size);
	
	for (int i=0; i<list->size; i++) {
		if (start[i]->headPosition != -1) {
			printf("----------SEC_LIST: : headPosition: %d, lastName: %s\n", start[i]->headPosition, start[i]->lastName);
			//Printing inverted list elements for this lastName
			fseek(inverted_list_file_stream, start[i]->headPosition, SEEK_SET);
			fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
			printf("Inv_List_elem: NUsp: %d, next_position: %d\n", iterator->nUsp, iterator->nxtValuePosition);
			while (iterator->nxtValuePosition != -1) {
				fseek(inverted_list_file_stream, iterator->nxtValuePosition, SEEK_SET);
				fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
				printf("Inv_List_elem: NUsp: %d, next_position: %d\n", iterator->nUsp, iterator->nxtValuePosition);
			}
		}
	}
	fclose(inverted_list_file_stream);
	free(iterator);
}

void add_index_sec_element(INDEX_SEC_RECORD_LIST *list, INDEX_SEC_RECORD* record, int addToInvertedList, int nUsp) {
	int i;
	INDEX_SEC_RECORD *newElement;
	INDEX_SEC_RECORD **iterator;

	//Checks if the last name is already on the secunday index list
	iterator = list->start;
	i=list->size-1;
	while(i >= 0 && (strcmp(iterator[i]->lastName, record->lastName) > 0)) {
		i--;
	}
	if (i>=0 && strcmp(iterator[i]->lastName, record->lastName) == 0) {
		if (addToInvertedList) {
			iterator[i]->headPosition = add_to_inverted_list(iterator[i], nUsp);
		}
	}
	else {
		list->size++;
		list->start = (INDEX_SEC_RECORD **) realloc(list->start,sizeof(INDEX_SEC_RECORD *) * (list->size));
		
		newElement = (INDEX_SEC_RECORD *) malloc(sizeof(INDEX_SEC_RECORD));
		strcpy(newElement->lastName, record->lastName);
		newElement->headPosition=record->headPosition;

		iterator = list->start;
		i=list->size-2;
		while(i >= 0 && (strcmp(iterator[i]->lastName, newElement->lastName) > 0)) {
			iterator[i + 1] = iterator[i];
			i--;
		}
		iterator[i+1]=newElement;
		if(addToInvertedList) newElement->headPosition = add_to_inverted_list(newElement, nUsp);
	}
	
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
  printf("size of btree: %ld\n",sizeof(pageList));
  int i=0;
  while (fread(index, sizeof(INDEX_RECORD), 1, index_stream)) {
    //add_index_element(list, index);
    //realloc
    //list->start = (INDEX_LIST_ELEMENT **) realloc(list->start,sizeof(INDEX_LIST_ELEMENT *) * (list->size));
    //ptr = realloc(ptr, n2 * sizeof(int));
    /*typedef struct {
      //int contador;
      int keys[order-1]; //assumindo chaves char
      int sons[order]; //armazena o RRN dos filhos
      int leaf;//1 = leaf, 0 = not leaf
    } PAGE; 
    */
    pageList = (PAGE *) realloc(pageList, sizeof(PAGE)*(i+1));
    int j;
    for(j=0;j<order-1;j++)
    {
      pageList[i].keys[j] = page->keys[j];
    }
    for(j=0;j<order;j++)
    {
      pageList[i].sons[j] = page->sons[j];
    }

    pageList[i].leaf = page->leaf;
    i++;
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
	
	fclose(index_stream);
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
int findUspNumber_binary_search(int uspNumberToSearch, INDEX_RECORD_LIST *list) {
	int range=list->size-1;
	int begin=0;
	while (begin <= range) { 
        int m = begin + (range - begin) / 2; 
  
        if (list->start[m]->record->nUsp == uspNumberToSearch) 
            return list->start[m]->record->position; 
  
        if (list->start[m]->record->nUsp < uspNumberToSearch) 
            begin = m + 1; 
  
        else range = m - 1; 
    } 
    return -1; 
}
int findUspNumber_binary_search_and_delete(int uspNumberToSearch, INDEX_RECORD_LIST *list) {
	int range=list->size-1;
	int begin=0;
	int position=-1;
	int m;
	while (begin <= range) { 
        m = begin + (range - begin) / 2; 
  
        if (list->start[m]->record->nUsp == uspNumberToSearch) {
			position=list->start[m]->record->position;
			if (m<list->size-1) {
				memcpy (&list->start[m], &list->start[m+1], sizeof(INDEX_LIST_ELEMENT *) * (list->size-1 - m) );
			}
			list->size--;
			list->start = (INDEX_LIST_ELEMENT **) realloc(list->start,sizeof(INDEX_LIST_ELEMENT *) * (list->size));
			
            return position; 
		}
        if (list->start[m]->record->nUsp < uspNumberToSearch) 
            begin = m + 1; 
  
        else range = m - 1; 
    } 
    return -1; 
}
void searchRecordByNUSP(INDEX_RECORD_LIST *list, int nUspToSearch) {
	int recordPositionOnFile;
	recordPositionOnFile=findUspNumber_binary_search(nUspToSearch, list);
	if (recordPositionOnFile >= 0) {
		FILE *data_file_stream= fopen("dados.txt", "r+");
		typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));

		fseek(data_file_stream, recordPositionOnFile, SEEK_SET);
		fread(student, sizeof(typeStudent), 1, data_file_stream);

		printf ("%d %s %s %s %.1f\n", student->numUSP, student->name, student->lastName, student->course, student->score);

		free(student);
		fclose(data_file_stream);
	}
	else {
		printf("Aluno nao cadastrado! nUSP: %d\n", nUspToSearch);
	}
}

void searchRecordByLastName(INDEX_SEC_RECORD_LIST *list_sec, INDEX_RECORD_LIST *list_prim) {
	char lastNameToSearch[LAST_NAME_SIZE];
	FILE *inverted_list_file_stream= fopen("inverted_list.dat", "r+");
	INVERTED_LIST_RECORD *iterator = (INVERTED_LIST_RECORD *) malloc(sizeof(INVERTED_LIST_RECORD));
	int i;

	printf("Digite o sobrenome para buscar: \n");
	scanf (" %[^\n]s", lastNameToSearch);
	for (i=0; i<list_sec->size; i++) {
		if (strstr(list_sec->start[i]->lastName,lastNameToSearch)) {
			//Getting nUsp for all records in inverted list for provided lastName
			if (list_sec->start[i]->headPosition != -1) {
				//Printing full student record for all NUSP present in inverted_chained_list for provided last name
				fseek(inverted_list_file_stream, list_sec->start[i]->headPosition, SEEK_SET);
				fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
				searchRecordByNUSP(list_prim, iterator->nUsp);
				while (iterator->nxtValuePosition != -1) {
					fseek(inverted_list_file_stream, iterator->nxtValuePosition, SEEK_SET);
					fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
					searchRecordByNUSP(list_prim, iterator->nUsp);
				}
			}
		}
	}
	fclose(inverted_list_file_stream);
	free(iterator);
}

void deleteRecordByLastName(INDEX_SEC_RECORD_LIST *list_sec, char *lastNameToSearch, int nUSP) {
	FILE *inverted_list_file_stream= fopen("inverted_list.dat", "r+");
	INVERTED_LIST_RECORD *iterator = (INVERTED_LIST_RECORD *) malloc(sizeof(INVERTED_LIST_RECORD));
	INVERTED_LIST_RECORD *iterator_backing_track = (INVERTED_LIST_RECORD *) malloc(sizeof(INVERTED_LIST_RECORD));
	int i;
	int backingTrackPosition=-1;

	for (i=0; i<list_sec->size; i++) {
		//Find provided last name on secunday index
		if (strcmp(list_sec->start[i]->lastName,lastNameToSearch) == 0) {
			//Finding USP number in inverted list
			if (list_sec->start[i]->headPosition != -1) {
				fseek(inverted_list_file_stream, list_sec->start[i]->headPosition, SEEK_SET);
				fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
				if (iterator->nUsp == nUSP) {
					list_sec->start[i]->headPosition=iterator->nxtValuePosition;
				} 
				else {
					backingTrackPosition=list_sec->start[i]->headPosition;
					while (iterator->nxtValuePosition != -1) {
						iterator_backing_track->nUsp=iterator->nUsp;
						iterator_backing_track->nxtValuePosition=iterator->nxtValuePosition;
						fseek(inverted_list_file_stream, iterator->nxtValuePosition, SEEK_SET);
						fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
						if (iterator->nUsp == nUSP) {
							iterator_backing_track->nxtValuePosition=iterator->nxtValuePosition;
							fseek(inverted_list_file_stream, backingTrackPosition, SEEK_SET);
							fwrite(iterator_backing_track, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
							break;
						}
						backingTrackPosition=ftell(inverted_list_file_stream) - sizeof(INVERTED_LIST_RECORD);
					}
				}
				
			}
			break;
		}
	}
	fclose(inverted_list_file_stream);
	free(iterator);
	free(iterator_backing_track);
}
void deleteAllRecordsByLastName(INDEX_RECORD_LIST *list_prim, INDEX_SEC_RECORD_LIST *list_sec) {
	char lastNameToSearch[LAST_NAME_SIZE];
	printf("Digite o sobrenome para excluir. Todos os alunos com esse sobrenome serao excluidos.\n");
	scanf (" %[^\n]s", lastNameToSearch);
	FILE *inverted_list_file_stream= fopen("inverted_list.dat", "r+");
	INVERTED_LIST_RECORD *iterator = (INVERTED_LIST_RECORD *) malloc(sizeof(INVERTED_LIST_RECORD));
	int i;

	for (i=0; i<list_sec->size; i++) {
		if (strcmp(list_sec->start[i]->lastName,lastNameToSearch) == 0) {
			//Getting all USP number and deleting records for provided last name
			if (list_sec->start[i]->headPosition != -1) {
				fseek(inverted_list_file_stream, list_sec->start[i]->headPosition, SEEK_SET);
				fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
				deleteRecord(list_prim, list_sec, iterator->nUsp);
				while (iterator->nxtValuePosition != -1) {
					fseek(inverted_list_file_stream, iterator->nxtValuePosition, SEEK_SET);
					fread(iterator, sizeof(INVERTED_LIST_RECORD), 1, inverted_list_file_stream);
					deleteRecord(list_prim, list_sec, iterator->nUsp);
				}
				
				
			}
			break;
		}
	}
	fclose(inverted_list_file_stream);
	free(iterator);
}
void deleteRecord(INDEX_RECORD_LIST *list_prim, INDEX_SEC_RECORD_LIST *list_sec, int nUspToSearch) {
	int recordPositionOnFile;
	//Deleting from primary index and getting record position on data file
	recordPositionOnFile=findUspNumber_binary_search_and_delete(nUspToSearch, list_prim);
	if (recordPositionOnFile >= 0) {
		FILE *data_file_stream= fopen("dados.txt", "r+");
		typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));

		//Getting record from data file
		fseek(data_file_stream, recordPositionOnFile, SEEK_SET);
		fread(student, sizeof(typeStudent), 1, data_file_stream);

		//Deleting from secundary index
		deleteRecordByLastName(list_sec, student->lastName, nUspToSearch);

		//Marking record as deleted and writing on datafile
		student->isDeleted=1;
		fseek(data_file_stream, recordPositionOnFile, SEEK_SET);
		fwrite(student, sizeof(typeStudent), 1, data_file_stream);
		printf("Aluno apagado: %d %s %s\n", student->numUSP, student->name, student->lastName);


		free(student);
		fclose(data_file_stream);
	}
	else {
		printf("Aluno nao cadastrado! nUSP: %d\n", nUspToSearch);
	}
}
void readToWriteRecord(INDEX_RECORD_LIST *listPrim, INDEX_SEC_RECORD_LIST *listSec) {
	typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));
	INDEX_RECORD *index_record = (INDEX_RECORD *) malloc(sizeof(INDEX_RECORD));
	INDEX_SEC_RECORD *index_sec_record = (INDEX_SEC_RECORD *) malloc(sizeof(INDEX_SEC_RECORD));

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

	writeRecord(listPrim, listSec, student, index_record, index_sec_record);

	free (index_record);
	free(index_sec_record);
	free(student);
}
void writeRecord(INDEX_RECORD_LIST *listPrim, INDEX_SEC_RECORD_LIST *listSec, typeStudent *student, INDEX_RECORD *index_record, INDEX_SEC_RECORD *index_sec_record) {

	FILE *data_file_stream= fopen("dados.txt", "a+");


	fwrite(student, sizeof(typeStudent), 1, data_file_stream);
	index_record->position = ftell(data_file_stream) - sizeof(typeStudent);
	index_record->nUsp = student->numUSP;

	strcpy(index_sec_record->lastName, student->lastName);
	index_sec_record->headPosition = -1;

	add_index_element(listPrim, index_record);
	add_index_sec_element(listSec, index_sec_record, 1, index_record->nUsp);

	fclose(data_file_stream);

}
void print_index_list(INDEX_RECORD_LIST *list) {
	INDEX_LIST_ELEMENT **start = list->start;
	printf("PRINTING INDEX LIST IN MEMORY: \n");
	printf("LIST_SIZE: %d\n", list->size);
	for (int i=0; i<list->size; i++) {
		printf("LIST: nUsp: %d, position in file: %d\n", start[i]->record->nUsp, start[i]->record->position);
		
	}
}

void geradorAlunos(INDEX_RECORD_LIST *listPrim, INDEX_SEC_RECORD_LIST *listSec, INDEX_RECORD *index_record, INDEX_SEC_RECORD *index_sec_record)
{
	int i;
	for(i=0;i<data_size;i++)
	{
		int nUSP = rand() % 100000;
		typeStudent *student = (typeStudent *) malloc(sizeof(typeStudent));
		student->numUSP = nUSP;
		strcpy(student->name, "Hugo");
		strcpy(student->lastName, "Cruz");
		strcpy(student->course, "BSI");
		student->score = 8.0;
		student->isDeleted = 0;
		writeRecord(listPrim, listSec, student, index_record, index_sec_record);
		printf("Student inserted: %d\n",nUSP);
		free(student);
	}
}
