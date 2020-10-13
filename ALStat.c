/********************************************************************
Name: Anne Liang
UNIX Tools
HW#3
Part: 1
1. Write a small program that illustrates the use of the chown() system call on a
file.
2. Write a program that displays all the information from the stat structure
about a file in a “formatted” table, with the constant names on the left, and
their binary values on the right. In particular, the st_mode member of the
stat structure, which encodes the file type and the permissions mode, should
be broken down into the values of its components, given that each bit has a
meaning. List the meanings of each bit of st_mode and its values for your file.
Convert into binary using the % operator on the octal value of st_mode , then
test against the values obtained by selecting each bit using bitwise operations.
There are 16 bits for st_mode (no two leading 0 bits as in the mask examples):
• 1 for set-user-ID
• 1 for set-group-ID
• 1 for the sticky bit
• 4 for the file type
• 9 for access permissions
********************************************************************/

/******************************************************************
#include <unistd.h>
int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *pathname, uid_t owner, gid_t group);
****************************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]){
/*Receive inputs*/
/*Pathname*/
	char *path="books.txt"; //Wasn't sure the path for which specific file so I just put books.txt 
/*New User ID*/
	uid_t ownerNew="AnneL";
/*New Group ID*/
	gid_t groupNew="ALGroup";
/*We will need this to get user ID and group ID*/
	struct stat buf; 

/*First, we should check the stat of the file, given the path*/
if (lstat(path, &buf)<0)
{
	printf("lstat() error\n");
	continue;
}

/*Print out the user ID and group ID*/
	printf("The current information given for the file is: \n");
	printf("User ID: %d\n", buf.st_uid); /*User ID*/
	printf("Group ID: %d\n\n", buf.st_gid); /*Group ID*/

/*Change owner and group of file*/
	printf("Following information is being changed...\n\n");
	printf("Change complete! Here is the updated information:\n");
	printf("User ID: %d\n", ownerNew); /*User ID*/
	printf("Group ID: %d\n", groupNew); /*Group ID*/

	chown(path. ownerNew, groupNew);

/*Binary Print out*/
	/*Get the file/path name*/
	char *filepath=argv[1];
	if (lstat(filepath, &buf)<0)
	{
		printf("lstat() error\n");
		continue;
	}

	printf("-----------------------------------------\n");
	/*set-user-ID*/	
	printf("|\tS_ISUID (user-ID)\t\t|\t%o\t|\n", S_ISUID);
	/*set-group-ID*/
	printf("|\tS_ISGID (group-ID)\t\t|\t%o\t|\n", S_ISGID);
	/*Sticky bit*/
	printf("|\tS_ISVTX (sticky bit)\t\t|\t%o\t|\n", S_ISVTX);
	/*File type*/
	printf("|\tS_IFBLK (file type)\t\t|\t8x%o\t\t|\n", S_IFBLK);
	printf("|\tS_IFCHR (file type)\t\t|\t8x%o\t\t|\n", S_IFCHR);
	printf("|\tS_IFDIR (file type)\t\t|\t8x%o\t\t|\n", S_IFDIR);
	printf("|\tS_IFREG (file type)\t\t|\t8x%o\t|\n", S_IFREG);
	/*Access Permission Bits*/
	printf("|\tS_IRUSR (access permission)\t\t|\t%o\t|\n", S_IRUSR);
	printf("|\tS_IWUSR (access permission)\t\t|\t%o\t|\n", S_IWUSR);
	printf("|\tS_IXUSR (access permission)\t\t|\t%o\t|\n", S_IXUSR);
	printf("|\tS_IRGRP (access permission)\t\t|\t%o\t|\n", S_IRGRP);
	printf("|\tS_IWGRP (access permission)\t\t|\t%o\t|\n", S_IWGRP);
	printf("|\tS_IXGRP (access permission)\t\t|\t%o\t|\n", S_IXGRP);
	printf("|\tS_IROTH (access permission)\t\t|\t%o\t|\n", S_IROTH);
	printf("|\tS_IWOTH (access permission)\t\t|\t%o\t|\n", S_IWOTH);
	printf("|\tS_IXOTH (access permission)\t\t|\t%o\t|\n", S_IXOTH);
	printf("-----------------------------------------\n");

return 0;
}






