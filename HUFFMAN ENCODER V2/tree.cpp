#include <tree.h>

treeNode* createTreeNode(int val, treeNode* left, treeNode* right, char ch)
{
	treeNode* newTnode = (treeNode*) malloc(sizeof(treeNode));
	newTnode->freqOfAlphabet = val;
	newTnode->left = left;
	newTnode->right = right;
	newTnode->alphabet = ch;
	return newTnode;
}