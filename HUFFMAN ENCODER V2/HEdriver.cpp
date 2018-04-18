#include <HEdriver.h>
using namespace std;
void driver_encodefile()
{
	char* fname=NULL;
    if(openD(&fname))
        return;//throw error
    if(fname!=NULL)
    	;//MessageBox(NULL, L"nn", L"File Path", MB_OK);
    else
    	return;
    File_Handler FH(fname);
    map< char , node* > letterFreqMap;
    FH.readInputfromFile(letterFreqMap);
    Encoder EN;
    node* root=EN.encoder(letterFreqMap);
    EN.initialiseCodemap();
    EN.fillCodemapFromTree(root,0,0);
	//MessageBox(NULL, L"oo", L"File Path", MB_OK);
	FH.createHEdir();
	//FH.generteFileId();
	FH.set_tree_path();
	bool rs=FH.serializetree_wrapper(root);
	if(!rs)
		MessageBox(NULL, L"ERROR", L"File Path", MB_OK);
	FH.set_compfile_path_toInputLoc();//FH.set_compfile_path();
	FH.makeCompressedFile(EN.letter_prefix_0bit_len,EN.letterCodeMap);
	
}

int driver_decodefile(int x)
{
	x=6;
	return x;
}
