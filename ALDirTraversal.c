/************************************************************
Name: Anne Liang
UNIX Tools
HW#4
Due: 03/06/18
Description: Copy, adapt, and compile the program that 
traverses a file hierarchy from Chapter 4, Files and 
Directories. Taking as input a starting pathname, the program 
descends the file hierarchy from that point, and returns how 
many files of each of the seven types there are, and what 
percentage of the total that represents. (You will need to 
(re)visit Chapter 2, UNIX Standardizations and Implementations, 
and create a *.c and a *.h file for path_alloc().)
************************************************************/

#define S_IFMT 0170000
#define S_ISBLK(m) (((mode) & S_IFMT)==S_IFBLK)
#define S_ISCHR(m) (((mode) & S_IFMT)==S_IFCHR)
#define S_ISDIR(m) (((mode) & S_IFMT)==S_IFDIR)
#define S_ISFIFO(m) (((mode) & S_IFMT)==S_IFIFO)
#define S_ISREG(m) (((mode) & S_IFMT)==S_IFREG)
#define S_ISLINK(m) (((mode) & S_IFMT)==S_IFLNK)
#define S_ISSOCK(m) (((mode) & S_IFMT)==S_IFSOCK)

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

char *path_alloc(int *size){
	char *ptr;
	int pathmin=_PC_PATH_MAX;
	if (pathmin == 0) {
        errno = 0;
        if ( ( pathmin = pathconf("/", _PC_PATH_MAX) ) >= 0 ) {
            if ( errno == 0 )
                pathmin = 0;
            else
                perror("pathconf() error for _PC_PATH_MAX");
        }
        else
            pathmin--;
    }

    return ptr;
}

int main(int argc, char *argv[])
{
	int i;
	struct stat buf;
	char *ptr;
	int totalfiles=0;
	int r=0;
	int d=0;
	int c=0;
	int b=0;
	int f=0;
	int so=0;
	int sy=0;
	int u=0;

	for (i=1; i<argc; i++)
	{
		path_alloc(i);
		printf("%s: ", argv[i]);
		if (lstat(argv[i], &buf)<0)
		{
			printf("lstat() error\n");
			continue;
		}
		printf("buf.st_mode=%o\n", buf.st_mode);
		printf("S_IFMT=%o\n", S_IFMT);
		printf("buf.st_mode & S_IFMT=%o\n", buf.st_mode & S_IFMT);
		printf("buf.st_ino=%llo\n", buf.st_ino);
		printf("buf.st_dev=%o\n", buf.st_dev);
		if (S_ISREG(buf.st_mode))
			ptr="regular";
			totalfiles++;
			r++;
		else if (S_ISDIR(buf.st_mode))
			ptr="directory";
			totalfiles++;
			d++;
		else if (S_ISCHR(buf.st_mode))
			ptr="character special";
			totalfiles++;
			c++;
		else if (S_ISBLK(buf.st_mode))
			ptr="block special";
			totalfiles++;
			b++;
		else if (S_ISFIFO(buf.st_mode))
			ptr="FIFO";
			totalfiles++;
			f++;
		else if (S_ISLNK(buf.st_mode))
			ptr="symbolic link";
			totalfiles++;
			sy++;
		else if (S_ISSOCK(buf.st_mode))
			ptr="socket";
			totalfiles++;
			s++;
		else
			ptr="unknown mode";
			printf("%s\n", ptr);
			totalfiles++;
			u++;
	}

	//Print our percentages
	printf("Here is the percentage of the number of files:");
	printf("regular: %d%", (r*100)/totalfiles);
	printf("character special: %d%", (c*100)/totalfiles);
	printf("block special: %d%", (b*100)/totalfiles);
	printf("FIFO: %d%", (f*100)/totalfiles);
	printf("symbolic link: %d%", (sy*100)/totalfiles);
	printf("socket: %d%", (so*100)/totalfiles);
	printf("unknown: %d%", (u*100)/totalfiles);

	return 0;
}