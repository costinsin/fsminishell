#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "file.h"

Directory *runCommand(char args[][100], Directory *root, Directory *current)
{
	//checks every case and applies the correct function
	if (!strcmp(args[0], "create") && !strcmp(args[1], "fs")) {
		createFs(root);
	} else if (!strcmp(args[0], "delete") && !strcmp(args[1], "fs")) {
		deleteFS(root);
		return NULL;
	} else if (!strcmp(args[0], "touch"))
		touch(current, args[1], args[2]);
	else if (!strcmp(args[0], "mkdir"))
		mkdir(current, args[1]);
	else if (!strcmp(args[0], "ls"))
		ls(current);
	else if (!strcmp(args[0], "pwd")) {
		pwd(current);
		printf("\n");
	} else if (!strcmp(args[0], "cd"))
		cd(&current, args[1]);
	else if (!strcmp(args[0], "tree")) {
		printf("/%s\n", current->name);
		tree(current, 1);
	} else if (!strcmp(args[0], "rm"))
		rm(current, args[1]);
	else if (!strcmp(args[0], "rmdir"))
		rmdir(current, args[1]);
	else if (!strcmp(args[0], "find")) {
		find(current, 0, atoi(args[1]), atoi(args[2]),
		atoi(args[3]), args[4]);
		printf("\n");
	}
	return current;
}

//reads the lines from STDIN and devides them into words\
then calls the runCommand() function with the devided arguments
void readLines(void)
{
	char line[100];
	char args[10][100];
	char *pch;
	int wordNo;
	Directory root, *current;

	current = &root;
	while (1) {
		//parsing the words into args char matrix
		fgets(line, 100, stdin);
		line[strlen(line) - 1] = '\0';
		pch = strtok(line, " ");
		wordNo = -1;
		while (pch != NULL) {
			strcpy(args[++wordNo], pch);
			pch = strtok(NULL, " ");
		}
		//update current directory after command was run
		current = runCommand(args, &root, current);
		if (current == NULL) //only happens when fs is deleted
			break;
		args[0][0] = '\0'; //reset arg[0] string in case there\
							is no input
	}
}

int main(void)
{
	readLines();
	return 0;
}
