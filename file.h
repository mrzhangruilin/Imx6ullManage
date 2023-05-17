#ifndef _FILE_H__
#define	_FILE_H__

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

/* 结构体声明 */
typedef struct TargetConfig
{
	char* name;				//工程名
	char* include_path;		//头文件包含路径
	char* output_path;		//输出路径
	char* output_name;		//输出文件名
	char* download_path;	//下载路径
}TargetConfig;


/* 函数声明 */
char File_Verify(char* fdata);
char File_OpenRead(char* filename, FILE** fp, char** fdata);
char File_Load_Target(char* fdata, TargetConfig* target);
void File_Load_Group(char* fdata, Group_List* group);
char File_Load(char* fdata, TargetConfig* target, Group_List* head);
void File_Save(char* filename, TargetConfig* target, Group_List* ghead);
char File_Access(char* filename);
#endif // !_FILE_H__

