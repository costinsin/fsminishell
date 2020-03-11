#ifndef __LIST_H__
#define __LIST_H__

typedef struct node {
	// A pointer to the node data
	void *data;

	// A pointer to the next node
	struct node *next;
} node;

typedef struct linkedList {
	// An element count in the linked list
	int countElem;

	// A pointer to the starting node
	node *start;
} linkedList;

void init(linkedList *a);

void add(linkedList *a, void *data, size_t data_size);

void addBefore(linkedList *a, void *data, size_t data_size, node *beforeMe);

void removeNode(linkedList *a, node *toRemove, char type[]);

void showList(linkedList *a, char type[]);

#endif // __LIST_H__
