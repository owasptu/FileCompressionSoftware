#ifndef __tree__H__
#define __tree__H__
#include <stdlib.h>

struct tree {
	int freqOfAlphabet;
	struct tree* left;
	struct tree* right;
	char alphabet;
};

typedef tree node;
typedef tree treeNode;

treeNode* createTreeNode(int val = 0, treeNode* left = NULL, treeNode* right = NULL, char ch='*');

#endif