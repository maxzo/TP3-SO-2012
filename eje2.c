#include <errno.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "semaforo.c"

#define MUTEX 0
#define VACIAS 1
#define LLENAS 2

#define N 3
#define SHM_SIZE 6

void crear_shmem();

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
	
	crear_shmem();
	//return 0;
	
	char* comando;
		
	if (fork() == 0)
	{
		comando = "./productor";
		execve(comando, argv, envp);
		printf("Falla productor\n");
	}
	else
	{
		comando = "./locutor";
		execve(comando, argv, envp);
		printf("Falla locutor\n");
		
		wait(NULL);
	}
	
	return 0;
}

void crear_shmem()
{
	key_t shm_key1, shm_key2, shm_key3;
	int shmid1, shmid2, shmid3;
	char* shmem1, * shmem2, * shmem3;
	
	if ((shm_key1 = ftok(".", 'x')) == -1)
	{
		perror("ftok");
		exit(1);
	}
	
	if ((shm_key2 = ftok(".", 'y')) == -1)
	{
		perror("ftok");
		exit(1);
	}
	
	if ((shm_key3 = ftok(".", 'z')) == -1)
	{
		perror("ftok");
		exit(1);
	}
	
	if((shmid1 = shmget(shm_key1, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1)
	{
		if((shmid1 = shmget(shm_key1, SHM_SIZE, 0)) == -1)
		{
		  perror("shmget");
		  exit(1);
		}
	}
    
    if((shmid2 = shmget(shm_key2, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1)
	{
		if((shmid2 = shmget(shm_key2, SHM_SIZE, 0)) == -1)
		{
		  perror("shmget");
		  exit(1);
		}
	}
    
    if((shmid3 = shmget(shm_key3, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1)
	{
		if((shmid3 = shmget(shm_key3, SHM_SIZE, 0)) == -1)
		{
		  perror("shmget");
		  exit(1);
		}
	}
    
    if ((shmem1 = (char *) shmat(shmid1, 0, 0)) == (void *) -1)
	{
        perror("shmat");
        exit(1);
    }
    
    if ((shmem2 = (char *) shmat(shmid2, 0, 0)) == (void *) -1)
	{
        perror("shmat");
        exit(1);
    }
    
    if ((shmem3 = (char *) shmat(shmid3, 0, 0)) == (void *) -1)
	{
        perror("shmat");
        exit(1);
    }
    
    int i;
    const char* vacio = "     ";
    
    strcpy(shmem1, vacio);
    strcpy(shmem2, vacio);
    strcpy(shmem3, vacio);
    
    /*shmctl(shmid1, IPC_RMID, 0);
	shmctl(shmid2, IPC_RMID, 0);
	shmctl(shmid3, IPC_RMID, 0);*/
}
