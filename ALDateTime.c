/*****************************************************************
Name: Anne Liang
UNIX Tools
HW #6, P1
Due: 03/13/18
Description: Write  a  program  that  appends  the  current  
date  and  time  into  a  temporary  file every  hour.  Run  it  
as  a  background  process.  The  output  should  be  in  a 
formatted table. 
*****************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <time.h>

int main(void)
{
	//Create the temporary file
	char temp[] = "time.txt";
	int fd = mkstemp(temp);

	//Get time
	time_t t;	

	struct tm *dtold; //Date and time (OLD)
	dtold = *localtime(t); //Current time will not change in order to compare time

	struct tm dtnew; //Date and time (NEW)
	dtnew = *localtime(&t);	
	
	//See if the time compared is 1 hour apart
	while(difftime(dtold,dtnew)==3600){
		if(difftime(dtold, dtnew)==3600){
			//Write time in temporary file
			write(fd, dtnew, 30); //Unsure of length of time shown so I put 50 so text isn't cut off
			//Set the new time to old time
			dtold=dtnew;
		}
	}

	return 0;
}

