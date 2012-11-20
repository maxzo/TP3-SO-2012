#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <linux/ipc.h>
#include <linux/sem.h>
#include "semaforo.c"

#define SEGSIZE 1

void crear_o_enlazar();

char* shmem;
int shmid;

int main()
{
	crear_o_enlazar();
	
	printf("Presione ENTER para continuar ...\n");
	getchar();
	shmctl(shmid, IPC_RMID, 0);
	
	return 0;
}

void crear_o_enlazar()
{
	key_t key;
	int shmem_creada = 1;
	
	key = ftok(".", 'x');
	
	if((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1)
	{
		if((shmid = shmget(key, SEGSIZE, 0)) == -1)
		{
		  perror("shmget");
		  exit(1);
		}
		shmem_creada = 0;
	}
	
	if((shmem = shmat(shmid, 0, 0)) == -1)
	{
	  perror("shmat");
	  exit(1);
	}
	
	if (shmem_creada)
	{
		shmem[0] = '3';
		printf("He creado el segmento\n");
	}
	else
	{
		printf("He enlazado el segmento\n");
	}
	
	printf("shmem = %c\n", shmem[0]);
}
