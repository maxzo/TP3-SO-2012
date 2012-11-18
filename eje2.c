#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <linux/ipc.h>
#include <linux/sem.h>
#include "semaforo.c"

#define MUTEX 0
#define VACIAS 1
#define LLENAS 2

#define N 3

int main(int argc, char* argv[], char* envp[])
{
	key_t key;
	int semid;
	
	if ((key = ftok(".", 'a')) == -1)
	{
	  perror("ftok");
	  exit(1);
	}
	
	if ((semid = semget(key, 3, 0666 | IPC_CREAT)) == -1)
	{
	  perror("semget");
	  exit(1);
	}
	
	inicializar_sem(semid, MUTEX, 1);
	inicializar_sem(semid, VACIAS, N);
	inicializar_sem(semid, LLENAS, 0);
	
	char* comando;
		
	if (fork() == 0)
	{
		comando = "./locutor";
		execve(comando, argv, envp);
		printf("Falla locutor\n");
	}
	else if (fork() == 0)
	{
		comando = "./productor";
		execve(comando, argv, envp);
		printf("Falla productor\n");
	}
	else
	{
		wait();
		
		// TODO: remover semáforos
	}
	
	return 0;
}
