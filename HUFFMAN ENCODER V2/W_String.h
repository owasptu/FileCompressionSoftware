#include <windows.h>
#ifndef W_String_H_
#define W_String_H_

void strreplace(char* str);
BOOL WSTR_to_CharStr(PWSTR pszFilePath,char** pfname,int flag);

#endif