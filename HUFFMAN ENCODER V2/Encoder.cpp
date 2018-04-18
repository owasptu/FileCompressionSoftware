#include <Encoder.h>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>

bool cmpFunc(const node*  a,const node*  b)
{
	return ((a->freqOfAlphabet) < (b->freqOfAlphabet)) ;
}

//Encoder::Encoder(/*map<char,node*>& t*/): /*letterFreqMap(t), mset(&cmpFunc)*/ {}
//class constructor

/*void Encoder::print_multiset()
{
	multiset<node*>:: iterator iter ;
	for (iter=mset.begin();iter!=mset.end();iter++)
	{
		cout<<(*iter)->alphabet<<endl;
	}
}*/

node* Encoder::encoder( map<char,node*>& letterFreqMap)
{
	cout<<"entering encoder"<<endl;
	multiset<node*,bool (*)(const node*,const node*)> mset(&cmpFunc) ;
	//fills the mset with letter nodes
	map<char,node*>::iterator m_iter;
	for (m_iter = letterFreqMap.begin(); m_iter != letterFreqMap.end(); m_iter++)
	{
		mset.insert( m_iter->second ) ;
	}
	//multiset<  node*, bool(*)(const node*,const node*)  >:: iterator iter ;
	multiset<node*>:: iterator iter ;
	iter = mset.begin();
	if(iter == mset.end() )
		return NULL;
	
	node *temp1=NULL, *temp2=NULL ; //to process and remove first 2 elements of mset
	//int tt=0; //remove line after debugging
	cout<<"chars processed in order: "<<endl;
	while( mset.size() >= 2 ) //till mset has just one element
	{
		//Encoder::print_multiset();
		temp1 = *iter;cout<<"temp1="<<(temp1->alphabet)<<endl ;
		iter++;
		temp2 = *iter;cout<<"temp2="<<(temp2->alphabet)<<endl ;
		mset.erase( (mset.begin()) );
		mset.erase( (mset.begin()) );
		mset.insert( createTreeNode( temp1->freqOfAlphabet + temp2->freqOfAlphabet, temp1, temp2 ) );
		iter = mset.begin();//cout<<"begin-"<<( (*(mset.begin()))->alphabet)<<endl ;

	}

	node* root = *(mset.begin());htreeroot=root;
	cout<<"returning encoder"<<endl;return root;

}

void Encoder::fillCodemapFromTree(node* root,int pathval,int prefix_0bit_len,int flag)
{
	cout<<"entering fillCodemapFromTree"<<endl<<"flag="<<flag<<endl;
	if(!root)
		return;
	if( (!root->right) && (!root->left) )
	{
		//put error mechanism to check root->alphabet is alphanumeric
		cout<<"root->alphabet "<<root->alphabet<<endl;
		if(prefix_0bit_len)
		{
			//cout<<"zerobitlen: "<<zerobitlen<<endl; 
			letter_prefix_0bit_len[ root->alphabet ] = prefix_0bit_len ;
		}
		cout<<"pathval: "<<pathval<<endl;
		letterCodeMap[ root->alphabet ] = pathval ;
		return;
	}
	if(flag)
		fillCodemapFromTree(root->left, pathval*2,prefix_0bit_len+1,1);
	else
		fillCodemapFromTree(root->left, pathval*2,prefix_0bit_len,0);
	fillCodemapFromTree(root->right, (pathval*2) + 1,prefix_0bit_len,0);
	cout<<"returning fillCodemapFromTree"<<endl ;
	return;
}

void Encoder::initialiseCodemap()
{
	//cout<<"entering initialiseCodemap"<<endl;
	for (int i = 97; i < (97+26) ; ++i)
	{
		letterCodeMap[(char)i] = -1;
		letter_prefix_0bit_len[(char)i]=0;
	}
	//cout<<"returning initialiseCodemap"<<endl;return;
}

void Encoder::print_letterCodeMap()
{
	cout<<"generated char key pairs are:"<<endl ;
	map< char,int >::iterator it;
	
	for( it = letterCodeMap.begin(); it != letterCodeMap.end(); it++ )
	{
		cout<<(it->first)<<"  ";
		if(letter_prefix_0bit_len.find(it->first)!=letter_prefix_0bit_len.end())
		{
			int zerobitlen=letter_prefix_0bit_len[it->first];
			while(zerobitlen--)
				cout<<"0";
		}
		cout<<(it->second)<<endl ;
	}
}