#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX 30000

int bruteForce(int[]);
int anagramCheck(char*, char*);
void mergeSort(int[], int);
void merge(int[], int[], int[], int, int, int);
int presort(int[]);

int presort(int a[]) {
	int count = 0;
	char str[256];
	char buff[256];
	printf("Enter a string of numbers: ");
	scanf("%s", str);
	for (int i = 0; i < MAX; i++) {
		sprintf(buff, "%d", a[i]);
		if (strlen(str) == strlen(buff)) {
			int check = anagramCheck(str, buff);
			if (check == 1)
			{
				printf("Anagram found at index[%d]: %s\n", i, buff);
				count++;
			}
		}
	}
	double excTime = 0.0;
	clock_t begin = clock();
	mergeSort(a, MAX);
	clock_t end = clock();
	excTime += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("It took %.06fs to sort\n", excTime);

	return count;
}

void mergeSort(int a[], int n) {
	if (n > 1) {
		int mb = floor((double)n/(double)2.0);
		int mc = ceil((double)n/(double)2.0);
		int b[mb];
		int c[mc];
		//copy first half of A into B
		for (int i = 0; i < mb; i++) {
			b[i] = a[i];
		}
		//copy second half of A into C
		int j = 0;
		for (int i = mb; i < n; i++)
		{
			c[j] = a[i];
			j++;
		}
		mergeSort(b, mb);
		mergeSort(c, mc);
		merge(b, c, a, mb, mc, n);
	}
}

void merge(int b[], int c[], int a[], int p, int q, int n) {
	int i = 0;
	int j = 0;
	int k = 0;
	while (i < p && j < q) {
		if (b[i] <= c[j]) {
			a[k] = b[i];
			i++;
		}
		else {
			a[k] = c[j];
			j++;
		}
		k++;
	}

	if (i == p) {
		while (j < p && k < n) {
			a[k] = c[j];
			k++;
			i++;
		}
	}
	else {
		while (i < p && k < n) {
			a[k] = b[i];
			i++;
			k++;
		}
	}
}

int bruteForce(int a[]) {
	char str[100];
	int count = 0;
	char buff[100];
	printf("Enter a string of numbers: ");
	scanf("%s", str);
	//goes through whole array
	for (int i = 0; i < MAX; i++) {
		sprintf(buff, "%d", a[i]);
		if (strlen(str) == strlen(buff)) {
			int check = anagramCheck(str, buff);
			if (check == 1)
			{
				printf("Anagram found at index[%d]: %s\n", i, buff);
				count++;
			}
		}
	}

	return count;
}

//to check if strings are anagrams
int anagramCheck(char* str1, char* str2) {
	int counter[10] = {0,0,0,0,0,0,0,0,0,0};
	int counter2[10] = {0,0,0,0,0,0,0,0,0,0};

	for (int i = 0; i < strlen(str1); i++) {
		if (str1[i] == '0') {
			counter[0]++;
		}
		else if (str1[i] == '1') {
			counter[1]++;
		}
		else if (str1[i] == '2') {
			counter[2]++;
		}
		else if (str1[i] =='3') {
			counter[3]++;
		}
		else if (str1[i] == '4') {
			counter[4]++;
		}
		else if (str1[i] == '5') {
			counter[5]++;
		}
		else if (str1[i] == '6') {
			counter[6]++;
		}
		else if (str1[i] == '7') {
			counter[7]++;
		}
		else if (str1[i] == '8') {
			counter[8]++;
		}
		else if (str1[i] == '9') {
			counter[9]++;
		}
	}

	for (int i = 0; i < strlen(str2); i++) {
		if (str2[i] == '0') {
			counter2[0]++;
		}
		else if (str2[i] == '1') {
			counter2[1]++;
		}
		else if (str2[i] == '2') {
			counter2[2]++;
		}
		else if (str2[i] =='3') {
			counter2[3]++;
		}
		else if (str2[i] == '4') {
			counter2[4]++;
		}
		else if (str2[i] == '5') {
			counter2[5]++;
		}
		else if (str2[i] == '6') {
			counter2[6]++;
		}
		else if (str2[i] == '7') {
			counter2[7]++;
		}
		else if (str2[i] == '8') {
			counter2[8]++;
		}
		else if (str2[i] == '9') {
			counter2[9]++;
		}
	}

	for (int i = 0; i < 10; i++) {
		if (counter[i] != counter2[i]) {
			return 0;
		}
	}
	return 1;
}

int main() {
	double excTime = 0.0;

	int a[MAX];
	int numAnagrams;
	int num;
	FILE *fptr = fopen("data_4.txt", "r");
	if (fptr == NULL) {
		printf("Error opening file\n");
		return 0;
	}

	for (int i = 0; i < MAX; i++) {
		fscanf(fptr, "%d ", &a[i]);
	}
	/*
	 * Code for how to calculate execution time was found at
	 * https://www.techiedelight.com/find-execution-time-c-program/
	 * Code starts here:
	 */
	clock_t begin = clock();
	numAnagrams = bruteForce(a);
	clock_t end = clock();
	excTime += (double)(end - begin) / CLOCKS_PER_SEC;
	/*
	 * Code ends here
	 */

	printf("It took %.6fs to find %d anagram(s) with a brute force method\n", excTime, numAnagrams);

	printf("------------Presort Algorithm------------\n");

	num = presort(a);
	if (num != 0) {
		printf("%d anagram(s) found\n", num);
	}


	fclose(fptr);
	return 0;
}