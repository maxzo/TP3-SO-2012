#include <errno.h>
//#include <linux/ipc.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include "semaforo.c"

#define MUTEX 0
#define VACIAS 1
#define LLENAS 2

#define A_SIZE 5 // Tamaño del anuncio
#define SHM_SIZE 15

char* extraer_anuncio();
void publicar_anuncio(char* anuncio);

int esta_vacio(char* str, int tamanho);

char* shmem; // Memoria compartida

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
	
	key_t shm_key;
	int shmid;
	
	if ((shm_key = ftok(".", 'x')) == -1)
	{
		perror("ftok");
		exit(1);
	}
	
	if ((shmid = shmget(shm_key, SHM_SIZE, 0660)) == -1)
	{
        perror("shmget");
        exit(1);
    }
	
	if ((shmem = shmat(shmid, 0, 0)) == (void *) -1)
	{
        perror("shmat");
        exit(1);
    }
	
	while (1)
	{
		down_sem(semid, LLENAS);
		down_sem(semid, MUTEX);
		anuncio = extraer_anuncio();
		up_sem(semid, MUTEX);
		up_sem(semid, VACIAS);
		publicar_anuncio(anuncio);
	}
	
	return 0;
}

char* extraer_anuncio()
{
	char* anuncio;
	
	int i, j, k;
	
	do
	{
		srand(time(NULL));
		i = rand() % 3;
	}
	while (esta_vacio(shmem + A_SIZE * i, A_SIZE));
	
	k = A_SIZE * i;
	
	for (j = 0; j < A_SIZE; j++)
	{
		anuncio[j] = shmem[k++];
		shmem[k-1] = ' ';
	}
	anuncio[i] = '\0';
	
	return anuncio;
}

void publicar_anuncio(char* anuncio)
{
	printf("Anuncio pasado: %s ...\n", anuncio);
}

int esta_vacio(char* str, int tamanho)
{
	int i;
	
	for (i = 0; i < tamanho; i++)
	{
		if (str[i] != ' ')
		{
			return 0;
		}
	}
	
	return 1;
}
