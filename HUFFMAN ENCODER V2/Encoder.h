#include <map>
#include <set>
#include <tree.h>
#ifndef __Encoder__H__
#define __Encoder__H__
using namespace std;
bool cmpFunc(const node*  a,const node*  b);

class Encoder
{
	node* htreeroot;
	//multiset<node*,bool (*)(const node*,const node*)> mset ;
	//map<char,node*>& letterFreqMap;//initialise with constructor
	//multiset<node*>:: iterator iter ;

public:
	map<char,int> letter_prefix_0bit_len;
	map< char,int > letterCodeMap ;// contains generated code of each letter

	//void print_multiset();
	node* encoder(map<char,node*>&) ;
	void fillCodemapFromTree(node* root, int pathval,int prefix_0bit_len=0,int flag=1);
	void initialiseCodemap();// creates a pair for each letter in letterCodeMap
	void print_letterCodeMap();
};

#endif