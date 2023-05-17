#include "display.h"
#include "list.h"


#include <stdio.h>

/**
* @brief	输出工程信息
* @param	target	工程配置指针
*/
void Display_Target_List(TargetConfig* target)
{
	printf("工程名：%s\r\n", target->name);
	printf("头文件包含路径：%s\r\n", target->include_path);
	printf("输出路径：%s\r\n", target->output_path);
	printf("输出文件名：%s\r\n", target->output_name);
	printf("下载路径：%s\r\n", target->download_path);
}

/*清屏*/
void Display_Clear()
{
	system("cls");
}

/**
* @brief	主界面显示
*/
void Display_Main()
{
	printf("******************************************\r\n");
	printf("\t<工程管理>\r\n");
	printf("输入字母选择操作（大小写均可）：\r\n");
	printf("\tB.编译\r\n");
	printf("\tD.下载\r\n");
	printf("\tF.文件操作\r\n");
	printf("\tG.组操作\r\n");
	printf("\tT.工程配置\r\n");
	printf("\tQ.退出程序\r\n");
	printf("******************************************\r\n");
}

/**
* @brief	工程配置界面显示
*/
void Display_Target()
{
	printf("******************************************\r\n");
	printf("\t<工程配置>\r\n");
	printf("\tB.更改输出路径\r\n");
	printf("\tC.清屏\r\n");
	printf("\tI.头文件路径更改\r\n");
	printf("\tL.显示所有工程配置\r\n");
	printf("\tN.更改工程名\r\n");
	printf("\tO.更改输出文件名\r\n");
	printf("\tQ.退出工程配置\r\n");
	printf("******************************************\r\n");
}

/**
* @brief	文件管理界面展示
*/
void Display_File()
{
	printf("******************************************\r\n");
	printf("\t<文件管理>\r\n");
	printf("\tA.增加文件\r\n");
	printf("\tC.清屏\r\n");
	printf("\tD.删除文件\r\n");
	printf("\tL.显示所有文件\r\n");
	printf("\tQ.退出文件管理\r\n");
	printf("******************************************\r\n");
}

/**
* @brief	文件管理界面展示
*/
void Display_Group()
{
	printf("******************************************\r\n");
	printf("\t<组管理>\r\n");
	printf("\tA.增加组\r\n");
	printf("\tC.清屏\r\n");
	printf("\tD.删除组\r\n");
	printf("\tL.显示所有组\r\n");
	printf("\tQ.退出组管理\r\n");
	printf("******************************************\r\n");
}