/*****************************************************************
Name: Anne Liang
UNIX Tools
HW #7
Due: 03/13/18
Description: 
1. Write a program that finds all users on a system. You should test for a large
number of UID numbers, since no function returns that information wholesale.
2. Modify the program that returns supplementary GIDs for a user to also list
these groups’ names.
*****************************************************************/
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/param.h> 
#include <limits.h>

int main(void)
{
	printf("Here is a list of the users in this system:");
	for(int i=0;i<100;i++){
		struct passwd *pwd;
		uid_t UID=i;
		printf("UID=%d\n", UID);
		uid_t eUID=geteuid();
		printf("UID=%d\n", eUID);
		pwd=getpwuid(UID);
		 
		printf("%d:\tuser name\t\t| %20s \t|\n", i, pwd->pw_name);
	}

	/*Part 2*/
	printf("-------------------Part 2----------------------\n");
	int num_groups;
	gid_t grouplist[NGROUPS_MAX];

	gid_t *gi;

	num_groups=getgroups(NGROUPS_MAX, grouplist);

	printf("There are %d supplementary groups\n", num_groups);
	printf("Here is the list with its group's names:");

	for (int i=0; i<num_groups; i++)
	{
		printf("group %d: %s\n", grouplist[i], getgrgid(grouplist[i]));
	}

	return 0;
}

