#include <File_Handler.h>
#include <wchar.h>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <W_String.h>

map<char,node*>& File_Handler::readInputfromFile(map<char,node*>& letterFreqMap) 
{
	cout<<"entering readInputfromFile"<<endl;
	ifstream instream(ipfpath);
	if (!instream)
	{
		MessageBox(NULL, L"unable to open input file", L"File Path", MB_OK);//
		return letterFreqMap ;
	}
	//ipfpath=fname;
	//ipfpath=(char*)malloc(strlen(fname)+1);
	//strcpy(ipfpath,fname);
	char inchar ;cout<<"read chars are: ";
	while(instream>> inchar)
	{
		cout<<inchar<<" ";
		int x=inchar ;
		if( !isalnum( x ) )
			continue ;
		if( ( x >= 65)&&(x <= 90) )
			inchar = 97 + (x - 65) ;

		//letterCodeMap[inchar] = -1; no letterCodeMap in this class
		map<char,node*>::iterator m_iter;
		m_iter = letterFreqMap.find(inchar) ;

		if( m_iter != letterFreqMap.end() )
		{
			(letterFreqMap[inchar]->freqOfAlphabet)++ ;
		}
		else
		{
			letterFreqMap.insert( pair< char,node* > ( inchar, createTreeNode(1, NULL, NULL, inchar) ) );
		}
	}
	instream.close();
	cout<<"returning form readInputfromFile"<<endl ;
	return letterFreqMap ;
}

void File_Handler::serializetree(node* root,ofstream& fout)
{
	if(!root)
	{
		fout.put('+');
		return;
	}
	fout.put(root->alphabet);
	serializetree(root->left,fout);
	serializetree(root->right,fout);
}

bool File_Handler::serializetree_wrapper(node* root)
{
	ofstream fout(treePath);
	serializetree(root,fout);//fout.write(fname,5);
	fout.close();
	return 1;
}

char* File_Handler::createHEdir()
{
	PWSTR pathToDocuments;
	HRESULT y= SHGetKnownFolderPath(
		FOLDERID_Documents,
		0,
		NULL,
		&pathToDocuments
		);
	if(y!=S_OK)
		MessageBox(NULL, L"UNABLE TO GET DOCUMENTS DIRECTORY", L"File Path", MB_OK);	
	//copy oppath to fullpath ,oppath allocated inside function
	//std::wstring wstr_hedir=*opPath + L"\\Compressor Files" ;
	int len_of_pathToDocuments=wcslen(pathToDocuments);
	wchar_t *opPathFull = new wchar_t[50 + len_of_pathToDocuments];
	//alternative use cotaskmemalloc for pwstr buffer
	wcsncpy(opPathFull,pathToDocuments, len_of_pathToDocuments+1);
	CoTaskMemFree(pathToDocuments);
	//opPathFull[len_of_pathToDocuments]=L"\0";
	//concat opPathFull
	wcsncat(opPathFull,L"\\Compressor Files",17);
	BOOL rs=CreateDirectory(opPathFull,NULL);
	if(rs==FALSE)
	{
		DWORD ec=GetLastError();
		if(ec!=ERROR_ALREADY_EXISTS)
			MessageBox(NULL, /*L"UNABLE TO CREATE HUFFMAN ENCODER DIRECTORY"*/opPathFull, L"File Path", MB_OK);
	}
	wcsncat(opPathFull,L"\\",1);
	WSTR_to_CharStr(opPathFull,&hedir,1);
	hedir_size=strlen(hedir);
	delete [] opPathFull;
	return hedir;
}

void File_Handler::set_tree_path()
{
	
	if(hedir_size==-1)
		MessageBox(NULL, L"hedir_size is -1 in set_tree_path", L"File Path", MB_OK);
	treePath=(char*)malloc(hedir_size+60);
	if(!treePath)
		MessageBox(NULL, L"UNABLE TO ALLOCATE MEMORY TO treePath in set_tree_path ", L"File Path", MB_OK);
	strncpy(treePath,hedir,hedir_size+1);
	strncat(treePath,ipUniqueFileName,40);
	strncat(treePath,"_savedtree",10);

}

