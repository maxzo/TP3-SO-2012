#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void main(int argc, char* argv[], char* envp[])
{
	key_t key;
	int semid;
	union semun arg;
	
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
	
	// Semaforo 0: mutex
	arg.val = 1;
	if (semctl(semid, 0, SETVAL, arg) == -1)
	{
	  perror("semctl");
	  exit(1);
	}
	
	// Semaforo 1: vacias
	arg.val = 3;
	if (semctl(semid, 1, SETVAL, arg) == -1)
	{
	  perror("semctl");
	  exit(1);
	}
	
	// Semaforo 2: llenas
	arg.val = 0;
	if (semctl(semid, 2, SETVAL, arg) == -1)
	{
	  perror("semctl");
	  exit(1);
	}

	char* comando;
	
	if (fork() == 0)
	{
		comando = "./locutor";
		execve(comando, argv, envp);
	}
	else
	{
		comando = "./productor";
		execve(comando, argv, envp);
	}
	
	wait();
}
