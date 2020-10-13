/*************************************************************************************************
UNIX Tools
HW #9
Due: 03/16/18
Description: Add functionality to this program to print the
error messages associated with the numeric termination status values from
<signal.h>. Also add functionality to print the values of the members of struct
rusage, which will get filled in by calling wait3(). Check these entries with man
2.
*************************************************************************************************/
#include <unistd.h> /* for read(), write() */
#include <fcntl.h> /* for open() */
#include <string.h> /* for strlen() */
#include <stdio.h> /* for printf() */
#include <stdlib.h> /* for calloc() */
#include <stdbool.h> /* for true */
#include <signal.h>
#include <sys/time.h>   
#include <sys/resource.h> 

void Input(int);
void PrintStatus(int status);

int main(int argc, char *argv[])
{
	struct rusage buf;
	int fd=open("file5.txt", O_CREAT|O_RDWR|O_APPEND, S_IRUSR| S_IWUSR);
	int ret, status;
	for (int n=0; n<3; n++)
	{
		if ((ret=fork())==0) /* child process */
		{
			Input(fd);
			printf("Child process, ret=%d.\n", ret);
			status=wait(&status);
			switch(n)
			{
				case 0:
					exit(7);
					break;
				case 1:
					abort();
					break;
				case 2:
					status=status/0;
					break;
			}
		}
		else
		{
			printf("Parent process, child ret==%d.\n", ret);
			wait(&status);
			PrintStatus(status);
		}
	}

	printRUsage(buf);
	/* end for */
	return 0;
}

void Input(int filedes)
{
	char **name=calloc(3, 20*sizeof(char));
	char **it;
	name[0]="Adriana Wise\n";
	name[1]="Richard Stevens\n";
	name[2]="Evi Nemeth\n";
	for (it=&name[0]; *it!=NULL; it++)
	{
		ssize_t num_bytes=write(filedes, *it, strlen(*it));
		sleep(2);
	}
}
void PrintStatus(int status)
{
	int lower_8_bits;
	if ((lower_8_bits=WIFEXITED(status))==true)
	{
		printf("WIFEXITED(status)=%d\n", WIFEXITED(status));
		printf("Exit status for child=%d\n", WEXITSTATUS(status));
		printf("lower_8_bits=%x\n", status);
	}
	else if (WIFSIGNALED(status)==true)
	{
		printf("WIFSIGNALED(status)=%d\n", WIFSIGNALED(status));
		printf("Exit status for child=%d\n", WTERMSIG(status));
		printf("Exit status for child=%d\n", WCOREDUMP(status));
	}
	else if (WIFSTOPPED(status)==true)
	{
		printf("WIFSTOPPED(status)=%d\n", WIFSTOPPED(status));
		printf("Exit status for child=%d\n", WSTOPSIG(status));
	}
	else if ((lower_8_bits=WIFEXITED(status))==false)
	{
		printf("WIFEXITED(status)=%d\n", perror(status));
	}
	else if (WIFSIGNALED(status)==false)
	{
		printf("WIFSIGNALED(status)=%d\n", perror(status));
	}
	else if (WIFSTOPPED(status)==false)
	{
		printf("WIFSTOPPED(status)=%d\n", perror(status));
	}
}

void printRUsage(struct rusage r){
	printf("Rusage:\n");
	printf("ru_maxrss: %d\n", p.ru_maxrss); /* maximum resident set size */
	printf("ru_ixrss: %d\n", p.ru_ixrss); /* integral shared memory size */
	printf("ru_idrss: %d\n", p.ru_idrss); /* integral unshared data size */
	printf("ru_isrss: %d\n", p.ru_isrss); /* integral unshared stack size */
	printf("ru_minflt: %d\n", p.ru_minflt); /* page reclaims (soft page faults) */
	printf("ru_majflt: %d\n", p.ru_majflt);/* page faults (hard page faults) */
	printf("ru_nswap: %d\n", p.ru_nswap); /* swaps */
	printf("ru_inblock: %d\n", p.ru_inblock); /* block input operations */
	printf("ru_oublock: %d\n", p.ru_oublock); /* block output operations */
	printf("ru_msgsnd: %d\n", p.ru_msgsnd); /* IPC messages sent */
	printf("ru_msgrcv: %d\n", p.ru_msgrcv); /* IPC messages received */
	printf("ru_nsignals: %d\n", p.ru_nsignals); /* signals received */
	printf("ru_nvcsw: %d\n", p.ru_nvcsw); /* voluntary context switches */
	printf("ru_nivcsw: %d\n", p.ru_nivcsw);
}