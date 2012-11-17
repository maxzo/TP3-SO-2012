#include <signal.h>
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

void despertar(int sig) { }

int main()
{
	signal(SIGUSR1, &despertar);
	
	int msqid_prioritaria, msqid1, msqid2, msqid3;
	//int msgflg = IPC_CREAT | 0666;
	key_t key_prioritaria, key1, key2, key3;
	message_buf rbuf_prioritaria, rbuf1, sbuf2, rbuf3;
	size_t buf_length;
	
	int pid_pedidos, pid_recibidos1;
	
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
	
	sbuf2.mtype = 200;
	sprintf(sbuf2.mtext, "%d", getpid());
	buf_length = strlen(sbuf2.mtext) + 1;
	
	if (msgsnd(msqid2, &sbuf2, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	
	if (msgsnd(msqid2, &sbuf2, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	
	while (msgrcv(msqid1, &rbuf1, MSG_SIZE, 100, 0) < 1);
	
	pid_pedidos = atoi(rbuf1.mtext);
	
	while (msgrcv(msqid3, &rbuf3, MSG_SIZE, 300, 0) < 1);
	
	pid_recibidos1 = atoi(rbuf3.mtext);
	
	printf("Mi PID = %d\nPID pedidos.c = %d\nPID recibidos1.c = %d\n\n", getpid(), pid_pedidos, pid_recibidos1);
	
	/**------------------------------------------------------------------------
	 * Recepción de mensajes
	 * ------------------------------------------------------------------------
	 */
	
	//signal(SIGUSR1, &despertar);
	
	key_prioritaria = 2020;
	if ((msqid_prioritaria = msgget(key_prioritaria, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	printf("Agenda prioritaria:\n");
	
	while (1)
	{
		pause();
		
		if (msgrcv(msqid_prioritaria, &rbuf_prioritaria, MSG_SIZE, 3, 0) > 0)
		{
			printf("%s\n", rbuf_prioritaria.mtext);
		}
		
		kill(pid_pedidos, SIGUSR1);
	}
	
	return 0;
}
