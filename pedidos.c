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
	int msqid, msqid102, msqid103, msqid201, msqid301;
	int msgflg = IPC_CREAT | 0666;
	key_t key, key102, key103, key201, key301;
	message_buf sbuf, sbuf102, sbuf103, rbuf201, rbuf301;
	size_t buf_length;
	
	int pid_recibidos, pid_recibidos1;
	
	key102 = 1002;
	if ((msqid102 = msgget(key102, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key201 = 2001;
	if ((msqid201 = msgget(key201, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	sbuf102.mtype = 102;
	sprintf(sbuf102.mtext, "%d", getpid());
	buf_length = strlen(sbuf102.mtext) + 1 ;
	
	int mensaje_recibido;
	
	do
	{
		if (msgsnd(msqid102, &sbuf102, buf_length, IPC_NOWAIT) < 0)
		{
			perror("msgsnd");
			exit(1);
		}
	}
	while ((mensaje_recibido = msgrcv(msqid201, &rbuf201, MSG_SIZE, 201, 0)) < 1);
	
	pid_recibidos = atoi(rbuf201.mtext);
	
	printf("Mi PID = %d\nPID recibidos.c = %d\n", getpid(), pid_recibidos);
	
	return 0;
}

