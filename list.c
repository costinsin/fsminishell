#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/list.h"
#include "header/file.h"

//initializes the linked list
void init(linkedList *a)
{
	a->countElem = 0;
	a->start = NULL;
}

//adds a new node at the end of the list
void add(linkedList *a, void *data, size_t data_size)
{
	node *it = a->start;
	node *tmp;
	int i;

	tmp = malloc(sizeof(node));
	tmp->data = malloc(data_size);
	for (i = 0; i < data_size; i++)
		*(char *)(tmp->data + i) = *(char *)(data + i);
	tmp->next = NULL;
	free(data);
	if (it == NULL) {
		a->start = tmp;
		return;
	}
	while (it->next != NULL)
		it = it->next;
	it->next = tmp;
}

//adds a new node before the 'beforeMe' node
void addBefore(linkedList *a, void *data, size_t data_size, node *beforeMe)
{
	node *it = a->start;
	node *tmp;
	int i;

    //allocates memory for the new node
	tmp = malloc(sizeof(node));
	tmp->data = malloc(data_size);
	for (i = 0; i < data_size; i++)
		*(char *)(tmp->data + i) = *(char *)(data + i);
	free(data);

	//makes the connections in the linked list
	if (a->start == beforeMe) {
		tmp->next = a->start;
		a->start = tmp;
		return;
	}
	while (it->next != beforeMe) {
		it = it->next;
		if (it->next == NULL)
			break;
	}
	tmp->next = it->next;
	it->next = tmp;
}

//prints the list
void showList(linkedList *a, char type[])
{
	node *it = a->start;

	while (it != NULL) {
		if (strcmp(type, "dir") == 0)
			printf("%s ", ((Directory *)(it->data))->name);
		else if (strcmp(type, "file") == 0)
			printf("%s ", ((File *)(it->data))->name);
		it = it->next;
	}
}

//removes node from the linked list and frees the memory for this node\
and other nodes that were allocated for this node's linked lists
void removeNode(linkedList *a, node *toRemove, char type[])
{
	node *it;
	// Disconnects 'toRemove' from the list and redoes the connections
	if (a->start == toRemove)
		a->start = a->start->next;
	else {
		it = a->start;
		while (it->next != toRemove)
			it = it->next;
		if(it->next == NULL)
			return;
		it->next = it->next->next;
	}

	(a->countElem)--; //element count goes down by 1

	//frees the memory for subdirectories and subfiles
	if (!strcmp(type, "dir")) {
		linkedList *fileList =
			&(((Directory *)(toRemove->data))->files);
		linkedList *dirList =
			&(((Directory *)(toRemove->data))->directories);

		while (fileList->start != NULL)
			removeNode(fileList, fileList->start, "file");
		while (dirList->start != NULL)
			removeNode(dirList, dirList->start, "dir");
		free(((Directory *)(toRemove->data))->name);
		free(((Directory *)(toRemove->data)));
		free(toRemove);
	} else if (!strcmp(type, "file")) {
		free(((File *)(toRemove->data))->name);
		free(((File *)(toRemove->data))->data);
		free(((File *)(toRemove->data)));
		free(toRemove);
	}
}
