/****************************************************************************************
Name : Anne Liang
CSCI 49366: UNIX Tools
Professor: Adriana Wise
HW #: 1
Description: Write a small program that creates a file and writes your name in it. Use 
		system functions creat() and write(). Adapt the ./mycp prgram to copy the newly 
		created file into another file without the redirect operators. You need to pass 
		the source and the target files as command line arguments.
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> //For mode

#define BUFFSIZE 4096

int main(int argc, char *argv []){
	//Variables for the program
	char buffer[BUFFSIZE]; //Create buffer for file descriptors
	char *fileN1="ALName.txt";
	char *fileN2="ALName2.txt";
	char *name="Anne Liang";
	
	//Create file
	/************************************************************************
	int creat(const char *path, mode_t mode);
	Modes: S_IRWXO -> Read, Write and Search for users other than file owner
	       S_IRWXU -> Read, Write and Search for file owner
	************************************************************************/
	mode_t mode=  S_IRWXO | S_IRWXU; 
	int fd;
	fd=creat(fileN1, mode); //Create a file
	
	//Write into file
	/************************************************************************
	ssize_t write(int fd, const void *buf, size_t nbytes);
	nbytes: can get the value by getting length of string
	************************************************************************/
	if(fd<0){printf("Cannot write into file");}
	else{
		//Write into file
		int content=write(fd, name, strlen(name));
		
		printf("Writing has been completed in %s.\n", fileN1);
		printf("Please check the text file for the result.\n");
		printf("The text written into file descriptor %d is %d bytes.\n", fd, content);
	}

	int n; 
	int fd2;
	if((fd2=open(fileN2, O_RDWR))<0){
		mode_t mode=  S_IRWXO | S_IRWXU; 
		fd2=creat(fileN2, mode); //Create a file if file doesn't exist
	}

	printf("The file descriptor for %s is: %d\n", fileN1, fd);
	printf("The file descriptor for %s is: %d\n", fileN2, fd2);


	while((n=read(fd, buffer, BUFFSIZE))>0){
		if(write(fd2, buffer, n)!=n)
			printf("write error\n");
	}
	if(n<0)
		printf("read error\n");

	printf("Copying file from %s to %s has been done. \nPlease look at %s for the result.\n", fileN1, fileN2, fileN2);
	
	close(fd);
	exit(0);

}
