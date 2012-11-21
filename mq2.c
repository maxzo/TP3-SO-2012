#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/ipc.h>
#include <linux/sem.h>
#include "semaforo.c"

#define MUTEX 0

#define N 1
#define SEGSIZE 1

void crear_o_enlazar();

char* shmem;

int main()
{
	crear_o_enlazar();
	
	key_t key;
	int semid;
	
	if ((key = ftok(".", 'f')) == -1)
	{
		perror("ftok");
		exit(1);
	}
	
	if ((semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666)) == -1)
	{
		if ((semid = semget(key, 1, 0)) == -1)
		{
			perror("semget");
			exit(1);
		}
	}
	else
	{
		inicializar_sem(semid, MUTEX, 1);
	}
	
	int botellas = 0;
	
	while (1)
	{
		down_sem(semid, MUTEX);
		
		if (shmem[0] == '1')
		{
			botellas++;
			
			printf("Entra botella vacia: %d\n", botellas);
			
			shmem[0] = '2';
			
			printf("Sale botella llena: %d\n", botellas);
			sleep(1);
		}
		
		up_sem(semid, MUTEX);
	}
	
	return 0;
}

void crear_o_enlazar()
{
	key_t key;
	int shmid;
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
	
	if((shmem = (char *) shmat(shmid, 0, 0)) == (void *) -1)
	{
	  perror("shmat");
	  exit(1);
	}
	
	if (shmem_creada)
	{
		shmem[0] = '3';
	}
}
