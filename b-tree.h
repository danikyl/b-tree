/*  
    Hugo Fellipe Lage Alvarães da Cruz  10903872
    Daniel De Marco Fucci               
*/
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#define data_size 128
#define order 8

#define LAST_NAME_SIZE 50

#include <stdio.h>


#define TAM 16


typedef int tipoChave;

typedef struct {
	tipoChave numUSP;
	char name[TAM];
	char lastName[TAM];
	char course[TAM];
	float score;
	int isDeleted;
} typeStudent;

typedef struct {
	tipoChave nusp;
	int rrn;
} searchKey;

typedef struct {
	//int contador;
	searchKey keys[order-1]; //assumindo chaves char
	int sons[order]; //armazena o RRN dos filhos
	int leaf;//1 = leaf, 0 = not leaf
} PAGE; 


typedef struct {
	int nUsp;
	int position;
} INDEX_RECORD;

typedef struct {
	int nUsp;
	int nxtValuePosition;
} INVERTED_LIST_RECORD;

typedef struct {
	char lastName[LAST_NAME_SIZE];
	int headPosition;
} INDEX_SEC_RECORD;

typedef struct INDEX_LIST_ELEMENT INDEX_LIST_ELEMENT;

struct INDEX_LIST_ELEMENT{
	INDEX_RECORD *record;
};

typedef struct {
	INDEX_LIST_ELEMENT **start;
	int size;
} INDEX_RECORD_LIST;

typedef struct {
	INDEX_SEC_RECORD **start;
	int size;
} INDEX_SEC_RECORD_LIST;

void readToWriteRecord(INDEX_RECORD_LIST *, INDEX_SEC_RECORD_LIST *);
int writeRecord(typeStudent *);
void showAll();
void searchRecordByNUSP(INDEX_RECORD_LIST *, int);
void deleteRecord(INDEX_RECORD_LIST *, INDEX_SEC_RECORD_LIST *, int);
void loadIndexPrim(INDEX_RECORD_LIST *);
void loadIndexSec(INDEX_SEC_RECORD_LIST *);
void add_index_element(INDEX_RECORD_LIST *, INDEX_RECORD*);
void print_index_list(INDEX_RECORD_LIST *);
void exitAndSave(INDEX_RECORD_LIST *, INDEX_SEC_RECORD_LIST *);
void print_index_file();
int findUspNumber_binary_search(int, INDEX_RECORD_LIST *);
int findUspNumber_binary_search_and_delete(int, INDEX_RECORD_LIST *);
int add_to_inverted_list(INDEX_SEC_RECORD *, int);
void add_index_sec_element(INDEX_SEC_RECORD_LIST *, INDEX_SEC_RECORD *, int, int);
void print_sec_index_list(INDEX_SEC_RECORD_LIST *);
void searchRecordByLastName(INDEX_SEC_RECORD_LIST *, INDEX_RECORD_LIST *);
void deleteRecordByLastName(INDEX_SEC_RECORD_LIST *, char *, int);
void deleteAllRecordsByLastName(INDEX_RECORD_LIST *, INDEX_SEC_RECORD_LIST *);
void geradorAlunos(INDEX_RECORD_LIST *, INDEX_SEC_RECORD_LIST *, INDEX_RECORD *, INDEX_SEC_RECORD *);
void loadBTree(PAGE *pageList);
void addToBTree(PAGE *);

#endif
