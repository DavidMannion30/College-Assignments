#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 50

typedef struct Book
{
	char bookName[SIZE];
	// char* author[SIZE];				//seg fault due to author pointer returning NULL
	int wordCount;
	int bookId;

	struct Book* left;
	struct Book* right;

	int height; 			//for AVL
} Book;

Book* root = NULL;					//global root
int bookIdValue = 1;				// starting Id's at 1, avoiding 0 as returning 0 could have be a negative in certain cases

int numberOfNodes = 0;
int totalSearches = 0;

int numberOfNodesCreated = 0;


int
bstdb_init ( void ) {
	
	root = NULL;
	bookIdValue = 1;
	return 1;
}



int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}


int getHeight(Book* book)
{
	if (book == NULL)
	{
		return 0;
	}
	else
	{
		return book->height;
	}
}


int getBalanceFactor(Book* book)
{
	if (book == NULL)
	{
		return 0;
	}
	else
	{
		return (getHeight(book->left) - getHeight(book->right));
	}
}



Book* rightRotate(Book* book)
{
    Book* temp1 = book->left;      
    Book* temp2 = temp1->right; 

    temp1->right = book;           
    book->left = temp2;            

    book->height = max(getHeight(book->left), getHeight(book->right)) + 1;
    temp1->height = max(getHeight(temp1->left), getHeight(temp1->right)) + 1;

    return temp1;                  
}


Book* leftRotate(Book* book)
{
    Book* temp1 = book->right;  
    Book* temp2 = temp1->left;   

    temp1->left = book;         
    book->right = temp2;         

    book->height = max(getHeight(book->left), getHeight(book->right)) + 1;
    temp1->height = max(getHeight(temp1->left), getHeight(temp1->right)) + 1;

    return temp1;           
}



void treeDelete(Book* root)
{ 
    if (root == NULL)
    {
        return;
    }

   treeDelete(root -> left);
   treeDelete(root -> right);
   free(root);
}


int calculateHeight(Book* book)
{
	if (book == NULL)
	{
		return 0;
	}

	int leftHeight = calculateHeight(book->left);
	int rightHeight = calculateHeight(book->right);

	return 1 + max(leftHeight,rightHeight);

}


Book* insertIntoDB(Book* node, Book* newBook) {
    if (node == NULL)
        return newBook;

    if (newBook->bookId < node->bookId)
        node->left = insertIntoDB(node->left, newBook);
    else if (newBook->bookId > node->bookId)
        node->right = insertIntoDB(node->right, newBook);
    else
        return node; 		


    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);


    if (balance > 1 && newBook->bookId < node->left->bookId)			//left left
        return rightRotate(node);

    if (balance < -1 && newBook->bookId > node->right->bookId)			//right right
        return leftRotate(node);	
		
    if (balance > 1 && newBook->bookId > node->left->bookId) {			//left right
        node->left = leftRotate(node->left);			
        return rightRotate(node);
    }

    if (balance < -1 && newBook->bookId < node->right->bookId) {		//right left
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}




int bstdb_add(char *name, int word_count, char *author) {
    Book* newBook = malloc(sizeof(Book));
    if (!newBook) return -1; // malloc failed

	numberOfNodesCreated++;

    strncpy(newBook->bookName, name, SIZE - 1);
    newBook->bookName[SIZE - 1] = '\0';

    newBook->wordCount = word_count;
    newBook->bookId = bookIdValue++;
    newBook->left = NULL;
    newBook->right = NULL;
    newBook->height = 1;

    root = insertIntoDB(root, newBook);  

    return newBook->bookId;
}



int 
bstdb_get_word_count(int doc_id) 
{

    Book* current = root;
	totalSearches++;


    while (current != NULL) 
	{
		numberOfNodes++;

        if (doc_id == current->bookId)
		{
			return current->wordCount;
		}
        else if (doc_id < current->bookId)
		{
			current = current->left;
		}
        else
		{
			current = current->right;
		}
    }
    return -1; 
}


char*
bstdb_get_name ( int doc_id ) 
{

	Book* current = root;
	totalSearches++;

	while(current != NULL)
	{
		numberOfNodes++;

		if (doc_id == current->bookId)
		{
			return current->bookName;
		}
		else if (doc_id < current->bookId)
		{
			current = current->left;
		}
		else if (doc_id > current->bookId)
		{
			current = current->right;
		}
	}
	return 0;
}


int checkIfBalanced(Book* node)
{
	if (!node) 
	{
		return 1;
	}  

    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);

    if (abs(leftHeight - rightHeight) > 1)
        return 0;

    return checkIfBalanced(node->left) && checkIfBalanced(node->right);
}


int balanceInfo(Book* node)
{
	if (!node)
	{
		printf("Tree is empty, what the helly\n");
	}

	int left = getHeight(node->left);
	int right = getHeight(node->right);
	int middle = getHeight(node);

	printf("Left levels = %d, Middle levels = %d, Right Levels = %d\n", left, middle, right);
	

}


void
 bstdb_stat(void) {


	//PROOF OF NO DUPLICATES FROM insertIntoDb

    // if (newBook->bookId < node->bookId)
    //     node->left = insertIntoDB(node->left, newBook);
    // else if (newBook->bookId > node->bookId)
    //     node->right = insertIntoDB(node->right, newBook);
    // else
    //     return node; 		if its less it goes into the left subtree, greater than it goes into the right subtree, if its equal it returns the node
	
	int balancedResult = 0;
	balancedResult = checkIfBalanced(root);


	printf("STATS\n");

	if (balancedResult)
	{
		printf("The tree is balanced\n");
	}

	balanceInfo(root);

	 if (totalSearches > 0) 
	{
        double averageNodes = (double)numberOfNodes / totalSearches;
        printf("Average nodes per search: %.2f\n", averageNodes);
    } 
	else 
	{
        printf("No searches performed\n");
    }

	if (numberOfNodesCreated == (bookIdValue - 1))
	{
		printf("The number of nodes matches the number of insertions\n");
	}
	else
	{
		printf("The number of nodes does not match the number of insertions\n");
		printf("The number of nodes = %d\n", numberOfNodes);
	}

	printf("\n");

	printf("The number of nodes is %d\n", numberOfNodes);
	printf("The number of insertions is %d\n", numberOfNodesCreated);
 
}


void
bstdb_quit ( void ) 
{
	treeDelete(root);
}

