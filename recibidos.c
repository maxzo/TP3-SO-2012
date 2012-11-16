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
	int msqid, msqid102, msqid201;
	//int msgflg = IPC_CREAT | 0666;
	key_t key, key102, key201;
	message_buf sbuf, rbuf102, sbuf201;
	size_t buf_length;
	
	int pid_pedidos, pid_recibidos1;
	
	key201 = 2001;
	if ((msqid201 = msgget(key201, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key102 = 1002;
	if ((msqid102 = msgget(key102, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	sbuf201.mtype = 201;
	sprintf(sbuf201.mtext, "%d", getpid());
	buf_length = strlen(sbuf201.mtext) + 1 ;
	
	int mensaje_recibido;
	
	do
	{
		if (msgsnd(msqid201, &sbuf201, buf_length, IPC_NOWAIT) < 0)
		{
			perror("msgsnd");
			exit(1);
		}
	}
	while ((mensaje_recibido = msgrcv(msqid102, &rbuf102, MSG_SIZE, 102, 0)) < 1);
	
	pid_pedidos = atoi(rbuf102.mtext);
	
	printf("Mi PID = %d\nPID pedidos.c = %d\n", getpid(), pid_pedidos);
	
	return 0;
}

