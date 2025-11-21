#include <stdio.h>
#include <stdlib.h>
#include "bst.h"


void tree_insert(Tree_Node** root, char data)
{
    if (*root == NULL) {
        Tree_Node* newNode = malloc(sizeof(Tree_Node));     //instead of checking for children during comps, just check first here
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
        return;
    }

    if (data < (*root)->data) 
    {
        tree_insert(&(*root)->left, data);
    } 
    
    else //if (data > (*root)->data) 
    {
        tree_insert(&(*root)->right, data);         // also handles duplicates
    }

    // else if (data == ((*root) -> data))
    // {
    //     Tree_Node* newNode = malloc(sizeof(Tree_Node));
    //     newNode->data = data;
    //     newNode->left = NULL;
    //     newNode->right = NULL;

    //     (*root) -> left = newNode;
    // }
  
} 


Tree_Node* create_bst (char data[])
{
    if (data[0] == '\0')
     {
         return NULL; 
     } 

    Tree_Node* root = malloc(sizeof(Tree_Node));
    root->data = data[0];
    root->left = NULL;
    root->right = NULL;

    int i = 1;
    while (data[i] != '\0') 
    {
        tree_insert(&root, data[i]);
        i++;
    }
    return root;
}

Tree_Node* tree_search(Tree_Node* root, char data){
   
if (root == NULL) {
        return NULL;  
    }

    if (root->data == data) {
        return root;   
    }

    if (data < root->data) {
        return tree_search(root->left, data);   
    } else {
        return tree_search(root->right, data);  
    }
}


void tree_print_sorted(Tree_Node* root)
{

    if (root == NULL)
    {
        return;
    }

    tree_print_sorted(root -> left);

    printf("%c", root -> data);

    tree_print_sorted(root -> right);

}

void tree_delete(Tree_Node* root)
{
    
    if (root == NULL)
    {
        return;
    }

   tree_delete(root -> left);
   tree_delete(root -> right);
   free(root);


}


