//CSCI 474 Project 1
//Peter Fraser, Jackson Gravel, Matthew Greatens, Tom Hoskins
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#define SMALL 100000000
#define MEDIUM 1000000000
#define LARGE 10000000000

/* Function Declarations */
double OneProcess(int size);
double TwoProcesses(int size);
double FourProcesses(int size);
double EightProcesses(long size);

/* Main Function */
int main()
{
	time_t small1StartTime = time(NULL);
	clock_t small1Begin = clock();
	double sum1 = OneProcess(MEDIUM);
	clock_t small1End = clock();
	time_t small1EndTime = time(NULL);
	double small1ExecutionTime = (double)(small1End - small1Begin) / CLOCKS_PER_SEC; //Execution time
	int small1WallTime = small1EndTime - small1StartTime;							 //Time elapsed

	printf("\n1 process, small: Sum: %f Execution Time: %f sec Wall Time: %d sec\n", sum1, small1ExecutionTime, small1WallTime);

	time_t small2StartTime = time(NULL);
	clock_t small2Begin = clock();
	double sum2 = TwoProcesses(MEDIUM);
	clock_t small2End = clock();
	time_t small2EndTime = time(NULL);
	double small2ExecutionTime = (double)(small2End - small2Begin) / CLOCKS_PER_SEC;
	int small2WallTime = small2EndTime - small2StartTime;

	printf("\n2 processes, small: Sum: %f Execution Time: %f sec Wall Time: %d sec\n", sum2, small2ExecutionTime, small2WallTime);

	time_t small3StartTime = time(NULL);
	clock_t small3Begin = clock();
	double sum3 = FourProcesses(MEDIUM);
	clock_t small3End = clock();
	time_t small3EndTime = time(NULL);
	double small3ExecutionTime = (double)(small3End - small3Begin) / CLOCKS_PER_SEC;
	int small3WallTime = small3EndTime - small3StartTime;

	printf("\n4 processes, small: Sum: %f Execution Time: %f sec Wall Time: %d sec\n", sum3, small3ExecutionTime, small3WallTime);

	time_t small4StartTime = time(NULL);
	clock_t small4Begin = clock();
	double sum4 = EightProcesses(MEDIUM);
	clock_t small4End = clock();
	time_t small4EndTime = time(NULL);
	double small4ExecutionTime = (double)(small4End - small4Begin) / CLOCKS_PER_SEC;
	int small4WallTime = small4EndTime - small4StartTime;

	printf("\n4 processes, small: Sum: %f Execution Time: %f sec Wall Time: %d sec\n", sum4, small4ExecutionTime, small4WallTime);

	return 0;
}

