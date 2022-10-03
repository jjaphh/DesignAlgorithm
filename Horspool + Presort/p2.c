#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

int bruteString(FILE *);
int bruteStringMatch(char[], char[], int*);
int *shiftTable(char[]);
int horspoolString(FILE *);
int horspoolMatching(char[], char[], int*);
int *badSymbol(char[]);
int boyerMatch(FILE*);
int boyerSearch(char[], char[], int*);
int max(int, int);

//runner function (2.3)
int boyerMatch(FILE *fptr) {
	char line[256];
	char str[256];
	int count = 0;
	int shift = 0;
	printf("Enter a string: ");
	scanf("%s", str);
	while(fgets(line, sizeof(line), fptr) != NULL) {
		int check = boyerSearch(line, str, &shift);
		if (check != -1) {
			count++;
		}
	}
	printf("Number of shifts: %d\n", shift);
	return count;
}

/*
 * Boyer Moore Algorithm based on textbook
 * and lecture notes
 * Found on page 263 - 267
 */
int boyerSearch(char text[], char pattern[], int *shift) {
	int m = strlen(pattern);
	int n = strlen(text);
	int *badTable = badSymbol(pattern);
	int i = 0;
	while (i <= n - m) {
		int j = m - 1;
		while ((j >= 0) && (pattern[j] == text[i + j])) {
			j--;
		}
		if (j < 0) {
			return i;
		}
		else {
			int letter = 0;
			if (text[i + j] >= 97 && text[i + j] <= 122) {
				letter = text[i + j] - 97;
			}
			else if (text[i + j] >= 65 && text[i + j] <= 90) {
				letter = text[i + j] - 39;
			}
			i = i + max(1, j - badTable[letter]);
			(*shift)++;
		}
	}
	return -1;

}

//runner function (2.2)
int horspoolString(FILE *fptr) {
	char line[256];
	char str[256];
	int count = 0;
	int shift = 0;
	printf("Enter a string: ");
	scanf("%s", str);
	//goes through file line by line
	while(fgets(line, sizeof(line), fptr) != NULL) {
		int check = horspoolMatching(line, str, &shift);
		if (check != -1) {
			count++;
		}
	}
	printf("Number of shifts: %d\n", shift);
	return count;
}

//function to find max between 2 integers
int max(int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

/* 
 * Horspool algorithm based on the textbook
 * Found on page 262
 */
int horspoolMatching(char text[], char pattern[], int *shift) {
	int m = strlen(pattern);
	int n = strlen(text);
	int *table = shiftTable(pattern);
	int i = m - 1;
	while (i <= n - 1) {
		int k = 0;
		while ((k <= m - 1) && (pattern[m - 1 - k] == text[i - k])) {
			k++;
		}
		if (k == m) {
			return i - m + 1;
		}
		else {
			int letter = 0;
			if (text[i] >= 97 && text[i] <= 122) {
				letter = text[i] - 97;
			}
			else if (text[i] >= 65 && text[i] <= 90) {
				letter = text[i] - 39;
			}
			i = i + table[letter];
			(*shift)++;
		}
	}
	return -1;
}

//runner function (2.1)
int bruteString(FILE *fptr) {
	char line[256];
	char str[256];
	int count = 0;
	int shift = 0; //fix shifting
	printf("Enter a string: ");
	scanf("%s", str);
	//goes through file line by line
	while(fgets(line, sizeof(line),fptr) != NULL) {
		int check = bruteStringMatch(line, str, &shift);
		if (check != -1) {
			count++;
		}
	}
	printf("Number of shifts: %d\n", shift);
	return count;
}

/*
 * Brute force algorithm based on textbook
 * Page number 105
 */
int bruteStringMatch(char text[], char pattern[], int *shift) {
	int n = strlen(text);
	int m = strlen(pattern);
	for (int i = 0; i < n - m; i++) {
		int j = 0;
		while ((j < m) && (pattern[j] == text[i + j])) {
			j++;
		}
		if (j == m) {
			return i;
		}
		(*shift)++;
	}
	return -1;
}

int* badSymbol(char pattern[]) {
	static int badTable[256];
	int m = strlen(pattern);
	for (int i = 0; i < 256; i++) {
		badTable[i] = -1;
	}
	for (int j = 0; j < m; j++) {
		int letter;
		//lowercase
		if (pattern[j] >= 97 && pattern[j] <=122) {
			letter = pattern[j] - 97;
		}
		//uppercase
		else {
			letter = pattern[j] - 39;
		}
		badTable[letter] = j;
	}
	return badTable;

}

/*
 * Function to calculate the shift table
 * Takes in the pattern.
 * Returns the shift table for that pattern
 */
int* shiftTable(char pattern[]) {
	int m = strlen(pattern);
	static int table[51];
	for (int i = 0; i < 52; i++) {
		table[i] = m;
	}
	for (int j = 0; j < m - 1; j++) {
		int letter;
		//lowercase
		if (pattern[j] >= 97 && pattern[j] <=122) {
			letter = pattern[j] - 97;
		}
		//uppercase
		else {
			letter = pattern[j] - 39;
		}
		table[letter] = m - 1 - j;
	}
	return table;
}

/*
 * Main for running all 3 algorithms
 */
int main() {
	double bruteTime = 0.0;
	double hTime = 0.0;
	double bTime = 0.0;
	FILE *fptr = fopen("data_5.txt", "r");
	if (fptr == NULL) {
		printf("Error opening file\n");
		return 0;
	}

	printf("------------Brute Force Algorithm------------\n");
	clock_t begin = clock();
	int bCount = bruteString(fptr);
	clock_t end = clock();
	bruteTime += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("It took %.6fs to find %d occurrence(s)\n", bruteTime, bCount);

	fseek(fptr, 0 , SEEK_SET);
	printf("------------Horspool's Algorithm------------\n");
	clock_t begin2 = clock();
	int hCount = horspoolString(fptr);
	clock_t end2 = clock();
	hTime += (double)(end2 - begin2) / CLOCKS_PER_SEC;
	printf("It took %.6fs to find %d occurrence(s)\n", hTime, hCount);

	fseek(fptr, 0, SEEK_SET);
	printf("------------Boyer-Moore Algorithm------------\n");
	clock_t begin3 = clock();
	int byCount = boyerMatch(fptr);
	clock_t end3 = clock();
	bTime += (double)(end3 - begin3) / CLOCKS_PER_SEC;
	printf("It took %.6fs to find %d occurrence(s)\n", bTime, byCount);

	fclose(fptr);
	return 0;
}