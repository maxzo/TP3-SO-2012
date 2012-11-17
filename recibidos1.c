#include <signal.h>
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

void despertar(int sig) { }

int main()
{
	signal(SIGUSR1, &despertar);
	
	int msqid_ordinaria, msqid1, msqid2, msqid3;
	//int msgflg = IPC_CREAT | 0666;
	key_t key_ordinaria, key1, key2, key3;
	message_buf rbuf_ordinaria, rbuf1, rbuf2, sbuf3;
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
	
	printf("Mi PID = %d\nPID pedidos.c = %d\nPID recibidos.c = %d\n\n", getpid(), pid_pedidos, pid_recibidos);
	
	/**------------------------------------------------------------------------
	 * Recepción de mensajes
	 * ------------------------------------------------------------------------
	 */
	
	//signal(SIGUSR1, &despertar);
	
	key_ordinaria = 1010;
	if ((msqid_ordinaria = msgget(key_ordinaria, 0666)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	printf("Agenda ordinaria:\n");
	
	while (1)
	{
		pause();
		
		if (msgrcv(msqid_ordinaria, &rbuf_ordinaria, MSG_SIZE, 0, 0) > 0)
		{
			if (rbuf_ordinaria.mtype == 1 || rbuf_ordinaria.mtype == 2)
			{
				printf("%s\n", rbuf_ordinaria.mtext);
			}
		}
		
		kill(pid_pedidos, SIGUSR1);
	}
	
	return 0;
}
