#ifndef _LIST_H__
#define _LIST_H__


/* 头文件包含 */
#include <stdlib.h>
#include <stdio.h>


typedef struct File_List
{
	char* Name;
	char* Path;
	struct File_List* Next;
	struct File_List* Previous;
}File_List;

typedef struct Group_List
{
	char* Name;
	struct File_List* Files;
	struct Group_List* Next;
	struct Group_List* Previous;
}Group_List;




/* 函数声明 */
	/* 这部分是关于组链表操作 */
Group_List* List_Group_Create(void);
Group_List* List_Group_Find(Group_List* groupHead, char* groupname);
char List_Group_Add(Group_List* head, Group_List* group);
void List_Group_Del(Group_List* group);
char List_Group_Del_Name(Group_List* head, char* name);
void List_Group_print(Group_List* head);
char List_Group_Rename(Group_List* ghead, char* OldName, char* NewName);


	/* 这部分是关于文件链表操作 */
File_List* List_File_Create(void);
File_List* List_File_Find(File_List* listHead, char* filename);
Group_List* List_File_Find_G(Group_List* listHead, char* filename);
char List_File_Add(Group_List* group, File_List* file, Group_List* ghead);
void List_File_Del(File_List* file);
char List_File_Del_Name(Group_List* ghead, char* name);
void List_File_print_G(Group_List* group);
void List_File_print(Group_List* head);
char List_File_Rename(Group_List* ghead, char* OldName, char* NewName);
#endif // !_LIST_H__

