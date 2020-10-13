/**********************************************************************************************
Name: Anne Liang
UNIX Tools
HW #8
Due: 03/16/18
Description: Write an example program showing the use of
setjmp() and longjmp() across two function calls. The main program should set
a char * variable to some value. The program should proceed with calling a
function, using your name. This function should write your name into a file.
However, this value could be either “correct” or “incorrect”. A second function
should test the correctness against some pre-defined value (e.g. test an entry against
the correct spelling of your name). You would like your program to go back in time
to the state before the call to the function that wrote into the file. That course of
action should also replace the incorrect entry from the file with a hardcoded value.
*************************************************************************************************/
#include <stdio.h>
#include <setjump.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFSIZE 4096

//Functions used
char *Success(char *(*EnteredName)(void)); 
void Failure(char *); 

int main(int argc, char** argv) { 
	jmp_buf array; 
	char buffer[BUFFSIZE];

	//I will have user enter name
	printf("What is your name?");
	char *name; 
	scanf("%s", name);

	char *file="name.txt";
	//Create the file
	int fd=open(file, O_RDWR);

	int ret=setjmp(array); 
	if (ret==0) { 
		name=Success(name); 
		longjmp(array, 1); 
		//Write into file
		write(fd, "Anne Liang\n", 10);
	} 
	else {  
		Failure(name); 
		write(fd, "FAILURE\n", 7);
	} 
	return 0; 
	
} 

char *Success(char *(*EnteredName)(void)) { 
	char *result=(*EnteredName)(); 
	printf("%s\n", result); 
	if (strncmp(result, "Anne Liang", 10)==0) 
		printf("%s! It is you!", result); 
	return result; 
} 

void Failure(char *x) { 
	int nameNew=strlen(x); 
	int nameOri=strlen("Anne Liang"); 
	if (strncmp(x, "Anne Liang", strlen("Anne Liang"))!=0) 
	printf("You're not %s. Who are you?\n", x); 
}



