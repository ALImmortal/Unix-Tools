/**************************************************************************************
Name: Anne Liang
UNIX Tools
Assignment #: 
Due: 05/08/18
Description: Implement a multi-threaded matrix multiplication.
***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int matrix1[50][50];
int matrix2[50][50];
int matrix3[50][50]; //The size is rows of matrix 1 and cols of matrix 2

/*To be able to use this function in the pthread_join function, 
it will be best to make the function with the only argument void*/
void* matrixMult(void *a){
	int sum=0;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			for(int k=0;k<10; k++){
				sum+=matrix1[i][k]*matrix2[k][j];
			}
			matrix3[i][j]=sum;
			sum=0;
		}
	}
    return 0;
}

int main(int argc, char *argv []){
	int mr1, mc1, mr2, mc2, r, c;
	printf("Enter the size, row by column, for matrix1: \n");
	scanf("%d", &mr1);
	scanf("%d", &mc1);

	printf("Enter the size, row by column, for matrix2: \n");
	scanf("%d", &mr2);
	scanf("%d", &mc2);
//Filling in the values for matrix
    //Filling matrix1
    for(r=0;r<mr1;r++){
   		for(c=0;c<mc1;c++){
        		matrix1[r][c]=rand()%10;
    	}
	}
    //Filling matrix2
    for(r=0;r<mr2;r++){
   		for(c=0;c<mc2;c++){
                matrix2[r][c]=rand()%10;
    	}
	}
//Matrix 1
	//Print out matrix
	printf("Matrix 1:\n");
	for(r=0;r<mr1;r++){
   		for(c=0;c<mc1;c++){
        		printf("%d ",matrix1[r][c]);
    		}
		printf("\n");
	}
	printf("\n");

//Matrix 2
	//Print out matrix
	printf("Matrix 2:\n");
	for(r=0;r<mr2;r++){
   		for(c=0;c<mc2;c++){
        		printf("%d ",matrix2[r][c]);
    		}
		printf("\n");
	}
	printf("\n");
//Using pthread
	pthread_t resultthreads[mr1];

	for (int i = 0; i < mr1; i++) {
        	int* result;
        	pthread_create(&resultthreads[i], NULL, matrixMult, (void*)(result));
    }
    
    for (int i = 0; i < mr1; i++) 
        pthread_join(resultthreads[i], NULL);

//Result Matrix
	printf("The size of the final matrix is %d x %d", mr1, mc2);
	printf("Result Matrix:\n"); 
	for (int i = 0; i < mr1; i++) {
        for (int j = 0; j < mc2; j++) {
            		printf("%d ", matrix3[i][j]);   
        }
        printf("\n");
    }
}