void File_Handler::set_compfile_path()
{
	if(hedir_size==-1)
		MessageBox(NULL, L"hedir_size is -1 in set_compfile_path", L"File Path", MB_OK);
	//fname=(char*)malloc(hedir_size+30);
	compressedFilePath=(char*)malloc(hedir_size+60);
	if(!compressedFilePath)
		MessageBox(NULL, L"UNABLE TO ALLOCATE MEMORY TO fname in set_compfile_path ", L"File Path", MB_OK);
	strncpy(compressedFilePath,hedir,hedir_size+1);
	strncat(compressedFilePath,ipUniqueFileName,40);
	strncat(compressedFilePath,"_compressed",11);
}

void File_Handler::set_compfile_path_toInputLoc()
{
	int lastCopyOfIpfpath_i=0;
	while(1)
	{
		if(ipfpath[lastCopyOfIpfpath_i] =='\0')
			break;
		if(ipfpath[lastCopyOfIpfpath_i] =='.')
			break;
		lastCopyOfIpfpath_i++;
	}
	compressedFilePath=(char*)malloc(strlen(ipfpath)+30);
	strncpy(compressedFilePath,ipfpath,lastCopyOfIpfpath_i);
	compressedFilePath[lastCopyOfIpfpath_i]='\0';
	strncat(compressedFilePath,"__compressed",12);
}

bool File_Handler::DeleteInputFile()
{

}


bool File_Handler::makeCompressedFile(map<char,int>& letter_prefix_0bit_len,map< char,int >& letterCodeMap)
{
	//set_compfile_path();
	ofstream outstream;
	outstream.open(compressedFilePath,ios::binary);
	if(!outstream)
	{
		MessageBox(NULL, L"UNABLE TO CREATE COMPRESSED FILE", L"File Path", MB_OK);
		return 0;
	}
	ifstream instream(ipfpath);
	if(!instream)
			MessageBox(NULL, L"UNABLE TO OPEN INPUT FILE IN makeCompressedFile", L"File Path", MB_OK);

	cout<<"ip and op file stream opened"<<endl;//MessageBox(NULL, L"inside output", L"File Path", MB_OK);
	char inchar;

	int letterCode,zerobitlen,currbit=1,bitsize,bit,div=0;
	unsigned char byte=0x00;cout<<"file read loop entering";
	while(instream>> inchar)
	{
		//add logic for reading space
		cout<<"char read "<<inchar<<endl;
		int x=inchar ;
		if( !isalnum( x ) )
		{	
			continue ;
		}
		if( ( x >= 65)&&(x <= 90) )
			inchar = 97 + (x - 65) ;

		letterCode=letterCodeMap[inchar];cout<<"letterCode: "<<letterCode<<endl;
		zerobitlen=letter_prefix_0bit_len[inchar];cout<<"zerobitlen: "<<zerobitlen<<endl;

		while(zerobitlen--)
		{
			if(currbit==9)
			{
				outstream.put(byte);//outstream<<byte;
				currbit=1;
			}
			byte=byte<<1;byte=byte & 0xfe;cout<<"0";
			currbit++;
		}
		if(!letterCode)
		{
			//byte=byte<<1;byte=byte & 0xfe;currbit++;
			cout<<"0";div=0;
		}
		else
		{
			bitsize=1;
			while(pow(2,bitsize)<=letterCode)
				bitsize++;
			bitsize--;
			div=pow(2,bitsize);
		}
		while(div)
		{
			if(currbit==9)
			{
				outstream.put(byte);
				cout<<(int)byte<<" ";
				byte=byte>>8;
				currbit=1;
			}

			bit=letterCode/div;
			byte=byte<<1;
			if(bit)
				byte=byte|1;
			else
				byte=byte & 0xfe;
			
			
			currbit++;
			letterCode=letterCode%div;
			div=div/2;


			// if(currbit==9)
			// {
			// 	outstream.put(byte);//outstream<<byte;
			// 	currbit=1;
			// }
			// if (letterCode%2)
			// {
			// 	byte=byte>>1;cout<<"1";
			// 	byte=byte|128;
			// }
			// else
			// {
			// 	byte=byte>>1;cout<<"0";
			// }
			
			// currbit++;
			// letterCode/=2;
		}
		
		//cout<<endl;
	}
	if (currbit!=9)
		byte=byte<<(9-currbit);cout<<"unwritten bits left: "<<(9-currbit);
	
	outstream.put(byte);//outstream<<byte;
	outstream.close();
	instream.close();
	cout<<"returning makecompressed file";
	return 1;
}

