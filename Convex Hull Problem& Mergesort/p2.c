#include <stdio.h>
#include <math.h>
#include <time.h>
/*
 * Name: Jason Phung
 * Student ID:1047334
 * Email: phungj@uoguelph.ca
 * Description: CIS3490 Assignment 2
 * Solving the convex hull problem using
 * the brute force method and quickHull
 * Question 2.3
 */

#define MAX 30000
typedef struct {
	double x;
	double y;
} Point;

int convexBrute(double[], double[], double[], double[]);
void quickSort(Point points[], int, int);
int partition(Point points[], int, int);
int quickHull(Point[], Point, Point, int, int, Point[]);

//brute force method for finding a set of convex hull points
int convexBrute(double x[], double y[], double cx[], double cy[]) {
	double a;
	double b;
	double c;
	int count = 0;
	//for loop to get first point
	for (int i = 0; i < MAX - 1; i ++) {
		//for loop to get second point
		for (int j = i + 1; j < MAX; j++) {
			int lCount = 0;
			int rCount = 0;
			//equation given in textbook
			a = y[j] - y[i];
			b = x[i] - x[j];
			c = x[i]*y[j] - y[i]*x[j];
			//for loop to compare each points to the line segment given by the first point and second point
			for (int k = 0; k < MAX; k++) { 
				if (((x[k] != x[i]) && (y[k] != y[i])) && ((x[k] != x[j]) && (y[k] != y[j]))) { //if being compared is not the end points of the line
					if ((a*x[k] + b*y[k]) > c) { //checks if any points are to the left of the line
						lCount = 1;
					}
					else if ((a*x[k] + b*y[k]) < c) { //checks if any points are to the right of the line
						rCount = 1;
					}
					if (lCount == 1 && rCount == 1) { //checks if theres points to the right and left of the line (meaing not a convex point)
						break;
					}

				}
			}

			//if theres only points to one side of the line
			if (((lCount == 1) && (rCount == 0)) || ((lCount == 0) && (rCount == 1)))   {
				int dup = 0;
				//loop to make sure the same points are not added twice
				for (int p = 0; p < count; p ++) {
					if (cx[p] == x[i] && cy[p] == y[i]) {
						dup = 1;
						break;
					}
					if (cx[p] == x[j] && cy[p] == y[j]) {
						dup = 1;
						break;
					}
				}
				if (dup == 0) {
					cx[count] = x[i];
					cy[count] = y[i];
					count++;
					cx[count] = x[j];
					cy[count] = y[j];
					count++;

				}
			

			}

		}
	}
	return count;
}

//quicksort function based on the pseudocode in lecture slides
void quickSort(Point points[], int l, int r) {
	if (l < r) {
		int s;
		s = partition(points, l, r);
		quickSort(points, l, s - 1);
		quickSort(points, s + 1, r);
	}
}

int partition(Point points[], int l, int r) {
	double pivot = points[l].x;
	int i = l;
	int j = r + 1;
	while (i < j) {
		do {
			i++;
		} while (points[i].x < pivot); 

		do {
			j--;
		} while (points[j].x > pivot);

		Point tmp = points[i];
		points[i] = points[j];
		points[j] = tmp;
	}
	Point tmp = points[i];
	points[i] = points[j];
	points[j] = tmp;

	tmp = points[l];
	points[l] = points[j];
	points[j] = tmp;

	return j;

}

//function to find what side of the line the point is on
int sideToFind(Point p1, Point p2, Point p) {
	//formula from the textbook
	double a = p2.y - p1.y;
	double b = p1.x - p2.x;
	double c = p1.x*p2.y - p1.y*p2.x;
	if (a*p.x + b*p.y < c) {
		return -1;
	}
	else if (a*p.x + b*p.y > c) {
		return 1;
	} 
	return 0;
}

//quickHull function based on the pseudocode in lecture notes
int quickHull(Point points[], Point p1, Point p2, int n, int side, Point conPoints[]) {
	int pMax = -1; //index given a negative value so check if there is any points left
	double maxDist = 0;
	int side2;
	int count = 0;
	for (int i = 0; i < n; i++) {
		double dist = fabs((points[i].y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (points[i].x - p1.x)); //calculate distance from the point to the line segment
		side2 = sideToFind(p1, p2, points[i]); //finds which side of the line segment the point is on

		if (side2 == side && dist > maxDist ) { //if the point is greater than the current max and is on the correct side of the line
			pMax = i;
			maxDist = dist;
		}

	}

	if (pMax == -1) //if no points exist on one side of the line (meaning it is a convex point)
	{
		int dup = 0;
		//checks for duplicate points
		for (int p = 0; p < count; p++) {
			if (conPoints[p].x == p1.x && conPoints[p].y == p1.y) {
				dup = 1;
				break;
			}
			if (conPoints[p].x == p2.x && conPoints[p].y == p2.y) {
				dup = 1;
				break;
			}
		}
		if (dup == 0) {
			conPoints[count] = p2;
			printf("x: [%.2f] y: [%.2f]\n", conPoints[count].x, conPoints[count].y);
			count++;
			return count;
		}
	
	}

	//recursively calls quickHull on each line segnment made by the point and the endpoints of the line
	count += quickHull(points, p1, points[pMax], n, -sideToFind(p1, points[pMax], p2), conPoints);
	count += quickHull(points, points[pMax], p2, n, -sideToFind(points[pMax], p2, p1), conPoints);
	return count;
}


int main() {
	double xCord[MAX];
	double yCord[MAX];
	double convexX[MAX];
	double convexY[MAX];

	Point points[MAX];
	Point cPoints[MAX];

	double excTime = 0.0;
	double conTime = 0.0;

	FILE *fptr = fopen("data_2.txt", "r");
	if (fptr == NULL) {
		printf("Error opening file\n");
		return 0;
	}

	for (int i = 0; i < MAX; i++) {
		fscanf(fptr, "%lf %lf\n", &xCord[i], &yCord[i]);
	}
	clock_t begin = clock();
	int numCon = convexBrute(xCord, yCord, convexX, convexY);
	clock_t end = clock();
	excTime += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("It took %.2fs to find %d convex points using brute force\n", excTime, numCon);
	
	for (int i = 0; i < numCon; i++) {
		printf("x: [%.2lf] y: [%.2lf]\n", convexX[i], convexY[i]);
	}

	fseek(fptr, 0, SEEK_SET);
	for (int i = 0; i < MAX; i++) {
		fscanf(fptr, "%lf %lf\n", &points[i].x, &points[i].y);
	}

	printf("\n\n");

	clock_t begin2 = clock();
	quickSort(points, 0, MAX - 1);
	int conCount = quickHull(points, points[0], points[MAX - 1], MAX, 1, cPoints);
	conCount += quickHull(points,points[0], points[MAX - 1], MAX, -1, cPoints);
	clock_t end2 = clock();
	conTime += (double)(end2 - begin2) / CLOCKS_PER_SEC;
	printf("It took %.2fs to find %d convex points with the quickHull algorithm\n", conTime, conCount);

	fclose(fptr);
	return 0;
}