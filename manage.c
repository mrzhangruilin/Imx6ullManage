#include "manage.h"


/*
* @brief	编译生成.o文件
*/
void Item_Target_GCC(TargetConfig* target, Group_List* ghead)
{
	char* command=0;
	Group_List* gbuf;
	File_List* fbuf;

	gbuf = ghead->Next;
	while (gbuf!=ghead)
	{
		fbuf = gbuf->Files->Next;
		while (fbuf!=gbuf->Files)
		{
			Str_Assignment(&command, "arm-linux-genuibehf-gcc -c -O2 ");
			Str_Assignment(&command, Str_Cat(command , Str_Replace(target->include_path, " ", " -I ", 0, 0)));
			Str_Assignment(&command, Str_Cat(command, " -o "));
			Str_Assignment(&command, Str_Cat(command, target->output_path));
			if (Str_Find(fbuf->Name, ".c", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".c", ".o", 0, 1)));
			}else if(Str_Find(fbuf->Name, ".C", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".C", ".o", 0, 1)));
			}
			else if (Str_Find(fbuf->Name, ".s", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".s", ".o", 0, 1)));
			}
			else if (Str_Find(fbuf->Name, ".S", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".S", ".o", 0, 1)));
			}
			else
			{
				free(command);
				command = 0;
				fbuf = fbuf->Next;
				continue;
			}
			Str_Assignment(&command, Str_Cat(command, " "));
			Str_Assignment(&command, Str_Cat(command, fbuf->Path));
			printf("%s\r\n" , command);
			//if (system(command) != 0)Error_Handle(ERROR_FATAL_UNKNOWN);
			
			free(command);
			command = 0;
			fbuf = fbuf->Next;
		}
		gbuf = gbuf->Next;
	}
}

/*
* @brief	链接文件
*/
void Item_Target_LD(TargetConfig* target, Group_List* ghead)
{
	char* command=0;
	Group_List* gbuf;
	File_List* fbuf;

	Str_Assignment(&command, Str_Cat(command, "arm-linux-gneuabihf-ld -Tim6ull.lds"));
	
	gbuf = ghead->Next;
	while (gbuf != ghead)
	{
		fbuf = gbuf->Files->Next;
		while (fbuf != gbuf->Files)
		{
			Str_Assignment(&command, Str_Cat(command, " "));
			Str_Assignment(&command, Str_Cat(command, target->output_path));
			if (Str_Find(fbuf->Name, ".c", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".c", ".o", 0, 1)));
			}
			else if (Str_Find(fbuf->Name, ".C", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".C", ".o", 0, 1)));
			}
			else if (Str_Find(fbuf->Name, ".s", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".S", ".o", 0, 1)));
			}
			else if (Str_Find(fbuf->Name, ".S", 0) != -1)
			{
				Str_Assignment(&command, Str_Cat(command, Str_Replace(fbuf->Name, ".s", ".o", 0, 1)));
			}
			fbuf = fbuf->Next;
		}
		gbuf = gbuf->Next;
	}
	Str_Assignment(&command, Str_Cat(command, " -o "));
	Str_Assignment(&command, Str_Cat(command, target->output_path));
	Str_Assignment(&command, Str_Cat(command, target->output_name));
	Str_Assignment(&command, Str_Cat(command, ".elf"));
	printf("%s\r\n" , command);
	//if (system(command) != 0)Error_Handle(ERROR_FATAL_UNKNOWN);
}

/**
* @brief	生成bin文件
*/
void Item_Target_Bin(TargetConfig* target)
{
	char* command;

	command = 0;
	Str_Assignment(&command, Str_Cat(command, "arm-linux-gneuabihf-objcopy -O binary -S -g "));
	Str_Assignment(&command, Str_Cat(command, target->output_path));
	Str_Assignment(&command, Str_Cat(command, target->output_name));
	Str_Assignment(&command, Str_Cat(command, ".elf "));
	Str_Assignment(&command, Str_Cat(command, target->output_path));
	Str_Assignment(&command, Str_Cat(command, target->output_name));
	Str_Assignment(&command, Str_Cat(command, ".bin"));


	printf("%s\r\n", command);
	//if (system(command) != 0)Error_Handle(ERROR_FATAL_UNKNOWN);
}

/**
* @brief	生成反汇编
*/
void Item_Target_Dump(TargetConfig* target)
{
	char* command;

	command = 0;
	Str_Assignment(&command, Str_Cat(command, "arm-linux-gneuabihf-objdump -D "));

	Str_Assignment(&command, Str_Cat(command, target->output_path));
	Str_Assignment(&command, Str_Cat(command, target->output_name));
	Str_Assignment(&command, Str_Cat(command, ".elf > "));
	Str_Assignment(&command, Str_Cat(command, target->output_path));
	Str_Assignment(&command, Str_Cat(command, target->output_name));
	Str_Assignment(&command, Str_Cat(command, ".dis"));

	printf("%s\r\n", command);
	//if (system(command) != 0)Error_Handle(ERROR_FATAL_UNKNOWN);
}

/**
* @brief	编译工程
*/
void Item_Target_Build(TargetConfig* target, Group_List* ghead)
{
	Item_Target_GCC(target, ghead);
	Item_Target_LD(target , ghead);
	Item_Target_Bin(target);
	Item_Target_Dump(target);
}


