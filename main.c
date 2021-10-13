//CSCI 474 Project 1
//Peter Fraser, Jackson Gravel, Matthew Greatens, Tom Hoskins
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#define SMALL 100000000
#define MEDIUM 1000000000
#define LARGE 10000000000

/* Function Declarations */
float oneProcess(int size);
float twoProcesses(int size);
int main()
{
   time_t small1StartTime = time(NULL);
   clock_t small1Begin = clock();
   float sum1 = oneProcess(SMALL);
   clock_t small1End = clock();
   time_t small1EndTime = time(NULL);
   double small1ExecutionTime = (double)(small1End - small1Begin) / CLOCKS_PER_SEC;    //Execution time
   int small1WallTime = small1EndTime - small1StartTime;                               //Time elapsed

   printf("\n1 process, small: Sum: %f Execution Time: %f sec Wall Time: %d sec\n", sum1, small1ExecutionTime, small1WallTime);

   time_t small2StartTime = time(NULL);
   clock_t small2Begin = clock();
   float sum2 = twoProcesses(SMALL);
   clock_t small2End = clock();
   time_t small2EndTime = time(NULL);
   double small2ExecutionTime = (double)(small2End - small2Begin) / CLOCKS_PER_SEC;
   int small2WallTime = small2EndTime - small2StartTime;

   printf("\n2 processes, small: Sum: %f Execution Time: %f sec Wall Time: %d sec\n", sum2, small2ExecutionTime, small2WallTime);

return 0;
}

float oneProcess(int size)
{
   float sum = 0;
   float tempVal;
   for (int i = 0; i < size; i++)
   {
      tempVal = (float)rand() / (float)RAND_MAX;
      sum += tanh(tempVal);
   }
   return sum;
}
float twoProcesses(int size)
{
   float sum1 = 0;
   float sum2 = 0;
   float tempVal1, tempVal2, tempSum;
   int pipe1[2];
   if(pipe(pipe1) < 0)
      exit(1);
   int p = fork();
   if(p == 0) //child
   {
    close(pipe1[0]);
    for(int i = 0; i < (size/2); i++)
      {
       tempVal2 = (float)rand() / (float)RAND_MAX;
       sum2 += tanh(tempVal2);
      }
    write(pipe1[1], &sum2, sizeof(sum2));
    exit(0);
   }
   else //parent
   {
    close(pipe1[1]);
    for(int i = 0; i < (size/2); i++)
      {
       tempVal1 = (float)rand() / (float)RAND_MAX;
       sum1 = tanh(tempVal1);
      }
    read(pipe1[0], &tempSum, sizeof(tempSum));
    sum1 += tempSum;
   }
return sum1;
}
