/*********************************************
*	这个文件是关于组链表和文件链表的使用
*	链表使用前用List_xxxx_Create()创建链表头
**********************************************/

/* 头文件引用 */
#include "list.h"
#include "Str_Process.h"

/**
* @brief	创建一个组链表，返回链表头指针
*/
Group_List* List_Group_Create(void)
{
	Group_List* buf = (Group_List*)malloc(sizeof(Group_List));
	buf->Next = buf;
	buf->Previous = buf;
	return buf;
}

/**
* @brief	根据组名寻找组，成功返回指针，失败返回0
* @param	groupHead	组链表头
* @param	filename	要寻找的组名
*/
Group_List* List_Group_Find(Group_List* groupHead , char *groupname)
{
	Group_List* buf;
	buf = groupHead->Next;
	while (buf != groupHead)
	{
		if (Str_Compare(buf->Name , groupname) == 1)
		{
			return buf;
		}
		buf = buf->Next;
	}
	return 0;
}

/**
* @brief	向组列表末尾添加一个组，自动验证组名
* @param	group	要添加的组
* @retval	成功返回1，失败返回NULL
*/
char List_Group_Add(Group_List* head, Group_List* group)
{
	if (List_Group_Find(head, group->Name) != NULL)
	{
		return 0;
	}
	group->Next = head;
	group->Previous = head->Previous;
	head->Previous->Next = group;
	head->Previous = group;
	return 1;
}

/**
* @brief	从组列表中删除一个组
* @param	group	要删除的组
*/
void List_Group_Del(Group_List* group)
{
	//要删除这个组下的所有文件，文件的文件名和文件路径都要释放
	File_List* buf;
	buf = group->Files->Next;
	while (buf != group->Files)
	{
		List_File_Del(buf);
		buf = group->Files->Next;
	}
	group->Previous->Next = group->Next;
	group->Next->Previous = group->Previous;
	free(group);
}

/**
* @brief	根据组名从列表中删除一个组
* @param	head	组链表头
* @param	name	组名
* @retval	删除成功返回1，否则返回0
*/
char List_Group_Del_Name(Group_List* head, char* name)
{
	Group_List *buf;
	buf = List_Group_Find(head , name);
	if (buf == 0)return 0;
	List_Group_Del(buf);
	return 1;
}


/**
* @brief	打印组列表
*/
void List_Group_print(Group_List* head)
{
	Group_List* buf;
	buf = head->Next;
	while (buf != head)
	{
		printf("%s\r\n" , buf->Name);
		buf = buf->Next;
	}
}

/**
* @brief	创建一个文件链表，返回链表头指针
*/
File_List* List_File_Create(void)
{
	File_List* buf = (File_List*)malloc(sizeof(File_List));
	buf->Next = buf;
	buf->Previous = buf;
	return buf;
}


/**
* @brief	在组链表中，根据文件名寻找文件，成功返回指针，失败返回0
* @param	listHead	文件链表头
* @param	filename	要寻找的文件名
*/
File_List* List_File_Find(File_List* listHead, char* filename)
{
	File_List* buf;
	buf = listHead->Next;
	while (buf != listHead)
	{
		if (Str_Compare(buf->Name, filename) == 1)
		{
			return buf;
		}
		buf = buf->Next;
	}
	return 0;
}

/**
* @brief	在所有组中，根据文件名找文件，成功返回文件所在组
* @param	listHead	组链表头
* @param	filename
*/
Group_List* List_File_Find_G(Group_List* listHead, char* filename)
{
	Group_List* buf;
	buf = listHead->Next;
	while (buf != listHead)
	{
		if (List_File_Find(buf->Files, filename))
		{
			return buf;
		}
		buf = buf->Next;
	}
	return 0;
}

/**
* @brief	向文件链表末尾添加一个文件，自动验证文件名
* @param	fhead	要添加的文件链表
* @param	file	要添加的文件
* @param	ghead	组链表头，用来判断文件是否存在
* @retval	成功返回1，失败返回NULL
*/
char List_File_Add(Group_List* group, File_List* file, Group_List* ghead)
{
	if (List_File_Find_G(ghead, file->Name) != NULL)
	{
		return 0;
	}
	file->Next = group->Files;
	file->Previous = group->Files->Previous;
	group->Files->Previous->Next = file;
	group->Files->Previous = file;
	return 1;
}


/*
* @brief	打印一个组下的所有文件
*/
void List_File_print_G(Group_List* group)
{
	File_List* buf;
	buf = group->Files->Next;
	while (buf != group->Files)
	{
		printf("%s\t" , buf->Name);
		buf = buf->Next;
	}
	printf("\r\n\r\n");
}

/**
* @brief    文件打印，所有组的所有文件
*/
void List_File_print(Group_List* head)
{
	Group_List* buf;
	buf = head->Next;
	while (buf != head)
	{
		printf("组：%s\r\n" , buf->Name);
		List_File_print_G(buf);

		buf = buf->Next;
	}
}


/**
* @brief	删除一个文件
*/
void List_File_Del(File_List* file)
{
	file->Previous->Next = file->Next;
	file->Next->Previous = file->Previous;

	free(file->Name);
	free(file->Path);
	free(file);
}


/**
* @brief	根据文件名从链表中删除一个文件
* @param	ghead	组链表头
* @param	name	要删除的文件名
* @retval	删除成功返回1，否则返回0
*/
char List_File_Del_Name(Group_List*ghead , char* name)
{
	Group_List* buf;	//先找到文件所在组
	buf = List_File_Find_G(ghead , name);
	if (buf == NULL)
	{
		return 0;
	}
	List_File_Del(List_File_Find(buf->Files, name));

	return 1;
}

/**
* @brief	改变组名，会自动寻找组,自动寻找新名字是否重复
* @retval	成功返回1，失败返回0
* 由于指针是malloc申请的，所以在更改名字时应当释放他
*/
char List_Group_Rename(Group_List* ghead,char* OldName, char* NewName)
{
	Group_List* buf;
	buf = List_Group_Find(ghead, OldName);
	if (buf == 0)		//没有找到组
	{
		return 0;
	}
	else
	{
		if (List_Group_Find(ghead, NewName) != 0)	//新名字重复
		{
			return 0;
		}
		else
		{
			if (buf->Name != NULL)free(buf->Name);
			buf->Name = Str_Middle(NewName , 0 , Str_Len(NewName));
		}
	}
	return 1;
}

/**
* @brief	改变文件名，会自动寻找文件并检查新文件名是否重复
* @retval	成功返回1，失败返回0
* 由于指针是malloc申请的，所以在更改名字时应当释放他
* 文件名还是不改的比较好，有需求可以使用
*/
char List_File_Rename(Group_List* ghead, char* OldName, char* NewName)
{
	Group_List* gbuf;
	File_List* fbuf;
	gbuf = List_File_Find_G(ghead, OldName);
	if (gbuf == 0)		//没有找到文件
	{
		return 0;
	}
	else
	{
		if (List_File_Find_G(ghead, NewName) != 0)		//新名字重复
		{
			return 0;
		}
		else
		{
			fbuf = List_File_Find(gbuf->Files, OldName);	//找到文件所在指针
			if (fbuf->Name != NULL)free(fbuf->Name);		//释放它
			fbuf->Name = Str_Middle(NewName, 0, Str_Len(NewName));
		}
	}
	return 1;
}

