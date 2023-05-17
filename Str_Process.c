#include "Str_Process.h"



/**
* @brief	检测文本长度
*/
Str_size_t Str_Len(char* str)
{
	Str_size_t i;
	if (str == 0)return 0;
	for (i = 0; str[i] != 0; i++);
	return i;
}

/*
* @brief	给文本赋值
* 将t2的值赋给t1
*/
void Str_Assignment(char** str1, char *str2)
{
	if (*str1 != NULL)free(*str1);
	*str1 = (char*)malloc(Str_Len(str2)+1);
	for (Str_size_t i = 0; i < Str_Len(str2) + 1; i++)(*str1)[i] = 0;
	for (Str_size_t i = 0; str2[i] != 0; i++)
	{
		(*str1)[i] = str2[i];
	}
}


/**
 * @brief   清空字符串，初始化字符串
 * @param   str 字符串指针
 * @param   len 字符串长度
*/
void Str_Clean(char* str, Str_size_t len)
{
	for (Str_size_t i = 0; i < len; i++)
	{
		str[i] = 0;
	}
}

/**
* @brief    申请内存，并清空内存
* @param    申请长度，申请时会自动+1
* @retval   空字符串指针
*/
char* Str_Malloc(Str_size_t len)
{
	char* buf;
	buf = (char*)malloc(len + 1);
	Str_Clean(buf, len + 1);
	return buf;
}

/**
* @brief	连接两个字符串
*/
char* Str_Cat(char* str1, char* str2)
{
	char* res = Str_Malloc(Str_Len(str1)+Str_Len(str2));
	Str_size_t i,j;
	for (i = 0; str1!=NULL&&str1[i] != 0; i++)
	{
		res[i] = str1[i];
	}
	for (j = 0; str2!=NULL&&str2[j] != 0; j++,i++)
	{
		res[i] = str2[j];
	}
	return res;
}

/**
* @brief    比较字符串
* @param    str1
* @param    str2
* @retval   0-字符串不一样，1-字符串一样
*/
char Str_Compare(char* str1, char* str2)
{
	if (Str_Len(str1) != Str_Len(str2))return 0;
	for (Str_size_t i = 0; str1!=NULL&&str2!=NULL&&str1[i] != 0; i++)
	{
		if (str1[i] != str2[i])return 0;
	}
	return 1;
}

/**
 * @brief   取字符串左边
 * @param   str 字符串
 * @param   len 字符串长度
 * @retval  返回指针，
*/
char* Str_Left(char* str, Str_size_t len)
{
	char* res;

	res = Str_Malloc(Str_Len(str));

	if (str == NULL)return 0;
	for (Str_size_t i = 0;(str[i]!=0)&&(i<len); i++)
	{
		res[i] = str[i];
	}

	return res;
}

/**
 * @brief   寻找字符串
 * @param   str     被搜寻的字符串
 * @param   strFind 要搜寻的字符串
 * @param   begin   起始搜寻位置
 * @retval  返回找到的位置
*/
Str_size_t Str_Find(char* str, char* strFind, Str_size_t begin)
{
	Str_size_t len;
	Str_size_t res = -1;

	len = Str_Len(strFind);
	if (len == 0 || Str_Len(str)==0)return -1;
	for (Str_size_t i = begin; str[i] != 0; i++)
	{
		if (str[i] == strFind[0])
		{
			res = i;
			for (Str_size_t j = 0; j < len && res != -1; j++)
			{
				if (str[j + i] != strFind[j])
				{
					res = -1;
				}
			}
			if (res != -1)return res;
		}
	}
	return -1;
}


/**
* @brief    取文本中间，malloc申请的内存
* @param    str
* @param    index   开始位置
* @param    len     取出长度
* @retval   字符串指针
*/
char* Str_Middle(char* str, Str_size_t index, Str_size_t len)
{
	char* res;
	res = Str_Malloc(len + 1);
	if (Str_Len(str) == 0)return res;
	for (Str_size_t i = 0; i < len; i++)
	{
		res[i] = str[index + i];
	}
	return res;
}

/**
 * @brief   取字符串右边
 * @param   str 字符串
 * @param   len 取出长度
*/
char* Str_Right(char* str, Str_size_t len)
{
	char* res;
	Str_size_t total;

	total = Str_Len(str);
	if (total <= len || len <= 0)return 0;

	total -= len;       //复用存偏移量
	
	res = Str_Malloc(len+1);


	for (Str_size_t i = 0; i < len; i++)
	{
		res[i] = str[i + total];
	}
	res[len] = 0;

	return res;
}

/**
 * @brief   子文本替换
 * @param   str     文本
 * @param   strO    要被替换的子文本
 * @param   strN    用来替换的子文本
 * @param   begin   起始位置
 * @param   num     替换次数，为0时或者大于子文本出现次数时则全部替换
 * @retval  返回malloc申请的字符串指针
*/
char* Str_Replace(char* str, char* strO, char* strN, Str_size_t begin, Str_size_t num)
{
	Str_size_t index;
	Str_size_t i = 0;                //已经替换的次数
	char* res =0;
	char* buf =0;
	char* right;            //malloc申请，要释放

	index = Str_Find(str, strO, begin);
	Str_Assignment(&buf , str);
	while (index != -1 && (i < num || num == 0))
	{
		if (index != 0)
		{
			Str_Assignment(&res, Str_Left(buf , index));					//前半部分           
		}
		Str_Assignment(&res , Str_Cat(res , strN));
		right = Str_Right(buf, Str_Len(buf) - Str_Len(strO) - index);  //右边部分
		if (right != 0)      //空指针
		{
			Str_Assignment(&res, Str_Cat(res, right));
		}

		Str_Assignment(&buf, res);
		
		index = Str_Find(buf, strO, index + Str_Len(strN));
		i++;
	}
	free(buf);
	return res;
}

