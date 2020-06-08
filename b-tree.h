/*  
    Daniel De Marco Fucci
	Hugo Fellipe Lage Alvarães da Cruz
*/
#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H
#define data_size 128
#define ERR_NOT_FOUND -1

#define LAST_NAME_SIZE 50

//Using below order number, we have node size being 4kb
#define B_TREE_ORDER 171

#include <stdio.h>


#define TAM 16


extern int tree_size;

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
	int nodeSize;
	long int keyVector[B_TREE_ORDER-1];
	long int dataRrnVector[B_TREE_ORDER-1];
	long int childrenVector[B_TREE_ORDER];
} B_TREE_NODE;

typedef struct {
	int nodesNumber;
	int order;
	long int rootNodeRRN;
} B_TREE_HEADER;

void readToWriteRecord(B_TREE_HEADER *);
void writeRecord(B_TREE_HEADER *, typeStudent *);
void showAll();
<<<<<<< HEAD
void searchRecordByNUSP(B_TREE_HEADER *, int);
B_TREE_HEADER* loadBTreeHeader();
void print_b_tree(B_TREE_HEADER *);
int findUspNumber(int, B_TREE_HEADER *);
void printNodeSize();
B_TREE_NODE * createNewNode(B_TREE_HEADER *header);
void freeNode(B_TREE_NODE *);
int alreadyInUse (int, B_TREE_HEADER *);
void generateStudents(int, B_TREE_HEADER *);

#endif
=======
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
void generateStudents();
void loadBTree(PAGE *pageList);
void addToBTree(PAGE *);
int checkSons(int[]);
void swap(int*, int*);
int swapValues(PAGE *);
void insertOrdered(searchKey *,int ,int );
void setLeafSons(PAGE *);

#endif
>>>>>>> 6923a4d3b21f90457d13113a4411d866a51cc216