/**
* @brief	下载文件
*/
void Item_Target_Download(TargetConfig* target)
{

}

/**
* @brief	更改输出路径，自动判断路径是否存在
*/
char Item_Change_OPpath(TargetConfig* target, char* path)
{
	if (File_Access(path))
	{
		Str_Assignment(&target->output_path, path);
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
* @brief	更改头文件包含路径
*/
char Item_Change_Ipath(TargetConfig* target, char* path)
{
	char* buf;		//这里要做子文本替换

	buf = path;
	Str_DelEndBlank(buf);		//去除尾部空格
	while (Str_Find(buf , "  " , 0)!=-1)		//把双空格都替换掉
	{
		Str_Assignment(&buf, Str_Replace(buf, "  ", " ", 0, 1));
	}
	if (buf[0] != ' ')
	{
		Str_Assignment(&target->include_path , " ");
	}
	Str_Assignment(&target->include_path, Str_Cat(target->include_path , buf));

	if (buf != path)
	{
		free(buf);
	}
	return 1;
}

/**
* @brief	更改工程名
* @retval	更改成功返回1,否则返回0
*/
char Item_Change_TName(TargetConfig* target, char* name)
{
	if (Str_Verify(name , 0) != -1)		//是不允许有空格的
	{
		return 0;
	}
	Str_Assignment(&target->name , name);
	return 1;
}

/**
* @brief	更改输出文件名
* @retval	更改成功返回1,否则返回0
*/
char Item_Change_OPName(TargetConfig* target, char* name)
{
	if (Str_Verify(name,0) != -1)		//是不允许有空格的
	{
		return 0;
	}
	Str_Assignment(&target->output_name, name);
	return 1;
}


/**
* @brief	更改下载路径
* @retval	更改成功返回1，否则返回0
*/
char Item_Change_Dpath(TargetConfig* target, char* path)
{
	Str_Assignment(&target->download_path, path);
	return 1;
}

/**
* @brief	增加文件
* @param	group		要添加到的组
* @param	filename	文件名
* @retval	增加成功返回1，否则返回0
*/
char Item_File_Add(Group_List* ghead, char* group, char* filename)
{
	Group_List* gbuf;
	File_List* fbuf;
	char** files;
	Str_size_t num;

	/* 先判断组存不存在 */
	gbuf = List_Group_Find(ghead, group);
	if (gbuf == 0)
	{
		return 0;
	}
	
	/* 这里做个循环，空格隔开算做一个文件 */
	files = Str_Cut(filename , " " , &num);
	for (int i = 0; i < num; i++)
	{
		/* 判断是否为空 */
		if (Str_Len(files[i]) == 0)
		{
			continue;
		}

		/* 判断文件路径存在 */
		if (File_Access(files[i]) == 0)
		{
			printf("文件%s不存在\r\n" , files[i]);
			continue;
		}

		/* 判断文件是否已经存在 */
		if (List_File_Find_G(ghead, files[i]) != 0)
		{
			printf("文件%s已经存在\r\n" , files[i]);
			continue;
		}

		/* 增加文件 */
		fbuf = (File_List*)malloc(sizeof(File_List));
		fbuf->Name = 0;
		fbuf->Path = 0;
		Str_Assignment(&fbuf->Name, Str_GetName(files[i]));
		Str_Assignment(&fbuf->Path , files[i]);

		List_File_Add(gbuf, fbuf, ghead);
	}
	return 1;
}

/**
* @brief	删除文件
*/
char Item_File_Del(Group_List* ghead, char* filename)
{
	return List_File_Del_Name(ghead , filename);
}


/**
* @brief	增加组
* @param	groupName	组名
* @retval	增加成功返回1，否则返回0
*/
char Item_Group_Add(Group_List* ghead, char* groupName)
{
	Group_List* gbuf;

	/* 先判断组是不是存在 */
	if (List_Group_Find(ghead , groupName) != 0)
	{
		return 0;
	}
	gbuf = (Group_List*)malloc(sizeof(Group_List));
	gbuf->Name = 0;
	Str_Assignment(&gbuf->Name , groupName);
	gbuf->Files = List_File_Create();
	List_Group_Add(ghead , gbuf);

	return 1;
}

/**
* @brief	删除组
* @retval	删除成功返回1，否则返回0
*/
char Item_Group_Del(Group_List* ghead, char* groupName)
{
	Group_List* gbuf;

	/* 先判断组是不是存在 */
	gbuf = List_Group_Find(ghead, groupName);
	if (gbuf == 0)
	{
		return 0;
	}

	List_Group_Del(gbuf);
	return 1;
}

void Error_Handle(char Type)
{
	switch (Type)
	{
	case ERROR_FATAL_NO_FILE:
		printf("没有输入文件\r\n");
		break;
	case ERROR_FATAL_OPEN_FAIL:
		printf("打开文件失败\r\n");
		break;
	case ERROR_FATAL_READ_FAIL:
		printf("这不是一个工程文件\r\n");
		break;
	case ERROR_FATAL_NO_INPUT:
		printf("没有输入文件\r\n");
		break;
	default:case ERROR_FATAL_UNKNOWN:
		printf("fatal error:产生了未知错误\r\n");
		break;
	}
	system("pause");
	exit(Type);
}