/**
* @brief    匹配文本
* @param    str     文本
* @param    matel   左文本
* @param    mater   右文本
* @param    begin   起始匹配位置
* @param    len     保存匹配到的长度
* @retval   返回匹配到的位置
*/
Str_size_t Str_Mate(char* str, char* matel, char* mater, Str_size_t begin, Str_size_t* len)
{
	Str_size_t indexl, indexr;

	indexl = Str_Find(str, matel, begin);
	indexr = Str_Find(str, mater, indexl + Str_Len(matel));
	if (indexl != -1 && indexr != -1)
	{
		*len = indexr - indexl - Str_Len(matel);
		return indexl + Str_Len(matel);
	}
	return -1;
}


/**
* @brief    找到字符串出现的最后位置
* @param    str
* @param    strFind
* @retval   返回最后出现的位置，没出现则返回-1
*/
Str_size_t Str_FindFinal(char* str, char* strFind)
{
	Str_size_t res;
	Str_size_t final = -1;

	res = Str_Find(str, strFind, 0);
	while (res != -1)
	{
		final = res;
		res = Str_Find(str, strFind, res + Str_Len(strFind));
	}
	return final;
}

/**
* @brief    取路径中的文件名
* @param    filename    带路径文件名
* @retval   返回文件名
*/
char* Str_GetName(char* filename)
{
	Str_size_t index;
	char* name=0;

	index = Str_FindFinal(filename, "/");
	if (index == -1)
	{
		index = Str_FindFinal(filename, "/");
		if (index == -1)
		{
			Str_Assignment(&name , filename);
			return name;
		}
	}
	Str_Assignment(&name, filename+index+1);
	return name;
}

/**
* @brief    取文件的路径
* @param    filename    带路径文件名
* @retval   返回路径
*/
char* Str_GetPath(char* filename)
{
	Str_size_t index;
	char* path = 0;

	index = Str_FindFinal(filename, "/");
	if (index == -1)
	{
		index = Str_FindFinal(filename, "/");
		if (index == -1)
		{
			Str_Assignment(&path , "./");
			return path;
		}
	}
	path = Str_Middle(filename , 0 , index+1);
	return path;
}

/**
* @brief    去除尾部空格
*/
char* Str_DelEndBlank(char* str)
{
	Str_size_t i = Str_Len(str);
	for (i--; i >= 0 && str[i] == ' '; i--)
	{
		str[i] = 0;
	}
	return str;
}


/**
* @brief    分割子文本
* @param    text    要被分割的文本
* @param    str     用来分割的文本
* @param    num     返回分割数量
* @retval   reText  返回的字符串指针数组
*/
char** Str_Cut(char* text, char* str, Str_size_t* num)
{
	char** res=0;        //指针数组
	Str_size_t index;         //头部索引
	Str_size_t len;           //复用，str长度，以及匹配到的文本长度
	char* buf = 0;

	Str_Assignment(&buf, text);

	/* 文本尾没有str */
	index = Str_FindFinal(buf, str);
	if (index + Str_Len(str) != Str_Len(buf))
	{
		Str_Assignment(&buf , Str_Cat(buf , str));//在末尾放一个str
	}
	/* 文本头没有str */
	index = Str_Find(buf, str, 0);
	if (index != 0)
	{
		Str_Assignment(&buf, Str_Cat(str, buf));//在头部放一个str
	}

	*num = 0;
	index = 0;
	len = Str_Len(str);
	index = Str_Find(buf, str, index);
	while (index != -1)
	{
		if (index != 0)
		{
			(*num)++;
		}
		index = Str_Find(buf, str, index + len);
	}


	res = (char**)malloc((*num) * sizeof(char*));       //申请内存
	*num = 0;                                            //由于是复用，这一步一定要在申请内存之后
	index = 0;
	index = Str_Mate(buf, str, str, index, &len);
	while (index != -1)
	{
		res[*num] = Str_Middle(buf, index, len);
		(*num)++;
		index = Str_Mate(buf, str, str, index + len, &len);
	}

	return res;
}

/**
* @brief    检查违规字符
* @param	check	违规字符串，可以填" /\;:?<>|&"
* @retval   有违规字符返回索引，无返回-1
*/
Str_size_t Str_Verify(char* str , char *check)
{
	Str_size_t len, i,index;
	char* buf = 0;

	if (check == NULL)check = " /\\;:?<>|&";
	len = Str_Len(check);
	for (i = 0; i < len; i++)
	{
		Str_Assignment(&buf , Str_Middle(check , i , 1));
		index = Str_Find(str, buf, 0);
		if (index != -1)
		{
			return index;
		}
	}
	return -1;
}

/**
* @brief    从控制台获取一行字符串，上限200个字符
* @retval   返回字符串指针
*/
char* Str_GetLine()
{
	char* buf;
	int i = 0;
	buf = Str_Malloc(200);
	buf[i] = getchar();
	while (buf[i] != '\n' && i < 200)
	{
		i++;
		buf[i] = getchar();
	}
	buf[i] = 0;

	return buf;
}
