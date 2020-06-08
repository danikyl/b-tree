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