double OneProcess(int size)
{
	double sum1 = 0;
	double sum2 = 0;
	double tempVal1, tempVal2, tempSum;
	int pipe1[2];
	if (pipe(pipe1) < 0)
		exit(1);
	int p = fork();
	if (p == 0) //child
	{
		close(pipe1[0]);
		for (int i = 0; i < (size); i++)
		{
			tempVal2 = (double)rand() / (double)RAND_MAX;
			sum2 += tanh(tempVal2);
		}
		write(pipe1[1], &sum2, sizeof(sum2));
		exit(0);
	}
	else //parent
	{
		close(pipe1[1]);
		read(pipe1[0], &tempSum, sizeof(tempSum));
		sum1 = tempSum;
	}
	return sum1;
}
double TwoProcesses(int size)
{
	double sum1 = 0;
	double sum2 = 0;
	double sumTotal;
	double tempVal1, tempVal2, tempSum;
	double tempSum1, tempSum2;
	int pipe1[2];
	int pipe2[2];
	if (pipe(pipe1) < 0)
		exit(1);
	int p = fork();
	if (p == 0) //child
	{
		close(pipe1[0]);
		for (int i = 0; i < (size / 2); i++)
		{
			tempVal1 = (double)rand() / (double)RAND_MAX;
			sum1 += tanh(tempVal1);
		}
		write(pipe1[1], &sum1, sizeof(sum1));
		exit(0);
	}
	else //parent
	{
		close(pipe1[1]);
		if (pipe(pipe2) < 0)
			exit(1);
		int p2 = fork();
		if (p2 == 0) //child 2
		{
			close(pipe2[0]);
			for (int i = 0; i < (size / 2); i++)
			{
				tempVal2 = (double)rand() / (double)RAND_MAX;
				sum2 += tanh(tempVal2);
			}
			write(pipe2[1], &sum2, sizeof(sum2));
			exit(0);
		}
		else
		{
			close(pipe2[1]);
			read(pipe1[0], &tempSum1, sizeof(tempSum1));
			read(pipe2[0], &tempSum2, sizeof(tempSum2));
			sumTotal = tempSum1 + tempSum2;
		}
	}
	return sumTotal;
}
double FourProcesses(int size)
{
	int pipe1[2];
	int pipe2[2];
	int pipe3[2];
	int pipe4[2];
	int totalSum = 0;
	double sum1, sum2, sum3, sum4;
	double temp1, temp2, temp3, temp4;
	double tempSum1, tempSum2, tempSum3, tempSum4;

	if (pipe(pipe1) < 0) //pipe was not created successfully if pipe returns a negative value
		exit(1);
	int p1 = fork();
	if (p1 == 0) //child 1
	{
		close(pipe1[0]); //Close read end of the pipe
		sum1 = 0;
		srand(time(NULL));
		for (int i = 0; i < size / 4; i++)
		{
			temp1 = (double)rand() / (double)RAND_MAX;
			sum1 += temp1;
		}
		write(pipe1[1], &sum1, sizeof(sum1));
		printf("\nChild 1 finished with sum %f\n", sum1);
		exit(0);
	}
	else
	{
		close(pipe1[1]);
		if (pipe(pipe2) < 0)
			exit(1);
		int p2 = fork();
		if (p2 == 0) //child 2
		{
			close(pipe2[0]);
			sum2 = 0;
			srand(p1);
			for (int i = 0; i < size / 4; i++)
			{
				temp2 = (double)rand() / (double)RAND_MAX;
				sum2 += temp2;
			}
			write(pipe2[1], &sum2, sizeof(sum2));
			printf("\nChild 2 finished with sum %f\n", sum2);
			exit(0);
		}
		else
		{
			close(pipe2[1]);
			if (pipe(pipe3) < 0)
				exit(1);
			int p3 = fork();
			if (p3 == 0) //child 3
			{
				close(pipe3[0]);
				sum3 = 0;
				srand(p2);
				for (int i = 0; i < size / 4; i++)
				{
					temp3 = (double)rand() / (double)RAND_MAX;
					sum3 += temp3;
				}
				write(pipe3[1], &sum3, sizeof(sum3));
				printf("\nChild 3 finished with sum %f\n", sum3);
				exit(0);
			}
			else
			{
				close(pipe3[1]);
				if (pipe(pipe4) < 0)
					exit(1);
				int p4 = fork();
				if (p4 == 0) //child 4
				{
					close(pipe4[0]);
					sum4 = 0;
					srand(p3);
					for (int i = 0; i < size / 4; i++)
					{
						temp4 = (double)rand() / (double)RAND_MAX;
						sum4 += temp4;
					}
					write(pipe4[1], &sum4, sizeof(sum4));
					printf("\nChild 4 finished with sum %f\n", sum4);
					exit(0);
				}
				else
				{
					close(pipe4[1]);
					read(pipe1[0], &tempSum1, sizeof(tempSum1));
					read(pipe2[0], &tempSum2, sizeof(tempSum2));
					read(pipe3[0], &tempSum3, sizeof(tempSum3));
					read(pipe4[0], &tempSum4, sizeof(tempSum4));
					totalSum = tempSum1 + tempSum2 + tempSum3 + tempSum4;
				}
			}
		}
	}
	return totalSum;
}
double EightProcesses(long size)
{
	if (sizeof(size) == sizeof(int))
	{
		size = (int)size;
	}
	double sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8, parentSum;
	sum1 = sum2 = sum3 = sum4 = sum5 = sum6 = sum7 = sum8 = parentSum = 0;

	double temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;

	int pipe1[2];
	int pipe2[2];
	int pipe3[2];
	int pipe4[2];
	int pipe5[2];
	int pipe6[2];
	int pipe7[2];
	int pipe8[2];

	//Fork1
	if (pipe(pipe1) < 0)
		exit(1);

	int pid1 = fork();
	if (pid1 == 0)
	{
		//Child1
		clock_t childStart = clock();
		close(pipe1[0]);
		srand((unsigned)time(0)); //Generate Random number for child 1 based on time
		for (int i = 0; i < (size / 8); i++)
		{
			temp1 = (double)rand() / (double)RAND_MAX;
			sum1 += tanh(temp1);
		}
		write(pipe1[1], &sum1, sizeof(sum1));
		close(pipe1[1]);
		clock_t childEnd = clock();
		double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
		printf("Child 1 exited with execution time %f \n", childTime);
		exit(11);
	}
	//End Child 1
	else
	{
		//Fork 2
		if (pipe(pipe2) < 0)
			exit(1);

		srand((unsigned)pid1); //Generate seed from pid of child1
		int pid2 = fork();
		if (pid2 == 0)
		{

			//Child 2
			clock_t childStart = clock();
			close(pipe2[0]);
			for (int i = 0; i < (size / 8); i++)
			{
				temp2 = (double)rand() / (double)RAND_MAX;
				sum2 += tanh(temp2);
			}
			write(pipe2[1], &sum2, sizeof(sum2));
			close(pipe2[1]);
			clock_t childEnd = clock();
			double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
			printf("Child 2 exited with execution time %f \n", childTime);
			exit(12);
		}
		//End Child 2
		else
		{
			//Fork 3
			if (pipe(pipe3) < 0)
				exit(1);

			srand((unsigned)pid2); //Generate seed from pid of child 2
			int pid3 = fork();
			if (pid3 == 0)
			{
				//Child3
				clock_t childStart = clock();
				close(pipe3[0]);
				for (int i = 0; i < (size / 8); i++)
				{
					temp3 = (double)rand() / (double)RAND_MAX;
					sum3 += tanh(temp3);
				}
				write(pipe3[1], &sum3, sizeof(sum3));
				close(pipe3[1]);
				clock_t childEnd = clock();
				double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
				printf("Child 3 exited with execution time %f \n", childTime);
				exit(13);
			}
			//End Child 3
			else
			{
				//Fork 4
				if (pipe(pipe4) < 0)
					exit(1);

				srand((unsigned)pid3); //Generate seed from pid of child 3
				int pid4 = fork();
				if (pid4 == 0)
				{
					//Child 4
					clock_t childStart = clock();
					close(pipe4[0]);
					for (int i = 0; i < (size / 8); i++)
					{
						temp4 = (double)rand() / (double)RAND_MAX;
						sum4 += tanh(temp4);
					}
					write(pipe4[1], &sum4, sizeof(sum4));
					close(pipe4[1]);
					clock_t childEnd = clock();
					double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
					printf("Child 4 exited with execution time %f \n", childTime);
					exit(14);
				}
				//End Child 4
				else
				{
					//Fork 5
					if (pipe(pipe5) < 0)
						exit(1);

					srand((unsigned)pid4); //Generate seed from pid of child 4

					int pid5 = fork();
					if (pid5 == 0)
					{
						//Child 5
						clock_t childStart = clock();
						close(pipe5[0]);
						for (int i = 0; i < (size / 8); i++)
						{
							temp5 = (double)rand() / (double)RAND_MAX;
							sum5 += tanh(temp5);
						}
						write(pipe5[1], &sum5, sizeof(sum5));
						close(pipe5[1]);
						clock_t childEnd = clock();
						double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
						printf("Child 5 exited with execution time %f \n", childTime);
						exit(15);
					}
					//End Child 5
					else
					{
						//Fork 6
						if (pipe(pipe6) < 0)
							exit(1);

						srand((unsigned)pid5); //Generate seed from pid of child 5

						int pid6 = fork();
						if (pid6 == 0)
						{
							//Child 6
							clock_t childStart = clock();
							close(pipe6[0]);
							for (int i = 0; i < (size / 8); i++)
							{
								temp6 = (double)rand() / (double)RAND_MAX;
								sum6 += tanh(temp6);
							}
							write(pipe6[1], &sum6, sizeof(sum6));
							close(pipe6[1]);
							clock_t childEnd = clock();
							double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
							printf("Child 6 exited with execution time %f \n", childTime);
							exit(16);
						}
						//End Child 6
						else
						{
							//Fork 7
							if (pipe(pipe7) < 0)
								exit(1);

							srand((unsigned)pid6); //Generate seed from pid of child 6

							int pid7 = fork();
							if (pid7 == 0)
							{
								//Child 7
								clock_t childStart = clock();
								close(pipe7[0]);
								for (int i = 0; i < (size / 8); i++)
								{
									temp7 = (double)rand() / (double)RAND_MAX;
									sum7 += tanh(temp7);
								}
								write(pipe7[1], &sum7, sizeof(sum7));
								close(pipe7[1]);
								clock_t childEnd = clock();
								double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
								printf("Child 7 exited with execution time %f \n", childTime);
								exit(17);
							}
							//End Child 7
							else
							{
								//Fork 8
								if (pipe(pipe8) < 0)
									exit(1);

								srand((unsigned)pid7); //Generate seed from pid of child 7

								int pid8 = fork();
								if (pid8 == 0)
								{
									//Child 8
									clock_t childStart = clock();
									close(pipe8[0]);
									for (int i = 0; i < (size / 8); i++)
									{
										temp8 = (double)rand() / (double)RAND_MAX;
										sum8 += tanh(temp8);
									}
									write(pipe8[1], &sum8, sizeof(sum8));
									close(pipe8[1]);
									clock_t childEnd = clock();
									double childTime = ((double)(childEnd - childStart) / CLOCKS_PER_SEC);
									printf("Child 6 exited with execution time %f \n", childTime);
									exit(18);
								}
								//End Child 8
								else
								{
									//Parent
									close(pipe1[1]);
									close(pipe2[1]);
									close(pipe3[1]);
									close(pipe4[1]);
									close(pipe5[1]);
									close(pipe6[1]);
									close(pipe7[1]);
									close(pipe8[1]);

									int status;

									while (wait(&status) != -1) //Wait returns -1 when no children are left
									{

										if (status >> 8 == 11) //if 1st child status received
										{
											read(pipe1[0], &temp1, sizeof(double));
											close(pipe1[0]);
											parentSum += temp1;
											printf("child_1 finished with sum %f \n", temp1);
										}
										else if (status >> 8 == 12) //if 2nd child status received
										{
											read(pipe2[0], &temp2, sizeof(double));
											close(pipe2[0]);
											parentSum += temp2;
											printf("child_2 finished with sum %f \n", temp2);
										}
										else if (status >> 8 == 13) //if 3rd child status received
										{
											read(pipe3[0], &temp3, sizeof(double));
											close(pipe3[0]);
											parentSum += temp3;
											printf("child_3 finished with sum %f \n", temp3);
										}
										else if (status >> 8 == 14) //if 4th child status received
										{
											read(pipe4[0], &temp4, sizeof(double));
											close(pipe4[0]);
											parentSum += temp4;
											printf("child_4 finished with sum %f \n", temp4);
										}
										else if (status >> 8 == 15) //if 5th child status received
										{
											read(pipe5[0], &temp5, sizeof(double));
											close(pipe5[0]);
											parentSum += temp5;
											printf("child_5 finished with sum %f \n", temp5);
										}
										else if (status >> 8 == 16) //if 6th child status received
										{
											read(pipe6[0], &temp6, sizeof(double));
											close(pipe6[0]);
											parentSum += temp6;
											printf("child_6 finished with sum %f \n", temp6);
										}
										else if (status >> 8 == 17) //if 7th child status received
										{
											read(pipe7[0], &temp7, sizeof(double));
											close(pipe7[0]);
											parentSum += temp7;
											printf("child_7 finished with sum %f \n", temp7);
										}
										else if (status >> 8 == 18) //if 8th child status received
										{
											read(pipe8[0], &temp8, sizeof(double));
											close(pipe8[0]);
											parentSum += temp8;
											printf("child_8 finished with sum %f \n", temp8);
										}
									}

									return parentSum;
								}
							}
						}
					}
				}
			}
		}
	}
}