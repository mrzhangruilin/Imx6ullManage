#ifndef _TEXT_H__
#define _TEXT_H__

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long Str_size_t;

Str_size_t Str_Len(char *str);
void Str_Assignment(char** str1, char* str2);
char* Str_Malloc(Str_size_t len);
void Str_Clean(char* str, Str_size_t len);
char* Str_Cat(char* str1, char* str2);
char Str_Compare(char* str1, char* str2);
char* Str_Left(char* str, Str_size_t len);
Str_size_t Str_Find(char* str, char* strFind, Str_size_t begin);
char* Str_Middle(char* str, Str_size_t index, Str_size_t len);
char* Str_Right(char* str, Str_size_t len);
char* Str_Replace(char* str, char* strO, char* strN, Str_size_t begin, Str_size_t num);
Str_size_t Str_Mate(char* str, char* matel, char* mater, Str_size_t begin, Str_size_t* len);
Str_size_t Str_FindFinal(char* str, char* strFind);
char* Str_GetName(char* filename);
char* Str_GetPath(char* filename);
char* Str_DelEndBlank(char* str);
char** Str_Cut(char* text, char* str, Str_size_t* num);
Str_size_t Str_Verify(char* str, char* check);
char* Str_GetLine(void);
#endif // !_TEXT_H__
