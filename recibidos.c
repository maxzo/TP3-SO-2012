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
	int msqid_prioritaria, msqid_recibidos;
	key_t key_prioritaria, key_recibidos;
	message_buf rbuf_prioritaria, sbuf_recibidos;
	size_t buf_length;
	
	key_prioritaria = 2020;
	if ((msqid_prioritaria = msgget(key_prioritaria, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key_recibidos = 2022;
	if ((msqid_recibidos = msgget(key_recibidos, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	printf("Agenda prioritaria:\n");
	
	sbuf_recibidos.mtype = 22;
	sprintf(sbuf_recibidos.mtext, "");
	buf_length = strlen(sbuf_recibidos.mtext) + 1;
	
	msgsnd(msqid_recibidos, &sbuf_recibidos, buf_length, IPC_NOWAIT);
	
	while (1)
	{
		if (msgrcv(msqid_prioritaria, &rbuf_prioritaria, MSG_SIZE, 3, 0) > 0)
		{
			sbuf_recibidos.mtype = 22;
			sprintf(sbuf_recibidos.mtext, "");
			buf_length = strlen(sbuf_recibidos.mtext) + 1;
			
			if (msgsnd(msqid_recibidos, &sbuf_recibidos, buf_length, IPC_NOWAIT) < 0)
			{
				perror("msgsnd");
				exit(1);
			}
			
			printf("%s\n", rbuf_prioritaria.mtext);
		}
	}
	
	return 0;
}
