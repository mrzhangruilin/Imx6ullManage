#ifndef _MANAGE_H__
#define	_MANAGE_H__

/* 头文件包含 */
#include <stdio.h>
#include "file.h"
#include "list.h"
#include "Str_Process.h"
#include "display.h"

/* 宏定义 */
#define ERROR_FATAL_UNKNOWN		255
#define	ERROR_FATAL_NO_FILE		1
#define ERROR_FATAL_OPEN_FAIL	2
#define ERROR_FATAL_READ_FAIL	3
#define ERROR_FATAL_NO_INPUT	4



/* 函数声明 */
void Error_Handle(char Type);

	//编译类
void Item_Target_Build(TargetConfig* target, Group_List* ghead);
void Item_Target_GCC(TargetConfig* target, Group_List* ghead);
void Item_Target_LD(TargetConfig* target, Group_List* ghead);
void Item_Target_Bin(TargetConfig* target);
void Item_Target_Dump(TargetConfig* target);

	//更改类
char Item_Change_OPpath(TargetConfig* target, char* path);
char Item_Change_Ipath(TargetConfig* target, char* path);
char Item_Change_TName(TargetConfig* target, char* name);
char Item_Change_OPName(TargetConfig* target, char* name);
char Item_Change_Dpath(TargetConfig* target, char* path);

	//文件类
char Item_File_Add(Group_List* ghead, char* group, char* filename);
char Item_File_Del(Group_List* ghead, char* filename);
char Item_Group_Add(Group_List* ghead, char* groupName);
char Item_Group_Del(Group_List* ghead, char* groupName);
#endif // !_MANAGE_H__
