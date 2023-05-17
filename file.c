/**********************************************
*   1.先试用File_Verify验证文件有效性
*   2.用File_OpenRead将文件读入到指针中
*   3.调用File_Load函数会自动把工程加载进内存
**********************************************/

/* 头文件包含 */
#include "file.h"
#include "Str_Process.h"
#include "unistd.h"

/* 私有声明，不要外部使用 */
Str_size_t File_Write(char* str, FILE* fp);
Str_size_t File_Write_N(char* str, FILE* fp);
void File_Save_Target(FILE* fp, TargetConfig* target);
void File_Save_Group(FILE* fp, Group_List* ghead);
void File_Save_File(FILE* fp, Group_List* ghead);

/**
* @brief	验证文件有效性，要先把文件全部读入到内存中
* @param	文件内容指针
* @retval	0无效，1有效
*/
char File_Verify(char* fdata)
{
	if (Str_Find(fdata, "<item>", 0) == 0)
	{
		return 1;
	}
	return 0;
}

/**
* @brief	打开文件并读入到内存中，读取完成后会重新打开以写入的方式打开
* @param    filename文件名
* @param	fp保存文件指针
* @param	fdata保存文件数据指针
* @retval	0打开成功，1打开失败
*/
char File_OpenRead(char *filename , FILE **fp , char **fdata)
{
    long long len;
    *fp = fopen(filename, "r");
    if (*fp == 0)return 0;

    fseek(*fp, 0, SEEK_END);
    len = ftell(*fp);                    //获取文件长度
    *fdata = Str_Malloc(len);
    fseek(*fp, 0, SEEK_SET);             //移到文件头
    fread(*fdata, 1, len, *fp);

    fclose(*fp);
    return 1;
}

/**
* @brief    加载工程配置
* @param    target  工程配置指针
* @retval   成功返回1，失败返回0
*/
char File_Load_Target(char* fdata, TargetConfig* target)
{
    char* target_data;  //临时存放，要free
    Str_size_t len, index;

    index = Str_Mate(fdata , "<Target>" , "</Target>" , 0 , &len);
    if (index == -1)return 0;
    target_data = Str_Middle(fdata , index, len);                       //把target部分取出来

    index = Str_Mate(target_data, "<TargetName>", "</TargetName>", 0, &len);  //取工程名字
    if (index == -1)return 0;              
    target->name = Str_Malloc(len);
    target->name = Str_Middle(target_data , index , len);

    index = Str_Mate(target_data, "<IncludePath>", "</IncludePath>", 0, &len);  //取头文件包含路径
    if (index != -1)
    {
        target->include_path = Str_Malloc(len);
        target->include_path = Str_Middle(target_data, index, len);
    }

    index = Str_Mate(target_data, "<OutPutPath>", "</OutPutPath>", 0, &len);  //取输出路径
    if (index != -1)
    {
        target->output_path = Str_Malloc(len);
        target->output_path = Str_Middle(target_data, index, len);
    }

    index = Str_Mate(target_data, "<OutPutFileName>", "</OutPutFileName>", 0, &len);  //取输出文件名
    if (index != -1)
    {
        target->output_name = Str_Malloc(len);
        target->output_name = Str_Middle(target_data, index, len);
    }

    index = Str_Mate(target_data, "<DownloadFile>", "</DownloadFile>", 0, &len);  //取下载路径
    if (index != -1)
    {
        target->download_path = Str_Malloc(len);
        target->download_path = Str_Middle(target_data, index, len);
    }
    
    free(target_data);
    return 1;
}

/**
* @brief    加载文件
* @param    gdata   组文本
* @param    group   文件所在组
*/
void File_Load_File(char* gdata, Group_List* group , Group_List* ghead)
{
    char* buf;                  //暂存buf信息，用完free
    File_List* file_buf;        //要加入的文件
    Str_size_t index=0, len,index_1=0, len_1;   //index给while用，index_1用来寻找

    index = Str_Mate(gdata, "<File>", "</File>", index, &len);
    while (index != -1)
    {
        file_buf = (File_List*)malloc(sizeof(File_List));       //申请内存
        buf = Str_Middle(gdata, index, len);   

        /* 寻找文件名 */
        index_1 = Str_Mate(buf , "<FileName>" , "</FileName>" , 0, &len_1);
        if (index_1 == -1 || len_1 <= 0)
        {
            free(file_buf);
            index = Str_Mate(gdata, "<File>", "</File>", index + len, &len);  //寻找下一个文件
            continue;
        }
        file_buf->Name = Str_Middle(buf , index_1 , len_1);

        /* 寻找文件路径 */
        index_1 = Str_Mate(buf, "<FilePath>", "</FilePath>", 0, &len_1);
        if (index_1 == -1 || len_1 <= 0)
        {
            file_buf->Path = Str_Middle("./" , 0 , 2);
        }
        else
        {
            file_buf->Path = Str_Middle(buf, index_1, len_1);
        }

        /* 将文件添加到链表 */
        if (!List_File_Add(group , file_buf , ghead))           //添加失败，应该是文件已存在，释放空间
        {
            free(file_buf->Name);
            free(file_buf->Path);
            free(file_buf);
        }

        index = Str_Mate(gdata, "<File>", "</File>", index+len, &len);  //寻找下一个文件
    }
}

