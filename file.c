#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/list.h"
#include "header/file.h"

//creates the file system
void createFs(Directory *root)
{
	root->name = malloc(sizeof(char) * 30);
	strcpy(root->name, "");
	init(&(root->files));
	init(&(root->directories));
	root->parentDir = NULL;
}

//deletes the file system
void deleteFS(Directory *root)
{
	free(root->name);
	root->name = NULL;
	init(&(root->files));
	init(&(root->directories));
}

//creates a new file in the file system
void touch(Directory *current, char *filename, char *content)
{
	linkedList *fileList = &(current->files);
	node *it = fileList->start;
	File *newfile;

	//initializing new file
	newfile = malloc(sizeof(File));
	newfile->name = malloc(sizeof(char) * 30);
	strcpy(newfile->name, filename);
	newfile->size = strlen(content);
	newfile->data = malloc(sizeof(char) * 30);
	strcpy(newfile->data, content);
	newfile->dir = current;

	//searching it's lexicographic place in the list
	while (it != NULL &&
	strcmp(((File *)(it->data))->name, filename) <= 0) {
		if (strcmp(((File *)(it->data))->name, filename) == 0)
			return;
	it = it->next;
	}

	//adding it in the right lexicographic place
	addBefore(fileList, newfile, sizeof(File), it);
}

//creates a new directory in the file system
void mkdir(Directory *current, char *dirname)
{
	linkedList *dirList = &(current->directories);
	node *it = dirList->start;
	Directory *newdir;

	//initializing new directory
	newdir = malloc(sizeof(Directory));
	newdir->name = malloc(sizeof(char) * 30);
	strcpy(newdir->name, dirname);
	init(&(newdir->files));
	init(&(newdir->directories));
	newdir->parentDir = current;

	//searching it's lexicographic place in the list
	while (it != NULL &&
	strcmp(((Directory *)(it->data))->name, dirname) <= 0) {
		if (strcmp(((Directory *)(it->data))->name, dirname) == 0)
			return;
		it = it->next;
	}

	//adding it in the right lexicographic place
	addBefore(dirList, newdir, sizeof(Directory), it);
}

//prints the files and then the directories on the screen
void ls(Directory *current)
{
	linkedList *fileList = &(current->files);
	linkedList *dirList = &(current->directories);

	showList(fileList, "file");
	showList(dirList, "dir");
	printf("\n");
}

//prints the current directory path from root recursively
void pwd(Directory *current)
{
	Directory *it = current;

	if (!(strcmp(it->name, ""))) {
		printf("/");
		return;
	}
	pwd(current->parentDir);
	if (!(strcmp(current->parentDir->name, "")))
		printf("%s", current->name);
	else
		printf("/%s", current->name);
}

//changes directory to 'dirName' (which is inside current directory)
void cd(Directory **current, char *dirName)
{
	linkedList *dirList = &((*current)->directories);
	node *it = dirList->start;

	//searches the directory list and if it finds a match changes\
	the current directory and returns
	while (it != NULL) {
		if (!strcmp(dirName, ((Directory *)(it->data))->name)) {
			(*current) = ((Directory *)(it->data));
			return;
		}
		it = it->next;
	}

	//if no match was found checks for ".." to go back, else prints\
	error message
	if (!strcmp(dirName, "..")) {
		if ((*current)->parentDir != NULL)
			(*current) = (*current)->parentDir;
	} else
		printf("Cannot move to '%s': No such directory!\n", dirName);
}

//shows the file hierarchy starting from the current directory
void tree(Directory *current, int depth)
{
	linkedList *fileList = &(current->files);
	linkedList *dirList = &(current->directories);
	node *it;
	int i;

    //prints the current directory files with (depth * TAB)
	it = fileList->start;
	while (it != NULL) {
		for (i = 1; i <= depth; i++)
			printf("    ");
		printf("%s\n", ((File *)(it->data))->name);
		it = it->next;
	}
	//prints the current directory files with (depth * TAB)\
	and then calls the function recursively with depth + 1
	it = dirList->start;
	while (it != NULL) {
		for (i = 1; i <= depth; i++)
			printf("    ");
		printf("%s\n", ((Directory *)(it->data))->name);
		tree((Directory *)(it->data), depth + 1);
		it = it->next;
	}
}

//removes 'filename' from the current directory if it exists
void rm(Directory *current, char *filename)
{
	linkedList *fileList = &(current->files);
	node *it;

	//searches the file list and removes the right node
	it = fileList->start;
	while (it != NULL) {
		if (!strcmp(((File *)(it->data))->name, filename)) {
			removeNode(fileList, it, "file");
			return;
		}
		it = it->next;
	}
	printf("Cannot remove '%s': No such file!\n", filename);
}

//removes 'dirname' from the current directory if it exists
void rmdir(Directory *current, char *dirname)
{
	linkedList *dirList = &(current->directories);
	node *it;

	//searches the directory list and removes the right node
	it = dirList->start;
	while (it != NULL) {
		if (!strcmp(((Directory *)(it->data))->name, dirname)) {
			removeNode(dirList, it, "dir");
			return;
		}
		it = it->next;
	}
	printf("Cannot remove '%s': No such directory!\n", dirname);
}

//finds all the files that meet the specified criteria
void find(Directory *current, int depth, int max_depth, int min_size,
int max_size, char *subcontent)
{
	linkedList *fileList = &(current->files);
	linkedList *dirList = &(current->directories);
	node *it;
	File *file;

    //checks for files inside current directory
	it = fileList->start;
	while (it != NULL) {
		file = ((File *)(it->data));
		if (strstr(file->data, subcontent) != NULL && strlen(file->data)
		>= min_size && strlen(file->data) <= max_size)
			printf("%s ", file->name);
		it = it->next;
	}

	//increases the depth by 1 and checks the directories
	it = dirList->start;
	while (it != NULL) {
		if (depth + 1 <= max_depth)
			find(((Directory *)(it->data)), depth+1, max_depth,
			min_size, max_size, subcontent);
		it = it->next;
	}
}
