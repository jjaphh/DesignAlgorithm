#include <stdio.h>
#include <math.h>
#include <time.h>
/*
 * Name: Jason Phung
 * Student ID:1047334
 * Email: phungj@uoguelph.ca
 * Description: CIS3490 Assignment 2
 * Code to find inversions using the brute force algorithm
 * and recursive divide-and-conquer algorithm
 * Question 1.3
 */

#define MAX 50000

int bruteForce(int []);
int mergeMod(int [], int);
int merge(int[], int [], int, int);

//goes through whole array one by one and finds an inversion
int bruteForce(int a[]) {
	int count = 0;
	for (int i = 0; i < MAX - 1; i++) {
		for (int j = i + 1; j < MAX; j++) {
			if (a[i] > a[j]) {
				count++;
			}
		}
	}
	return count;
}

//modified version of the mergeSort algorithm
int mergeMod(int a[], int n) {
	int count = 0;
	if (n > 1) {
		int mb = floor(n/2.0);
		int mc = ceil(n/2.0);
		int b[mb];
		int c[mc];
		//copy first half of A into B
		for (int i = 0; i < mb; i++) {
			b[i] = a[i];
		}
		//copy second half of A into C
		int i = mb;
		int j = 0;
		while ((i < n) || (j < mc)) {
			c[j] = a[i];
			j++;
			i++;
		}
		count = mergeMod(b, mb);
		count = mergeMod(c, mc);
		count = merge(b ,c, mb, mc);
	}
	return count;
}

int merge(int b[], int c[], int p, int q) {
	int count = 0;
	int i = 0;
	int j = 0;

	while (i < p && j < q) {
		if (b[i] <= c[j]) {
			i++;
		}
		else {
			j++;
			count = count + (p - i);
		}
	}
	return count;
}


int main() {
	double excTime = 0.0;
	double merTime = 0.0;

	//to open file
	int a[MAX];
	FILE *fptr = fopen("data_1.txt", "r");
	if (fptr == NULL) {
		printf("Error opening file\n");
		return 0;
	}

	//puts all 50000 ints into an array
	for (int i = 0; i < MAX; i++) {
		fscanf(fptr, "%d ", &a[i]);
	}

	int inv;
	/*
	 * Code for how to calculate execution time was found at
	 * https://www.techiedelight.com/find-execution-time-c-program/
	 * Code starts here:
	 */
	clock_t begin = clock();
	inv = bruteForce(a); //calling brute force algorithm
	clock_t end = clock();
	excTime += (double)(end - begin) / CLOCKS_PER_SEC;
	/*
	 * Code ends here
	 */
	printf("It took %.2fs to find %d inversions with the brute force algorithm\n", excTime, inv);

	int invMerge = 0;
	clock_t begin2 = clock();
	invMerge = mergeMod(a, MAX);
	clock_t end2 = clock();
	merTime += (double)(end2 - begin2) / CLOCKS_PER_SEC;
	printf("It took %.4fs to find %d inversions with the recursive divide-and-conquer algorithm\n", merTime, invMerge);

	fclose(fptr);
	return 0;
}