#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSG_SIZE 138

typedef struct msgbuf
{
       long    mtype;
       char    mtext[MSG_SIZE];
} message_buf;

int main()
{
	int msqid_ordinaria, msqid_recibidos1;
	key_t key_ordinaria, key_recibidos1;
	message_buf rbuf_ordinaria, sbuf_recibidos1;
	size_t buf_length;
	
	key_ordinaria = 1010;
	if ((msqid_ordinaria = msgget(key_ordinaria, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key_recibidos1 = 1011;
	if ((msqid_recibidos1 = msgget(key_recibidos1, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	printf("Agenda ordinaria:\n");
	
	sbuf_recibidos1.mtype = 11;
	sprintf(sbuf_recibidos1.mtext, "");
	buf_length = strlen(sbuf_recibidos1.mtext) + 1;
	
	msgsnd(msqid_recibidos1, &sbuf_recibidos1, buf_length, IPC_NOWAIT);
	
	while (1)
	{
		if (msgrcv(msqid_ordinaria, &rbuf_ordinaria, MSG_SIZE, 0, 0) > 0)
		{
			if (rbuf_ordinaria.mtype == 1 || rbuf_ordinaria.mtype == 2)
			{
				sbuf_recibidos1.mtype = 11;
				sprintf(sbuf_recibidos1.mtext, "");
				buf_length = strlen(sbuf_recibidos1.mtext) + 1;
				
				if (msgsnd(msqid_recibidos1, &sbuf_recibidos1, buf_length, IPC_NOWAIT) < 0)
				{
					perror("msgsnd");
					exit(1);
				}
				
				printf("%s\n", rbuf_ordinaria.mtext);
			}
		}
	}
	
	return 0;
}
