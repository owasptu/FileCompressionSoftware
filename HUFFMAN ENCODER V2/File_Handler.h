// https://stackoverflow.com/questions/12387239/reference-member-variables-as-class-members
#include <fstream>
#include <tree.h>
#include <Shlobj.h>
#include <map>
#ifndef __File_Handler__H__
#define __File_Handler__H__
using namespace std;

int WINAPI openD(char** pfname);

class File_Handler
{
	//add deconstructor to free memory
	char* ipfpath;
	char* ipUniqueFileName;
	char* fname;
	char* hedir;
	char* treePath;
	char* compressedFilePath;
	int hedir_size;
	int fileID;
	//map<char,node*>& letterFreqMap;// should be initialised in constructor's initializer list
	
public:
	map< char , node* >& readInputfromFile(map< char, node* >& /*letterFreqMap*/) ;
	//returns an map with a node for each letter containing it's frequency
	bool serializetree_wrapper(node*);
	void serializetree(node* , ofstream& );
	//saves a tree in preorder traversal using + as marker for null
	char* createHEdir();
	void set_tree_path();
	void set_compfile_path();// path to documents
	void set_compfile_path_toInputLoc();
	bool makeCompressedFile(map< char,int >&,map< char,int >&);
	int generateFileId();
	void SetipUniqueFileName();// sets the variable ipUniqueFileName using ipfpath
	bool DeleteInputFile();
	~File_Handler();

	File_Handler(/*map<char,node*>& t,*/char *x) /*letterFreqMap(t),*/  
	{
		ipfpath=x;
		SetipUniqueFileName();
		fname=NULL;
		hedir=NULL;
		treePath=NULL;
		compressedFilePath=NULL;
		hedir_size=-1;
		fileID=-1;
	}
};

#endif