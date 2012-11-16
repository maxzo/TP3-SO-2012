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
	int msqid, msqid1, msqid2, msqid3;
	int msgflg = IPC_CREAT | 0666;
	key_t key, key1, key2, key3;
	message_buf sbuf, sbuf1, rbuf2, rbuf3;
	size_t buf_length;
	
	int pid_recibidos, pid_recibidos1;
	
	/**------------------------------------------------------------------------
	 * Configuración de la conexión con los otros procesos
	 * ------------------------------------------------------------------------
	 */
	
	key1 = 1000;
	if ((msqid1 = msgget(key1, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key2 = 2000;
	if ((msqid2 = msgget(key2, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key3 = 3000;
	if ((msqid3 = msgget(key3, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	sbuf1.mtype = 100;
	sprintf(sbuf1.mtext, "%d", getpid());
	buf_length = strlen(sbuf1.mtext) + 1;
	
	if (msgsnd(msqid1, &sbuf1, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	
	if (msgsnd(msqid1, &sbuf1, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	
	while (msgrcv(msqid2, &rbuf2, MSG_SIZE, 200, 0) < 1);
	
	pid_recibidos = atoi(rbuf2.mtext);
	
	while (msgrcv(msqid3, &rbuf3, MSG_SIZE, 300, 0) < 1);
	
	pid_recibidos1 = atoi(rbuf3.mtext);
	
	printf("Mi PID = %d\nPID recibidos.c = %d\nPID recibidos1.c = %d\n", getpid(), pid_recibidos, pid_recibidos1);
	
	getchar();
	
	msgctl(msqid1, IPC_RMID, NULL);
	msgctl(msqid2, IPC_RMID, NULL);
	msgctl(msqid3, IPC_RMID, NULL);
	
	return 0;
}
