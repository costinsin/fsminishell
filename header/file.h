#ifndef __FILE_H__
#define __FILE_H__

typedef struct Directory {
	// The name of the directory
	char *name;

	// The list of files of the current directory
	linkedList files;

	// The list of directories of the current directory
	linkedList directories;

	// The parent directory of the current directory
	struct Directory *parentDir;
} Directory;

typedef struct File {
	// The name of the file
	char *name;

	// The size of the file
	int size;

	// The content of the file
	char *data;

	// The directory in which the file is located
	Directory *dir;
} File;

void createFs(Directory *root);

void deleteFS(Directory *root);

void touch(Directory *current, char *filename, char *content);

void mkdir(Directory *current, char *filename);

void ls(Directory *current);

void pwd(Directory *current);

void cd(Directory **current, char *dirName);

void tree(Directory *current, int depth);

void rm(Directory *current, char *filename);

void rmdir(Directory *current, char *dirname);

void find(Directory *current, int depth, int max_depth,
int min_size, int max_size, char *subcontent);

#endif /* __FILE_H__ */