/**
* @brief    加载组
*/
void File_Load_Group(char* fdata, Group_List* ghead)
{
    char* buf=0;                  //暂存group信息，用完free
    Group_List* group_buf;      //要加入的组
    Str_size_t index=0, len, index_1, len_1;  //index给while用，index_1给while里面的用

    index = Str_Mate(fdata, "<Group>" , "</Group>" , index , &len);
    while (index != -1)
    {
        group_buf = (Group_List*)malloc(sizeof(Group_List));
        buf = Str_Middle(fdata , index , len);
        index_1 = Str_Mate(buf , "<GroupName>" , "</GroupName>" , 0 , &len_1);
        if (index_1 == -1)
        {
            free(group_buf);    //组名都没有，怕是有问题
            continue;
        }
        group_buf->Name = Str_Middle(buf , index_1 , len_1);
        group_buf->Files = List_File_Create();              //要创建file_list，否则就gg了
        File_Load_File(buf , group_buf , ghead);            //将文件加载到链表里

        List_Group_Add(ghead, group_buf);
        index = Str_Mate(fdata, "<Group>", "</Group>", index+len, &len);        //搜寻下一个组
    }

    if (buf != 0)
    {
        free(buf);
    }
    
}

/**
* @brief    工程加载，先调用File_OpenRead读入   
*/
char File_Load(char* fdata , TargetConfig *target , Group_List *ghead)
{
    if (!File_Verify(fdata))return 0;
    if(!File_Load_Target(fdata , target))return 0;
    File_Load_Group(fdata , ghead);
    free(fdata);
    return 1;
}


/**
* @brief    向文件中写入字符串，不带换行符
*/
Str_size_t File_Write(char* str, FILE* fp)
{
    return fwrite(str, 1, Str_Len(str) , fp);
}

Str_size_t File_Write_N(char* str, FILE* fp)
{
    return File_Write(str , fp)+ File_Write("\n" , fp);
}

/**
* @brief    将工程文件保存回文件中
*/
void File_Save(char *filename , TargetConfig*target , Group_List*ghead)
{
    FILE* fp;
    fp = fopen(filename, "w");
    if (fp == 0)return;
    

    fseek(fp , 0 , SEEK_SET);       //移到文件头
    File_Write_N("<item>" , fp);

    File_Save_Target(fp , target);
    File_Save_Group(fp , ghead);

    File_Write_N("</item>", fp);

    fclose(fp);
}

/**
* @brief    保存工程配置部分
*/
void File_Save_Target(FILE* fp, TargetConfig* target)
{
    File_Write_N("\t<Target>", fp);

    File_Write("\t\t<TargetName>", fp);
    File_Write(target->name, fp);
    File_Write_N("</TargetName>", fp);

    File_Write("\t\t<IncludePath>", fp);
    File_Write(target->include_path, fp);
    File_Write_N("</IncludePath>", fp);

    File_Write("\t\t<OutPutPath>", fp);
    File_Write(target->output_path, fp);
    File_Write_N("</OutPutPath>", fp);

    File_Write("\t\t<OutPutFileName>", fp);
    File_Write(target->output_name, fp);
    File_Write_N("</OutPutFileName>", fp);

    File_Write("\t\t<DownloadFile>", fp);
    File_Write(target->download_path, fp);
    File_Write_N("</DownloadFile>", fp);

    File_Write_N("\t</Target>", fp);
}

/**
* @brief    保存组
* @param    ghead   组链表头
*/
void File_Save_Group(FILE* fp, Group_List* ghead)
{
    Group_List* buf;

    File_Write_N("\t<Groups>", fp);

    buf = ghead->Next;
    while (buf != ghead)
    {
        File_Write_N("\t\t<Group>", fp);
        File_Write("\t\t\t<GroupName>", fp);
        File_Write(buf->Name, fp);
        File_Write_N("</GroupName>", fp);

        File_Save_File(fp, buf);        //保存组里的所有文件

        File_Write_N("\t\t</Group>", fp);
        buf = buf->Next;
    }

    File_Write_N("\t</Groups>", fp);
}

/**
* @brief    保存文件
* @brief    这里把要保存的组传进来就行
*/
void File_Save_File(FILE* fp, Group_List* group)
{
    File_List* buf;
    buf = group->Files->Next;
    while (buf != group->Files)
    {
        File_Write_N("\t\t\t<File>", fp);

        File_Write("\t\t\t\t<FileName>", fp);   //文件名
        File_Write(buf->Name, fp);
        File_Write_N("</FileName>", fp);

        File_Write("\t\t\t\t<FilePath>", fp);   //文件路径
        File_Write(buf->Path, fp);
        File_Write_N("</FilePath>", fp);

        File_Write_N("\t\t\t</File>", fp);
        buf = buf->Next;
    }
}

/**
* @brief    验证文件存在
* @retval   存在返回1，不存在返回0
*/
char File_Access(char* filename)
{
    if (access(filename, 0) != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}