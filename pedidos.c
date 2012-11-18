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
	int msqid_prioritaria, msqid_ordinaria, msqid_recibidos, msqid_recibidos1;
	int msgflg = IPC_CREAT | 0666;
	key_t key_prioritaria, key_ordinaria, key_recibidos, key_recibidos1;
	message_buf sbuf_prioritaria, sbuf_ordinaria, rbuf_recibidos, rbuf_recibidos1;
	size_t buf_length;
	int i;
	
	/**
	 * Limpieza de colas de mensajes que pudieron quedar
	 * en memoria de ejecuciones anteriores interrumpidas
	 */
	msgctl(msgget(1010, msgflg), IPC_RMID, NULL);
	msgctl(msgget(2020, msgflg), IPC_RMID, NULL);
	msgctl(msgget(1011, msgflg), IPC_RMID, NULL);
	msgctl(msgget(2022, msgflg), IPC_RMID, NULL);
	
	
	/**
	 * Creación de las colas de mensajes
	 */
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
	
	key_recibidos = 2022;
	if ((msqid_recibidos = msgget(key_recibidos, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	key_recibidos1 = 1011;
	if ((msqid_recibidos1 = msgget(key_recibidos1, msgflg)) < 0)
	{
		perror("msgget");
		exit(1);
	}
	
	/**
	 * 2 mensajes de mantenimiento
	 */
	for (i = 0; i < 2; i++)
	{
		sbuf_ordinaria.mtype = 2;
		sprintf(sbuf_ordinaria.mtext, "Requerimiento de mantenimiento de sala");
		buf_length = strlen(sbuf_ordinaria.mtext) + 1;
		
		while (msgrcv(msqid_recibidos1, &rbuf_recibidos1, MSG_SIZE, 11, 0) < 0);
		
		if (msgsnd(msqid_ordinaria, &sbuf_ordinaria, buf_length, IPC_NOWAIT) < 0)
		{
			perror("msgsnd");
			exit(1);
		}
		else
		{
			printf("Mensaje: \"%s\" enviado!\n", sbuf_ordinaria.mtext);
		}
	}
	
	/**
	 * 4 mensajes de conferencias
	 */
	for (i = 0; i < 4; i++)
	{
		sbuf_ordinaria.mtype = 1;
		sprintf(sbuf_ordinaria.mtext, "Solicitud de sala para videoconferencia (Conf. operativa)");
		buf_length = strlen(sbuf_ordinaria.mtext) + 1;
		
		while (msgrcv(msqid_recibidos1, &rbuf_recibidos1, MSG_SIZE, 11, 0) < 0);
		
		if (msgsnd(msqid_ordinaria, &sbuf_ordinaria, buf_length, IPC_NOWAIT) < 0)
		{
			perror("msgsnd");
			exit(1);
		}
		else
		{
			printf("Mensaje: \"%s\" enviado!\n", sbuf_ordinaria.mtext);
		}
	}
	
	/**
	 * 2 mensajes de mantenimiento
	 */
	for (i = 0; i < 2; i++)
	{
		sbuf_ordinaria.mtype = 2;
		sprintf(sbuf_ordinaria.mtext, "Requerimiento de mantenimiento de sala");
		buf_length = strlen(sbuf_ordinaria.mtext) + 1;
		
		while (msgrcv(msqid_recibidos1, &rbuf_recibidos1, MSG_SIZE, 11, 0) < 0);
		
		if (msgsnd(msqid_ordinaria, &sbuf_ordinaria, buf_length, IPC_NOWAIT) < 0)
		{
			perror("msgsnd");
			exit(1);
		}
		else
		{
			printf("Mensaje: \"%s\" enviado!\n", sbuf_ordinaria.mtext);
		}
	}
	
	/**
	 * 2 mensajes de conferencias gerenciales
	 */
	for (i = 0; i < 2; i++)
	{
		sbuf_prioritaria.mtype = 3;
		sprintf(sbuf_prioritaria.mtext, "Gerencia requiere sala para videoconferencia");
		buf_length = strlen(sbuf_prioritaria.mtext) + 1;
		
		while (msgrcv(msqid_recibidos, &rbuf_recibidos, MSG_SIZE, 22, 0) < 0);
		
		if (msgsnd(msqid_prioritaria, &sbuf_prioritaria, buf_length, IPC_NOWAIT) < 0)
		{
			perror("msgsnd");
			exit(1);
		}
		else
		{
			printf("Mensaje: \"%s\" enviado!\n", sbuf_prioritaria.mtext);
		}
	}
	
	/**
	 * Mensaje final prioritarios
	 */
	sbuf_prioritaria.mtype = 3;
	sprintf(sbuf_prioritaria.mtext, "Fin prioritarios");
	buf_length = strlen(sbuf_prioritaria.mtext) + 1;
	
	while (msgrcv(msqid_recibidos, &rbuf_recibidos, MSG_SIZE, 22, 0) < 0);
	
	if (msgsnd(msqid_prioritaria, &sbuf_prioritaria, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	else
	{
		printf("Mensaje: \"%s\" enviado!\n", sbuf_prioritaria.mtext);
	}
	
	
	/**
	 * Mensaje final no prioritarios
	 */
	
	sbuf_ordinaria.mtype = 1;
	sprintf(sbuf_ordinaria.mtext, "Fin no prioritarios");
	buf_length = strlen(sbuf_ordinaria.mtext) + 1;
	
	while (msgrcv(msqid_recibidos1, &rbuf_recibidos1, MSG_SIZE, 11, 0) < 0);
	
	if (msgsnd(msqid_ordinaria, &sbuf_ordinaria, buf_length, IPC_NOWAIT) < 0)
	{
		perror("msgsnd");
		exit(1);
	}
	else
	{
		printf("Mensaje: \"%s\" enviado!\n", sbuf_ordinaria.mtext);
	}
	
	/**
	 * Esto es para evitar que se borren las colas
	 * de mensajes antes de que sean utilizadas
	 */
	printf("\nPresione ENTER para continuar . . . ");
	getchar();
	
	msgctl(msqid_ordinaria, IPC_RMID, NULL);
	msgctl(msqid_prioritaria, IPC_RMID, NULL);
	msgctl(msqid_recibidos, IPC_RMID, NULL);
	msgctl(msqid_recibidos1, IPC_RMID, NULL);
	
	return 0;
}
