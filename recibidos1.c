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
	int msqid, msqid1, msqid2, msqid3;
	//int msgflg = IPC_CREAT | 0666;
	key_t key, key1, key2, key3;
	message_buf sbuf, rbuf1, rbuf2, sbuf3;
	size_t buf_length;
	
	int pid_pedidos, pid_recibidos;
	
	/**------------------------------------------------------------------------
	 * Configuración de la conexión con los otros procesos
	 * ------------------------------------------------------------------------
	 */
	
	key1 = 1000;
	if ((msqid1 = msgget(key1, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key2 = 2000;
	if ((msqid2 = msgget(key2, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key3 = 3000;
	if ((msqid3 = msgget(key3, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	sbuf3.mtype = 300;
	sprintf(sbuf3.mtext, "%d", getpid());
	buf_length = strlen(sbuf3.mtext) + 1;
	
	if (msgsnd(msqid3, &sbuf3, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	
	if (msgsnd(msqid3, &sbuf3, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	
	while (msgrcv(msqid1, &rbuf1, MSG_SIZE, 100, 0) < 1);
	
	pid_pedidos = atoi(rbuf1.mtext);
	
	while (msgrcv(msqid2, &rbuf2, MSG_SIZE, 200, 0) < 1);
	
	pid_recibidos = atoi(rbuf2.mtext);
	
	printf("Mi PID = %d\nPID pedidos.c = %d\nPID recibidos.c = %d\n", getpid(), pid_pedidos, pid_recibidos);
	
	return 0;
}
