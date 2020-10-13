/*******************************************************************************
Name: Anne Liang
UNIX Tools
Client Server Project
Due: May 15, 2018
Description: DB_SEARCH, DB_INSERT, DB_DELETE, DB_REPLACE
	- having  the  strings represent book records from your second assignment. 
	- The client requests should be aimed  at  taking  a  book  record  from  
	a  user  client,  and  perform  one  of   the  insert, delete,  or  replace  
	actions  as  requested  by  the  client  (also  on  the  command  line), 
	and, depending on whether the book is found in the records, perform the required 
	action: 
	DELETE, delete the record;
	INSERT, if  the book is found in the records, increment its count in the record; 
	REPLACE, and the book is found in the records, also increment its count in the record;  
	DELETE,  and  the  book  is  not  in  the  records,  output  an 
	error message; 
	INSERT, and the book is not in the records, insert happily;
	REPLACE,  and  the  book  is  not  in  the  records,  insert  it instead.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> /*for open()*/
#include <sys/stat.h> /*for file access permission bits*/
#include <pthread.h>
#include <string.h> /*for strcat() */
#include <errno.h> /*for errno */

#define CLIENT_THREADS 4
#define READ_REQUEST 1
#define WRITE_REQUEST 2
#define QUIT_REQUEST 3

#define DB_SEARCH 5
#define DB_INSERT 6
#define DB_DELETE 7
#define DB_REPLACE 8

/* a per-request struct, one per client thread */
typedef struct request_tag
{
	struct request_tag *next; /* pointer to next request */
	int operation; /* one of read/write/quit request */
	int synchronous; /* whether request is synchronous */
	int done_flag; /* predicate this request done */
	pthread_cond_t done;/* wait for this thread to finish */
	char prompt[32]; /* prompt server message to client */
	char text[28]; /* read/write text */
} request_t;

/* a server struct, organized as a client request queue */
typedef struct server_tag
{
	request_t *first; /* first request in the queue */
	request_t *last; /* last request in the queue */
	int running; /* predicate this server running */
	pthread_mutex_t mutex; /* lock around server data */
	pthread_cond_t request; /* wait for a request */
} server_t;

