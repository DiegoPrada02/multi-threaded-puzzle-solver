#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct tnode{
	char* value;
	struct tnode* left;
	struct tnode* right;
};
typedef struct tnode tnode;//tnode is my binary tree node typedef
tnode* talloc(){
	return (tnode*) malloc(sizeof(tnode));
}

void inorder_print(tnode* root){
	if(root == NULL) return;
	inorder_print(root -> left);//visiting the left substree
	printf("%s ", root->value);//visitig the root
	inorder_print(root -> right);//visiting the left substree	
}
int bst_insert(tnode** root_p, char* val){
	if((*root_p) == NULL){//tree is empty
		(*root_p) = talloc();
		(*root_p)->value = strdup(val);
		(*root_p)->left = (*root_p)->right = NULL;
		return 1;//successful insert
	}
	if(!strcmp((*root_p)->value, val))//duplicate!
		return 0;//unsuccessful
	if(strcmp((*root_p)->value, val) > 0)
		return bst_insert(&((*root_p)->left), val);
	return bst_insert(&((*root_p)->right), val);
}

