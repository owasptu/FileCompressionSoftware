#include <W_String.h>

void strreplace(char* str)
{
    char* ch=str;
    while((*ch) !='\0')
    {
        if(*ch=='\\')
            *ch='/';
        ch++;
        //cout<<"j";
    }
}

BOOL WSTR_to_CharStr(PWSTR pszFilePath,char** pfname,int flag)
{
	int szreq=WideCharToMultiByte
            (
            CP_UTF8,
            0,//Dont use this -> WC_NO_BEST_FIT_CHARS,
            pszFilePath,
            -1,/*wcslen(pszFilePath),*///sizeof(pszFilePath),//check if pszFilePath is null terminated
            NULL,
            0,
            NULL,
            NULL
            );
            if(flag)
            	*pfname=(char*)malloc(szreq+30);
            else
            	*pfname=(char*)malloc(szreq+1);
            WideCharToMultiByte
            (
            CP_UTF8,
            0,//WC_NO_BEST_FIT_CHARS,
            pszFilePath,
            -1,//wcslen(pszFilePath),//sizeof(pszFilePath),//check if pszFilePath is null terminated
            *pfname,
            szreq,
            NULL,
            NULL
            );
            strreplace(*pfname);
            //CoTaskMemFree(pszFilePath);
        return 1;
}