//头文件
#include "manage.h"

//私有函数声明
char Initlization(char* filename);
void Item_Target(void);
void Item_File(void);
void Item_Group(void);


//全局变量表
FILE* g_fp;
char* g_fdata;

Group_List* g_groups;
TargetConfig g_target;


int main(int argv , char *argc[])
{   
#if 1
    char* option;
    if (argv <= 1)
    {
        Error_Handle(ERROR_FATAL_NO_INPUT);
    }
    else
    {
        if (Initlization(argc[1]) == 0)Error_Handle(ERROR_FATAL_OPEN_FAIL);
    }
    
    
    Display_Clear();
    Display_Main();
    while (1)
    {
        option = Str_GetLine();
        /* 编译 */
        if (option[0] == 'b' || option[0] == 'B')
        {
            Item_Target_Build(&g_target , g_groups);
        }

        /* 进入工程管理 */
        if (option[0] == 't' || option[0] == 'T')
        {
            Item_Target();
            Display_Clear();
            Display_Main();
        }

        /* 文件管理 */
        if (option[0] == 'f' || option[0] == 'F')
        {
            Item_File();
            Display_Clear();
            Display_Main();
        }

        /* 组管理 */
        if (option[0] == 'g' || option[0] == 'G')
        {
            Item_Group();
            Display_Clear();
            Display_Main();
        }

        /* 清屏 */
        if (option[0] == 'c' || option[0] == 'C')
        {
            Display_Clear();
            Display_Main();
        }


        /* 退出程序 */
        if (option[0] == 'q' || option[0] == 'Q')
        {
            break;
        }
    }

    File_Save("test.i", &g_target, g_groups);                   //程序结束保存文件
#endif // 1

    //system("pause");                //windows下调试一闪而过
    return 0;
}

/**
* @brief    系统初始化和加载
*/
char Initlization(char* filename)
{
    g_groups = List_Group_Create();

    if (!File_OpenRead(filename, &g_fp, &g_fdata))
    {
        Error_Handle(ERROR_FATAL_OPEN_FAIL);
        return 0;
    }
    if (!File_Load(g_fdata, &g_target, g_groups))
    {
        Error_Handle(ERROR_FATAL_READ_FAIL);
        return 0;
    }
    return 1;
}

/**
* @brief    工程配置程序
* B.更改输出路径  D.更改下载路径    I.头文件路径更改   L.显示所有工程配置  N.更改工程名 O.更改输出文件名
*/
void Item_Target(void)
{
    char* option;
    char* buf;      //复用

    Display_Clear();
    Display_Target();
    while (1)
    {
        option = Str_GetLine();

        /* 更改输出路径 */
        if (option[0] == 'b' || option[0] == 'B')        
        {
            printf("原输出路径：%s\r\n" , g_target.output_path);
            printf("请输入新的路径：");
            buf = Str_GetLine();
            if (Item_Change_OPpath(&g_target, buf))
            {
                printf("更改成功\r\n");
            }
            else
            {
                printf("更改失败\r\n");
            }
            free(buf);
        }

        /* 更改头文件包含路径 */
        if (option[0] == 'i' || option[0] == 'I')
        {
            printf("原头文件包含路径：%s\r\n", g_target.include_path);
            printf("请输入新的路径（每个路径之间空格隔开）：");
            buf = Str_GetLine();
            Item_Change_Ipath(&g_target , buf);
            printf("更改成功\r\n");
            free(buf);
        }

        /* 更改工程名 */
        if (option[0] == 'n' || option[0] == 'N')
        {
            printf("原工程名：%s\r\n", g_target.name);
            printf("新工程名（不带空格，不能有特殊符号:<>?;）：");
            buf = Str_GetLine();
            if (Item_Change_TName(&g_target, buf))
            {
                printf("更改成功\r\n");
            }
            else
            {
                printf("更改失败，名字不能带特殊符号\r\n");
            }
            free(buf);
        }

        /* 更改输出文件名 */
        if (option[0] == 'o' || option[0] == 'O')
        {
            printf("原工程名：%s\r\n", g_target.output_name);
            printf("新工程名（不带空格，不能有特殊符号:<>?;）：");
            buf = Str_GetLine();
            if (Item_Change_OPName(&g_target, buf))
            {
                printf("更改成功\r\n");
            }
            else
            {
                printf("更改失败，名字不能带特殊符号\r\n");
            }
            free(buf);
        }

        /* 更改下载路径 */
        if (option[0] == 'd' || option[0] == 'D')
        {
            printf("原下载路径：%s\r\n", g_target.download_path);
            printf("新下载路径（不能带空格）：");
            buf = Str_GetLine();
            if (Item_Change_Dpath(&g_target, buf))
            {
                printf("更改成功\r\n");
            }
            else
            {
                printf("未知错误，更改失败\r\n");
            }
            free(buf);
        }

        /* 列出工程配置 */
        if (option[0] == 'l' || option[0] == 'L')       
        {
            Display_Target_List(&g_target);
        }
           
        /* 清屏 */
        if (option[0] == 'c' || option[0] == 'C')       
        {
            Display_Clear();
            Display_Target();
        }

        /* 退出 */
        if (option[0] == 'q' || option[0] == 'Q')      
        {
            break;
        }

        free(option);
    }
}