int File_Handler::generateFileId()
{
	return 0;
}


void File_Handler::SetipUniqueFileName()
{
	// add underscore at beginning
	ipUniqueFileName=(char*)malloc(30);
	int lastSlash_i=0,end_i=0,i=0;
	// end_i is index of last dot character or ending Null character
	// lastSlash_i is index of last forward or backward slash
	while(ipfpath[i]!='\0')
	{
		if((ipfpath[i]=='\\')||(ipfpath[i]=='/'))
			lastSlash_i=i;
		if(ipfpath[i]=='.')
			end_i=i;
		i++;
	}
	if(end_i == 0)
		end_i = i;
	if(lastSlash_i == 0)
		strncpy(ipUniqueFileName,"_UniqueName",12);
	else
	{
		strncpy(ipUniqueFileName,ipfpath+lastSlash_i + 1,end_i - lastSlash_i - 1);
		ipUniqueFileName[end_i - lastSlash_i -1]='\0';
	}
	return;
}
	
// 	int letterCode,zerobitlen,currbit=1;
// 	unsigned char byte='a';cout<<"file read loop entering";
// 	while(instream>> inchar)
// 	{
// 		//add logic for reading space
// 		cout<<"char read "<<inchar<<endl;
// 		int x=inchar ;
// 		if( !isalnum( x ) )
// 		{	
// 			continue ;
// 		}
// 		if( ( x >= 65)&&(x <= 90) )
// 			inchar = 97 + (x - 65) ;

// 		letterCode=letterCodeMap[inchar];cout<<"letterCode: "<<letterCode<<endl;
// 		zerobitlen=letter_prefix_0bit_len[inchar];cout<<"zerobitlen: "<<zerobitlen<<endl;

		
// 		if(!letterCode)
// 		{
// 			byte=byte>>1;currbit++;
// 			cout<<"0";
// 		}
// 		while(letterCode)
// 		{
// 			if(currbit==9)
// 			{
// 				outstream.put(byte);//outstream<<byte;
// 				currbit=1;
// 			}
// 			if (letterCode%2)
// 			{
// 				byte=byte>>1;cout<<"1";
// 				byte=byte|128;
// 			}
// 			else
// 			{
// 				byte=byte>>1;cout<<"0";
// 			}
			
// 			currbit++;
// 			letterCode/=2;
// 		}
// 		while(zerobitlen--)
// 		{
// 			if(currbit==9)
// 			{
// 				outstream.put(byte);//outstream<<byte;
// 				currbit=1;
// 			}
// 			byte=byte>>1;cout<<"0";
// 			currbit++;
// 		}
// 		cout<<endl;
// 	}
// 	if (currbit!=9)
// 		byte=byte>>(9-currbit);cout<<"unwritten bits left: "<<(9-currbit);
	
// 	outstream.put(byte);//outstream<<byte;
// 	outstream.close();
// 	instream.close();
// 	cout<<"returning makecompressed file";
// 	return 1;
// }


File_Handler::~File_Handler()
{
	free(ipfpath);
	free(ipUniqueFileName);
	free(fname);
	free(hedir);
	free(treePath);
	free(compressedFilePath);
}

int WINAPI openD(char** pfname)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user.
                    if (SUCCEEDED(hr))
                    {
                        MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
                        //use pszFilePath to save filename using widechartomultibyte
                        // int szreq=WideCharToMultiByte
                        // (
                        // CP_UTF8,
                        // 0,//Dont use this -> WC_NO_BEST_FIT_CHARS,
                        // pszFilePath,
                        // -1,/*wcslen(pszFilePath),*///sizeof(pszFilePath),//check if pszFilePath is null terminated
                        // NULL,
                        // 0,
                        // NULL,
                        // NULL
                        // );
                        // *pfname=(char*)malloc(szreq);
                        // WideCharToMultiByte
                        // (
                        // CP_UTF8,
                        // 0,//WC_NO_BEST_FIT_CHARS,
                        // pszFilePath,
                        // -1,//wcslen(pszFilePath),//sizeof(pszFilePath),//check if pszFilePath is null terminated
                        // *pfname,
                        // szreq,
                        // NULL,
                        // NULL
                        // );
                        // strreplace(*pfname);
                        WSTR_to_CharStr(pszFilePath,pfname,0);
                        //MessageBox(NULL, L"mm", L"File Path", MB_OK);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return 0;
}