/*Functions*/
server_t server={NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
int client_threads; /* client has 4 threads */
pthread_mutex_t client_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clients_done=PTHREAD_COND_INITIALIZER;
void *server_routine(void *arg);
void server_request(int operation, int sync, const char *prompt, char *string);
void *client_routine(void *arg);

int main(int argc, char **argv)
{
	char *book=argv[1];
	char *r=argv[2];
	pthread_t thread;
	int count;
	int err;
	client_threads=CLIENT_THREADS; /* #defined to 4 */
	for (count=0; count<client_threads; count++)
	{
		printf("Creating client #%d...\n", count);
		err=pthread_create(&thread, NULL, client_routine, (void *)(intptr_t)count);
	}
	err=pthread_mutex_lock(&client_mutex);
	while (client_threads>0)
		err=pthread_cond_wait(&clients_done, &client_mutex);
	err=pthread_mutex_unlock(&client_mutex);
	printf("All clients done.\n");
	server_request(QUIT_REQUEST, 1, NULL, NULL);

	char *titleAuthor= argv[2]; //User would provide title and author in quotations
			    //Ex: "MagicTreehouse MaryPopeOsborne"
	char *path_name="/Users/awise/Stevens/Lecture2/books.txt"; //I have provided my own books.txt but it was only for testing.
								   //to test my own code, my path was: /data/biocs/b/student.accounts/ANNE.LIANG10/...so on
	
	return 0;
}

void *server_routine(void *arg)
{
	int fd=open("books.txt", O_CREAT|O_RDWR);
	off_t b=(off_t)getBytePosition(row, fd);
	off_t curr_byte;
	ssize_t num_bytes;
	char buf[200];
	char *title="The Fault In Our Stars, John Green";
	curr_byte=lseek(fd, 0, SEEK_CUR);
	num_bytes=lseek(fd, 0, SEEK_END);
	curr_byte=lseek(fd, 0, SEEK_SET);
	ssize_t oldLength=lseek(fd, 1, 0);
	off_t b=(off_t)getBytePosition(row, fd);

	ssize_t read_bytes=read(fd, buf, num_bytes);
	ssize_t written_bytes=write(1, buf, read_bytes);

	printf("In server_routine()...\n");
	static pthread_mutex_t prompt_mutex=
	PTHREAD_MUTEX_INITIALIZER;
	request_t *request;
	int operation;
	int err;
	while (1)
	{
		/* lock the server mutex, to protect server-modifying
		data */
		err=pthread_mutex_lock(&server.mutex);
		while (server.first==NULL)
		{
			/* wait for request */
			err=pthread_cond_wait(&server.request, &server.mutex);
		}
		/* dequeue 1st request in the queue */
		request=server.first;
		/* advance the head of the queue */
		server.first=request->next;
		if (server.first==NULL)
		/* if queue is empty, reset tail to NULL */
		server.last=NULL;
		err=pthread_mutex_unlock(&server.mutex);
		/* read what the requested op was: read/write/quit */
		operation=request->operation;
		switch (operation)
		{
			case QUIT_REQUEST:
				break;
			case READ_REQUEST:
				if (strlen(request->prompt)>0)
					printf("--->%s", request->prompt);
				/* strip the newline character */
				if (fgets(request->text, 128, stdin)==0)
					request->text[0]='\0';
				request->text[strlen(request->text)-1]='\0';
				break;
			case WRITE_REQUEST:
				puts(request->text);
				break;
			case DB_SEARCH:
				open("books.txt", O_SEARCH);
				for(int i=0;i<num_bytes;i=i+strlen(title))
				{
					if(){printf("Book was found!\n");}
				}
				printf("Book was not found.\n");
				close(1);
				break;
			case DB_INSERT:
				open("books.txt", O_RDWR);
				if()
				{printf("Book already exists.\n");}
				else{
					puts(request->text);
					printf("Book was not found, hence book was added to end.\n");
					off_t b=(off_t)getBytePosition(row, fd);
					putChar(fd, '\n', b);
				}
				close(1);
				break;
			case DB_DELETE:
				open("books.txt", O_RDWR);
				if()
				{
					ssize_t oldLength=lseek(fd, 1, 0);
					for(ssize_t o=0; o<oldLength;o++){
						putChar(fd, " ", o+1); //Empty out the row.
					}
				}
				else
				{
					printf("Book was not found, hence nothing was deleted.\n");
				}
				close(1);
				break;
			case DB_REPLACE:
				open("books.txt", O_RDWR);
				//Get length of a row that is being replaced to remove all content from it.
				ssize_t oldLength=lseek(fd, 1, 0);
				for(ssize_t o=0; o<oldLength;o++){
					putChar(fd, " ", o+1); //Empty out the row.
				//If new book name and author is shorter than the current book title and
				//author in the file, it wouldn't replace all letters.
				}

				//I chose to do a for loop to replace/insert each char one by one, instead of the 'X'	
				for(ssize_t s=0;s<strlen(title);s++){
					putChar(fd, title[s], b+s+1); //This row was replaced/where a new book has been added
				}
				close(1);
				break;
			default:
				break;
		}
		/* 0=non-synchronous, 1=synchronous */
		if (request->synchronous==1)
		{
			err=pthread_mutex_lock(&server.mutex);
			/* set the done flag to 1, will be the predicate to
			client to synchronize with other */
			request->done_flag=1;
			/* signal that the request is done processing */
			pthread_cond_signal(&request->done);
			err=pthread_mutex_unlock(&server.mutex);
		}
		else
			free(request);
		if (operation==QUIT_REQUEST)
		break; /* break from the while(1) loop */
		}
	return NULL;
}

void *client_routine(void *arg)
{
	printf("In client_routine()...\n");
	int client_number=(intptr_t)arg;
	char prompt[32];
	char string[128], formatted[128];
	int err;
	sprintf(prompt, "Client %d>", client_number);
	//puts(prompt);
	while (1)
	{
		server_request(READ_REQUEST, 1, prompt, string);
		if (strlen(string)==0)
			break;
		sprintf(formatted, "(%d) %s", client_number, string);
		server_request(WRITE_REQUEST, 0, NULL, formatted);
	}
	err=pthread_mutex_lock(&client_mutex);
	client_threads--;
	if (client_threads<=0)
		err=pthread_cond_signal(&clients_done);
	err=pthread_mutex_unlock(&client_mutex);
	printf("Exiting client_routine()...\n");
return NULL;
}

void server_request(int operation, int sync, const char *prompt, char *string)
{
	request_t *request;
	int err;
	/* lock the server data */
	err=pthread_mutex_lock(&server.mutex);
	/* create the server thread */
	if (!server.running)
	{
		pthread_t thread;
		/* server thread is created as a detached thread */
		pthread_attr_t detached_attr;
		err=pthread_attr_init(&detached_attr);
		err=pthread_attr_setdetachstate(&detached_attr, PTHREAD_CREATE_DETACHED);
		server.running=1;
		err=pthread_create(&thread, &detached_attr, server_routine, NULL);
		pthread_attr_destroy(&detached_attr);
	}
	/* create the request */
	request=(request_t *)malloc(sizeof(request_t));
	request->next=NULL; /* this is tail */
	/* these are going to be passed-in arguments */
	request->operation=operation;
	request->synchronous=sync;
	if (sync)
	{
		/* set the done predicate to 0 */
		request->done_flag=0;
		/* initialize the pthread_cond_t variable "done" */
		err=pthread_cond_init(&request->done, NULL);
	}
	/* all requests get a prompt */
	if (prompt!=NULL)
		strncpy(request->prompt, prompt, 32);
	else
		request->prompt[0]='\0';
	/* only the write requests get to write */
	if (operation==WRITE_REQUEST && string!=NULL)
		strncpy(request->text, string, 128);
	else
		request->text[0]='\0';
	/* enqueue the request */
	if (server.first==NULL) /* if queue is empty */
	{
		server.first=request; /* head=tail=request */
		server.last=request;
	}
	else /* if queue is non-empty */
	{
		/* make next of current tail point to request */
		(server.last)->next=request;
		/* make tail equal to request */
		server.last=request;
	}
	/* tell server that there is a request made */
	err=pthread_cond_signal(&server.request);
	/* if request was synchronous, wait for a reply */
	if (sync)
	{
		while (!request->done_flag)
		err=pthread_cond_wait(&request->done,
		&server.mutex);
		if (operation==READ_REQUEST)
			strcpy(string, request->text);
		err=pthread_cond_destroy(&request->done);
		free(request);
	}
	err=pthread_mutex_unlock(&server.mutex);
}

void client_request(int operation, int sync, const char *prompt, char *string)
{
	request_t *request;
	int err;
	/* lock the server data */
	err=pthread_mutex_lock(&server.mutex);
	/* create the server thread */
	if (!server.running)
	{
		pthread_t thread;
		/* server thread is created as a detached thread */
		pthread_attr_t detached_attr;
		err=pthread_attr_init(&detached_attr);
		err=pthread_attr_setdetachstate(&detached_attr, PTHREAD_CREATE_DETACHED);
		server.running=1;
		err=pthread_create(&thread, &detached_attr, server_routine, NULL);
		pthread_attr_destroy(&detached_attr);
	}
	/* create the request */
	request=(request_t *)malloc(sizeof(request_t));
	request->next=NULL; /* this is tail */
	/* these are going to be passed-in arguments */
	request->operation=operation;
	request->synchronous=sync;
	if (sync)
	{
		/* set the done predicate to 0 */
		request->done_flag=0;
		/* initialize the pthread_cond_t variable "done" */
		err=pthread_cond_init(&request->done, NULL);
	}
	/* all requests get a prompt */
	if (prompt!=NULL)
		strncpy(request->prompt, prompt, 32);
	else
		request->prompt[0]='\0';
	/* only the write requests get to write */
	if (operation==WRITE_REQUEST && string!=NULL)
		strncpy(request->text, string, 128);
	else
		request->text[0]='\0';
	/* enqueue the request */
	if (server.first==NULL) /* if queue is empty */
	{
		server.first=request; /* head=tail=request */
		server.last=request;
	}
	else /* if queue is non-empty */
	{
		/* make next of current tail point to request */
		(server.last)->next=request;
		/* make tail equal to request */
		server.last=request;
	}
	/* tell server that there is a request made */
	err=pthread_cond_signal(&server.request);
	/* if request was synchronous, wait for a reply */
	if (sync)
	{
		while (!request->done_flag)
		err=pthread_cond_wait(&request->done, &server.mutex);
		if (operation==READ_REQUEST)
			strcpy(string, request->text);
		err=pthread_cond_destroy(&request->done);
		free(request);
	}
	err=pthread_mutex_unlock(&server.mutex);
}

/*Code involved from HW2 for books.txt*/
/* Returns the byte # at the beginning of specified row # */
int getBytePosition(int row, int filedes)
{
	printf("In getBytePosition()...\n");
	char buf[200];
	int i;

	/* Find the total number of bytes in file */
	ssize_t num_bytes=lseek(filedes, 0, SEEK_END);
	printf("num_bytes=%zd\n", num_bytes);

	/* IMPORTANT! Bring cursor back to BOF */
	lseek(filedes, 0, SEEK_SET);
	ssize_t read_bytes=read(filedes, buf, num_bytes);
	printf("read_bytes=%zd\n", read_bytes);

	int num_rows=1;
	/* Loop goes through the number of bytes in the file */
	for (i=0; i<num_bytes; i++)
	{
		printf("buf[%d]=%c\n", i, buf[i]);
		/* On EOL character, we increment the row # */
		if (buf[i]=='\n')
		{
			printf("***EOL\n");
			num_rows++;
			/* If row # equals row specified, ret. byte # */
			if (num_rows==row)
				return i;
		}
	}
	return -1;
}
/* Writes a character into a file at specified byte */
void putChar(int filedes, char c, off_t byte)
{
	lseek(filedes, byte, SEEK_SET);
	/* The write starts at the current offset */
	write(filedes, &c, 1);
}
