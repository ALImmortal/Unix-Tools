/*******************************************************************
Name: Anne Liang
UNIX Tools
HW #5
Due: 03/09/18
Description: Write a short program that writes into a file, and then 
reads back to the standard output stream, two binary objects of 
different types, a character array, and an integer array.
*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>

int main(void)
{
	//Naming my file "resultAL.txt" for easier recognition
	FILE *myfile=fopen("resultAL.txt", "r+b"); //Will create a new file since it doesn't exist
	FILE *file1=fopen("file1.txt", "r+b");
	FILE *file2=fopen("file2.txt", "r+b");

	//Char array
	char *s=(char *)malloc(10*sizeof(char));
	s="Anne Liang";

	//Integer array
	int intarray [5]= {1,2,3,4,5};
	
	//Print out binary files
	//File 1
	printf("File 1: ");
	//print contents of file 1
	char *f1;
	getline(file1, f1);
	printf("Content in file 1: %s\n", f1);

	//File 2
	printf("File 2: ");
	//print contents of file 2
	char *f2;
	getline(file2, f2);
	printf("Content in file 2: %s\n", f2);

	//Print out char array
	printf("Char array: %s\n", s);

	//Int array
	printf("Int array: ");
	for(int i=0;i<5;i++){
		printf("%d ", intarray[i]);
	}
	printf("\n");

	//Buffer to put all the info I provided
	char buffer[50];
	fwrite(s, strlen(s)+1, 1, myfile);
	fwrite(f1, strlen(f1)+1, 1, myfile);
	fwrite(f2, strlen(f2)+1, 1, myfile);
	fwrite(intarray, sizeof(int), sizeof(intarray), myfile);

	fseek(myfile, 0, SEEK_SET);
	fread(buffer, strlen(s)+1, 1, myfile);
	printf("The result:\n");
	printf("%s\n", buffer);
	fclose(myfile);
	return 0;
}