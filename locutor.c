#include <errno.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "semaforo.c"

#define MUTEX 0
#define VACIAS 1
#define LLENAS 2

#define A 9 // Cantidad de anuncios
#define SHM_SIZE 6

char* extraer_anuncio();
void publicar_anuncio(char* anuncio);

void enlazar_shmem();
int esta_vacio(char* str);

char* shmem1, * shmem2, * shmem3; // Memoria compartida
int shmid1, shmid2, shmid3;

int main(int argc, char* argv[], char* envp[])
{
	key_t key;
	int semid;
	
	if ((key = ftok(".", 'a')) == -1)
	{
		perror("ftok");
		exit(1);
	}
	
	if ((semid = semget(key, 3, 0)) == -1)
	{
		perror("semget");
		exit(1);
	}
	
	char* anuncio;
	const char* nulo = "";
	int cant_anunciados = 0;
	
	enlazar_shmem();
	
	sleep(2);
	
	while (cant_anunciados < A)
	{
		down_sem(semid, LLENAS);
		down_sem(semid, MUTEX);
		anuncio = extraer_anuncio();
		up_sem(semid, MUTEX);
		up_sem(semid, VACIAS);
		if (strcmp(anuncio, nulo) != 0)
		{
			publicar_anuncio(anuncio);
			cant_anunciados++;
		}
	}
	
	shmctl(shmid1, IPC_RMID, 0);
	shmctl(shmid2, IPC_RMID, 0);
	shmctl(shmid3, IPC_RMID, 0);
	
	union semun arg;
	
	if (semctl(semid, 0, IPC_RMID, arg) == -1)
	{
		perror("semctl");
		exit(1);
	}
	
	return 0;
}

char* extraer_anuncio()
{
	char anuncio[SHM_SIZE];
	const char* nulo = "";
	const char* vacio = "     ";
	
	if (!esta_vacio(shmem3))
	{
		strcpy(anuncio, shmem3);
		strcpy(shmem3, vacio);
	}
	else if (!esta_vacio(shmem2))
	{
		strcpy(anuncio, shmem2);
		strcpy(shmem2, vacio);
	}
	else if (!esta_vacio(shmem1))
	{
		strcpy(anuncio, shmem1);
		strcpy(shmem1, vacio);
	}
	else
	{
		strcpy(anuncio, nulo);
	}
	
	return (char *) anuncio;
}

void publicar_anuncio(char* anuncio)
{
	printf("Anuncio pasado: %c%c%c%c%c ...\n", anuncio[0], anuncio[1], anuncio[2], anuncio[3], anuncio[4]);
}

int esta_vacio(char* str)
{
	const char* vacio = "     ";
	
	if (strcmp(str, vacio) == 0) return 1;
	
	return 0;
}

void enlazar_shmem()
{
	key_t shm_key1, shm_key2, shm_key3;
	
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
	
	if ((shmid1 = shmget(shm_key1, SHM_SIZE, 0666)) == -1)
	{
        perror("shmget");
        exit(1);
    }
    
    if ((shmid2 = shmget(shm_key2, SHM_SIZE, 0666)) == -1)
	{
        perror("shmget");
        exit(1);
    }
    
    if ((shmid3 = shmget(shm_key3, SHM_SIZE, 0666)) == -1)
	{
        perror("shmget");
        exit(1);
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
}
