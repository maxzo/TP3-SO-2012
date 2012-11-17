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
	
	int msqid_prioritaria, msqid_ordinaria, msqid1, msqid2, msqid3;
	int msgflg = IPC_CREAT | 0666;
	key_t key_prioritaria, key_ordinaria, key1, key2, key3;
	message_buf sbuf_prioritaria, sbuf_ordinaria, sbuf1, rbuf2, rbuf3;
	size_t buf_length;
	
	int pid_recibidos, pid_recibidos1;
	
	/**
	 * Limpieza de colas de mensajes que pudieron quedar
	 * en memoria de ejecuciones anteriores interrumpidas
	 */
	msgctl(msgget(1000, msgflg), IPC_RMID, NULL);
	msgctl(msgget(2000, msgflg), IPC_RMID, NULL);
	msgctl(msgget(3000, msgflg), IPC_RMID, NULL);
	msgctl(1010, IPC_RMID, NULL);
	msgctl(2020, IPC_RMID, NULL);
	
	key_prioritaria = 2020;
	if ((msqid_prioritaria = msgget(key_prioritaria, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key_ordinaria = 1010;
	if ((msqid_ordinaria = msgget(key_ordinaria, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
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
	
	printf("Mi PID = %d\nPID recibidos.c = %d\nPID recibidos1.c = %d\n\n", getpid(), pid_recibidos, pid_recibidos1);
	
	/**------------------------------------------------------------------------
	 * Envío de mensajes
	 * ------------------------------------------------------------------------
	 */
	
	sbuf_ordinaria.mtype = 2;
	sprintf(sbuf_ordinaria.mtext, "Requerimiento de mantenimiento de sala");
	buf_length = strlen(sbuf_ordinaria.mtext) + 1;
	
	if (msgsnd(msqid_ordinaria, &sbuf_ordinaria, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	else
	{
		printf("Mensaje: \"%s\" enviado!\n", sbuf_ordinaria.mtext);
	}
	
	kill(pid_recibidos1, SIGUSR1);
	pause();
	
	sbuf_ordinaria.mtype = 2;
	sprintf(sbuf_ordinaria.mtext, "Requerimiento de mantenimiento de sala");
	buf_length = strlen(sbuf_ordinaria.mtext) + 1;
	
	if (msgsnd(msqid_ordinaria, &sbuf_ordinaria, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	else
	{
		printf("Mensaje: \"%s\" enviado!\n", sbuf_ordinaria.mtext);
	}
	
	kill(pid_recibidos1, SIGUSR1);
	pause();
	
	msgctl(msqid1, IPC_RMID, NULL);
	msgctl(msqid2, IPC_RMID, NULL);
	msgctl(msqid3, IPC_RMID, NULL);
	msgctl(msqid_ordinaria, IPC_RMID, NULL);
	msgctl(msqid_prioritaria, IPC_RMID, NULL);
	
	return 0;
}
