/***********************************************************************
Anne Liang
UNIX Tools
HW: Conditional Variable
Due: 04/13/18
Description: Find the flaw and modify the program on p.3 to not rely on the timed wait in order
to return control over the shared file from thread 2 back to thread 1. Use a simple
pthread_cond_wait() function instead.
***********************************************************************/
#include <pthread.h> /* for pthread_t etc. */
#include <stdio.h> /* for printf() */
#include <unistd.h> /* for read(), write(), sleep() */
#include <errno.h> /* for errno */
#include <stdlib.h> /* for calloc() */
#include <fcntl.h> /* for open() */
#include <string.h> /* for strlen(), strerror() */
#include <malloc/malloc.h> /* for malloc() */
#define BUFFSIZE 4096

pthread_t tid1, tid2;
void *tret1, *tret2;
int fd;
int status;
/* create a structure<mutex, cond, value> */

typedef struct
{
	pthread_mutex_t *mutex;
	pthread_cond_t *cond;
	int value;
} my_struct_t;

my_struct_t data;
struct timespec *timeout;
void *start_rtn1(void *arg);
void *start_rtn2(void *arg);
char **names1, **names2;
int main(void)
{
	time_t now;
	char buf[27];
	int err, err1, err2;
	fd=open("file6.txt", O_WRONLY | O_NONBLOCK | O_APPEND | O_CREAT | O_EXLOCK, S_IRUSR|S_IWUSR);

	/* initialize mutex with default attributes */
	data.mutex=(pthread_mutex_t *)
	malloc(sizeof(pthread_mutex_t));
	status=pthread_mutex_init(data.mutex, NULL);
	printf("Initialization mutex, status=%d.\n", status);

	/* initialize condition variable with default attributes */
	data.cond=(pthread_cond_t *)malloc(sizeof(pthread_cond_t));
	status=pthread_cond_init(data.cond, NULL);

	/* initialize variable whose value change signals that
	"condition" is fulfilled */
	data.value=0; /* we want thread 1 to stop when 3 lines of
	output into file6.txt have been written */
	names1=calloc(6, 20*sizeof(char));
	names2=calloc(8, 20*sizeof(char));
	err=pthread_create(&tid1, NULL, start_rtn1, NULL);
	err=pthread_create(&tid2, NULL, start_rtn2, NULL);
	err=pthread_join(tid1, &tret1); /* thread 1 joins the main thread */
	err=pthread_join(tid2, &tret2); /* thread 2 joins the main thread */
	pthread_cond_wait(data.cond, data.mutex);
	printf("After having waited for both threads...\n");
	status=pthread_cond_destroy(data.cond);
	status=pthread_mutex_destroy(data.mutex);
	printf("Thread %d has exit code %ld.\n", (int) tid1, (long)tret1);
	printf("Thread %d has exit code %ld.\n", (int) tid2, (long)tret2);
	return 0;
}
void *start_rtn1(void *arg)
{
	printf("In start_rtn1()...\n");
	int err1=pthread_mutex_lock(data.mutex);
	off_t offset;
	ssize_t num_bytes;
	int count1=0;
	printf("Doing stuff in thread 1...\n");
	char **name=calloc(6, 20*sizeof(char));
	char **it;
	name[0]="Adriana Wise\n";
	name[1]="Richard Stevens\n";
	name[2]="Evi Nemeth\n";
	name[3]="Thomas Cormen\n";
	name[4]="David Butenhof\n";
	name[5]="Marc Rochkind\n";
	data.value=0;
	count1=0;
	for (it=&name[0]; *it!=NULL; it++)
	{
		count1++;
		num_bytes=write(STDOUT_FILENO, *it, strlen(*it));
		num_bytes=write(fd, *it, strlen(*it));
		if (count1%3==0)
		{
			data.value=1;
			err1=pthread_cond_signal(data.cond);
			printf("-------Condition was signaled-------\n");
			err1=pthread_mutex_unlock(data.mutex);
			sleep(1);
			pthread_cond_wait(data.cond, data.mutex);
		}
	}
	printf("Thread 1 returning...\n");
	pthread_exit ((void *) 1);
}
void *start_rtn2(void *arg)
{
	printf("In start_rtn2()...\n");
	int err2=pthread_mutex_lock(data.mutex);
	off_t offset;
	ssize_t num_bytes;
	int count2=0;
	printf("Doing stuff in thread 2...\n");
	char **name=calloc(8, 20*sizeof(char));
	char **it;
	name[0]="William Sakas\n";
	name[1]="Susan Epstein\n";
	name[2]="Stewart Weiss\n";
	name[3]="Subash Shankar\n";
	name[4]="Ioannis Stamos\n";
	name[5]="Eric Schweitzer\n";
	name[6]="Christina Zamfirescu\n";
	name[7]="Constantin Negoita\n";
	count2=0;
	for (it=&name[0]; *it!=NULL; it++)
	{
		count2++;
		num_bytes=write(STDOUT_FILENO, *it, strlen(*it));
		num_bytes=write(fd, *it, strlen(*it));
		if (count2%4==0)
		{
			data.value=0;
			int err2=pthread_cond_signal(data.cond);
			printf("-------Condition was signaled-------\n");
			err2=pthread_mutex_unlock(data.mutex);
			sleep(1);
			pthread_cond_wait(data.cond, data.mutex);
		}
	}
	printf("Thread 2 returning...\n");
	pthread_exit ((void *) 2);
}