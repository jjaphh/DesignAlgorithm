#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Name: Jason Phung
 * ID: 1047734
 * CIS3490 - Assignment 4 Question 2
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

//compare function to compare two words (helper function for qsort)
int comparator (const void *p, const void *q) {
	return strcmp(((struct wordInstance *)p)->theWord, ((struct wordInstance *)q)->theWord);
}

//function to add nodes to a tree
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

//function to create a tree using the greedy algorithm
void createTree(struct tree **root, struct wordInstance words[], int size) {
	float freq_high = -1.0;
	int freq_ind = -1;
	//to find the word with highest freq
	for (int i = 0; i < size; i++) {
		if (words[i].prob > freq_high) {
			freq_high = words[i].prob;
			freq_ind = i;
		}
	}
	if (freq_ind != -1) {
		struct wordInstance word2Add;
		strcpy(word2Add.theWord, words[freq_ind].theWord);
		word2Add.count = words[freq_ind].count;
		word2Add.prob = words[freq_ind].prob;
		add_node(&(*root), word2Add);

		if ((freq_ind - 1) >= 0) {
			struct wordInstance left[freq_ind];
			//copy words to the left of the most frequent word
			for (int i = 0; i < freq_ind; i++) {
				strcpy(left[i].theWord, words[i].theWord);
				left[i].prob = words[i].prob;
				left[i].count = words[i].count;
			}
			//creates a subtree with the words to the left of the current most freq word
			createTree(&(*root), left, freq_ind);
		}

		if ((size - freq_ind) > 1) {
			struct wordInstance right[size - freq_ind - 1];
			int j = freq_ind + 1;
			//copy words to the right of the current most freq word
			for (int i = 0; i < size - freq_ind - 1; i++) {
				strcpy(right[i].theWord, words[j].theWord);
				right[i].prob = words[j].prob;
				right[i].count = words[j].count;
				j++;
			}
			//creates a subtree with the words to the right of the current most freq word
			createTree(&(*root), right, size - freq_ind - 1);
		}
	}
}

//function to search through tree
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

	//sorting the words
	qsort(allWords, n, sizeof(struct wordInstance), comparator);

	struct tree *root_node = NULL;

	createTree(&root_node, allWords, n);

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