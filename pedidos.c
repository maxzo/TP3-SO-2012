#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_SIZE 128

typedef struct msgbuf
{
       long    mtype;
       char    mtext[MSG_SIZE];
} message_buf;

int main()
{
     	int msqid;
	int msgflg = IPC_CREAT | 0666;
	key_t key;
	message_buf sbuf;
	size_t buf_length;
	
	key = 1111;
	if ((msqid = msgget(key, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	sbuf.mtype = 1;
	strcpy(sbuf.mtext, getpid());
	buf_length = strlen(sbuf.mtext) + 1 ;
	
	while()
	{
		if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0)
		{
			perror("msgsnd");
			exit(1);
		}
	}

	return 0;
}

