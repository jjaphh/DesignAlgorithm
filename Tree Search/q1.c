#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Name: Jason Phung
 * ID: 1047734
 * CIS3490 - Assignment 4 question 1
 */

#define MAX 2045

struct tree
{
	char word[256];
	float prob;
	struct tree *childern[2];
};

struct wordInstance
{
	char theWord[256];
	int count;
	float prob;
};

//function to sum up all counts 
int sum(struct wordInstance words[], int i, int j) {
	int s = 0;
	for (int k = i; k < j + 1; k++) {
		s += words[k].count;
	}
	return s;
}

//function to add nodes to the tree
void add_node(struct tree **root, struct wordInstance theWord) {
	if (*root) //there is a node, so move left or right of the bst
	{
		if (strcmp(theWord.theWord, (*root)->word) < 0 ) {
			add_node(&((*root)->childern[0]), theWord);
		}
		else
		{
			add_node(&((*root)->childern[1]), theWord);
		}
	}
	else //no node here
	{
		(*root) = malloc(sizeof(struct tree));
		strcpy((*root)->word, theWord.theWord);
		(*root)->prob = theWord.prob;
		(*root)->childern[0] = NULL;
		(*root)->childern[1] = NULL;
	}
}

//function to search through the tree with the given key
struct tree *search (struct tree *root, char key[]) {
	if (root)
	{
		printf("Comparing with %s (%f), ", root->word, root->prob);
		if (strcmp(root->word, key) == 0) {
			printf("found.\n");
			return root; //found
		}
		else if (strcmp(key, root->word) < 0) { // go left
			printf("go left subtree.\n");
			return search(root->childern[0], key);
		}
		else { //go right
			printf("go right subtree.\n");
			return search(root->childern[1], key);
		}
	}
	else { //not found
		printf("Not found.\n");
		return NULL;
	}
}

int main() {
	struct wordInstance allWords[MAX];
	FILE *fptr = fopen("data_7.txt", "r");
	if (fptr == NULL) {
		printf("Error opening file, ending program\n");
		return 0;
	}


	//init list of all words
	for (int i = 0; i < MAX; i++) {
		strcpy(allWords[i].theWord, "");
		allWords[i].count = 0;
		allWords[i].prob = 0.0;
	}

	char buff[256];
	int match;


	//reading each word one by one
	for (int i = 0; i < MAX; i++) {
		fscanf(fptr, "%s ", buff);
		match = 0;
		//goes through whole array of all words to see if the current word is already in the list
		for (int j = 0; j < MAX; j++) {
			if (strcmp(buff, allWords[j].theWord) == 0) {
				allWords[j].count++;
				match = 1;
				break;
			}
		}
		//if none of the words in the array of all words contains the current word
		if (match == 0) {
			//find an empty cell in the array of all words
			for (int k = 0; k < MAX; k++) {
				//checks if element k is empty
				if (strcmp(allWords[k].theWord, "") == 0) {
					strcpy(allWords[k].theWord, buff);
					allWords[k].count++;
					break;
				}
			}
		}
	}

	//find the length of allWords, since array was init to a size of 2045
	int n = 0;
	for (int i = 0; i < MAX; i++) {
		if (strcmp(allWords[i].theWord, "") == 0) {
			n = i;
			break;
		}
	}

	for (int i = 0; i < n; i++) {
		allWords[i].prob = (float)allWords[i].count / (float)n;
	}

	int cost[n][n];
	int root[n][n];

	for (int i = 0; i < n; i++) {
		cost[i][i] = allWords[i].count;
	}

	/*
	 * Optimal BST dynamic programming algorithm from textbook, pg 302
	 */
	for (int j = 2; j <= n; j++) {
		for (int i = 0; i <= n - j + 1; i++) {
			int k = i + j - 1;
			cost[i][k] = 99999;
			for (int r = i; r <= k; r++) {
				//following the lecture slides to find the main table, min {C[i,k-1] + C[k+1,j]} + sum of the countabilites
				int c = ((r > i)? cost[i][r-1]:0) + ((r < j)? cost[r+1][k]:0) + sum(allWords, i, k);
				if (c < cost[i][k]) {
					cost[i][k] = c;
				}
				root[i][j] = k;
			}
		}
	}

	//to init the root node to the most frequent word 
	int root_ind = root[0][n-1];
	struct tree *root_node = malloc(sizeof(struct tree));
	strcpy(root_node->word, allWords[root_ind].theWord);
	root_node->prob = allWords[root_ind].prob;
	root_node->childern[0] = NULL;
	root_node->childern[1] = NULL;

	//adding nodes, making the most frequent words the parent 
	for (int i = 0; i < n ; i++) {
		struct wordInstance word2Add;
		strcpy(word2Add.theWord, allWords[i].theWord);
		word2Add.count = allWords[i].count;
		word2Add.prob = allWords[i].prob;
		add_node(&root_node, word2Add);
	}

	//printf("Finished creating tree\n");
	printf("Enter a key: ");
	char key[256];
	scanf("%s", key);
	struct tree * found;
	found = search(root_node, key);
	if (found == NULL) {
		
	}

	return 0;
}