/**
* @brief    文件管理
*/
void Item_File(void)
{
    char* option;
    char* gbuf;      
    char* fbuf;  //复用

    Display_Clear();
    Display_File();
    while (1)
    {
        option = Str_GetLine();

        /* 增加文件 */
        if (option[0] == 'a' || option[0] == 'A')
        {
            List_Group_print(g_groups);         //打印所有组
            printf("请输入要添加到的组：");
            gbuf = Str_GetLine();
            printf("请输入要添加的文件名（带路径）：");
            fbuf = Str_GetLine();
            if (Item_File_Add(g_groups, gbuf, fbuf))
            {
                printf("增加成功\r\n");
            }
            else
            {
                printf("增加失败：文件不存在或组不存在\r\n");
            }
        }

        /* 删除文件 */
        if (option[0] == 'd' || option[0] == 'D')
        {
            printf("请输入要删除的文件：");
            fbuf = Str_GetLine();
            if (Item_File_Del(g_groups, fbuf))
            {
                printf("删除成功\r\n");
            }
            else
            {
                printf("删除失败：文件不存在\r\n");
            }
        }

        /* 列出所有文件 */
        if (option[0] == 'l' || option[0] == 'L')
        {
            List_File_print(g_groups);
        }

        /* 清屏 */
        if (option[0] == 'c' || option[0] == 'C')
        {
            Display_Clear();
            Display_File();
        }

        /* 退出 */
        if (option[0] == 'q' || option[0] == 'Q')
        {
            break;
        }

        free(option);
    }
}

/**
* @brief    组管理
*/
void Item_Group(void)
{
    char* option;
    char* gbuf;

    Display_Clear();
    Display_Group();
    while (1)
    {
        option = Str_GetLine();

        /* 增加组 */
        if (option[0] == 'a' || option[0] == 'A')
        {
            List_Group_print(g_groups);         //打印所有组
            printf("请输入要添加的组名：");
            gbuf = Str_GetLine();
            if (Item_Group_Add(g_groups , gbuf))
            {
                printf("增加成功\r\n");
            }
            else
            {
                printf("增加失败：组已存在\r\n");
            }
        }

        /* 删除文件 */
        if (option[0] == 'd' || option[0] == 'D')
        {
            List_Group_print(g_groups);
            printf("请输入要删除的组：");
            gbuf = Str_GetLine();
            if (Item_Group_Del(g_groups, gbuf))
            {
                printf("删除成功\r\n");
            }
            else
            {
                printf("删除失败：组不存在\r\n");
            }
        }

        /* 列出所有组 */
        if (option[0] == 'l' || option[0] == 'L')
        {
            List_Group_print(g_groups);
        }

        /* 清屏 */
        if (option[0] == 'c' || option[0] == 'C')
        {
            Display_Clear();
            Display_Group();
        }

        /* 退出 */
        if (option[0] == 'q' || option[0] == 'Q')
        {
            break;
        }

        free(option);
    }
}