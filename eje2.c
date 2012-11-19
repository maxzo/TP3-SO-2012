#include <errno.h>
//#include <linux/ipc.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/shm.h>
#include <sys/types.h>
#include "semaforo.c"

#define MUTEX 0
#define VACIAS 1
#define LLENAS 2

#define N 3
#define SHM_SIZE 15

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
	
	key_t shm_key;
	int shmid;
	char* shmem;
	int i;
	
	if ((shm_key = ftok(".", 'x')) == -1)
	{
		perror("ftok");
		exit(1);
	}
    
    if ((shmid = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0660)) == -1)
	{
        perror("shmget");
        exit(1);
    }
    
    if ((shmem = shmat(shmid, 0, 0)) == (void *) -1)
	{
        perror("shmat");
        exit(1);
    }
    
    for (i = 0; i < SHM_SIZE; i++)
    {
		shmem[i] = ' ';
	}
	
	char* comando;
		
	if (fork() == 0)
	{
		comando = "./productor";
		execve(comando, argv, envp);
		printf("Falla productor\n");
	}
	else if (fork() == 0)
	{
		comando = "./locutor";
		execve(comando, argv, envp);
		printf("Falla locutor\n");
	}
	/*else
	{
		wait();
		printf("Terminaron los hijos\n");
		
		union semun arg;
		
		if (semctl(semid, 0, IPC_RMID, arg) == -1)
		{
			perror("semctl");
			exit(1);
		}
		
		shmdt(shmem);
	}*/
	
	return 0;
}
