/*
CSCI 474 - Project 1
Peter Fraser, Jackson Gravel, Matthew Greatens, and Tom Hoskins
*/

/*
imports:
- stdio.h for general functionality
- stdlib.h for general functionality
- math.h for hyperbolic tangent function
*/
#import <stdio.h>
#import <stdlib.h>
#import <math.h>

/*
Define commonly used values to avoid "magic numbers"
*/
#define SMALL 100000000			//100,000,000
#define MEDIUM 1000000000		//1,000,000,000
#define LARGE 10000000000		//10,000,000,000

/*
Main function - medium and large dataset files are commented out to avoid the large amount of time it takes
to generate them. Files are whitespace delimited.
*/
int main(void)
{
	int counter;
	float tempFloat;
	/*
	Create small, medium, and large files containing values between 0 and 1
	*/
	FILE* small, * medium, * large;
	//small dataset
	small = fopen("smallDataset.txt", "w");
	for (counter = 0; counter < SMALL; counter++)
	{
		tempFloat = (float)rand() / RAND_MAX;		//Creates random floating point value between 0 and 1
		fprintf(small, "%f ", tempFloat);			//fprintf(file pointer, format string, variable)
	}
	fclose(small);
	/*
	//medium dataset
	medium = fopen("mediumDataset.txt", "w");
	for (counter = 0; counter < MEDIUM; counter++)
	{
		tempFloat = (float)rand() / RAND_MAX;		//Creates random floating point value between 0 and 1
		fprintf(medium, "%f ", tempFloat);			//fprintf(file pointer, format string, variable)
	}
	fclose(medium);
	*/
	/*
	//large dataset
	large = fopen("largeDataset.txt", "w");
	for (counter = 0; counter < LARGE; counter++)
	{
		tempFloat = (float)rand() / RAND_MAX;		//Creates random floating point value between 0 and 1
		fprintf(large, "%f ", tempFloat);			//fprintf(file pointer, format string, variable)
	}
	fclose(large);
	*/
	return 0;
}