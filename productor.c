#include <errno.h>
//#include <linux/ipc.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include "semaforo.c"

#define MUTEX 0
#define VACIAS 1
#define LLENAS 2

#define A 9 // Cantidad de anuncios
#define A_SIZE 5 // Tamaño del anuncio
#define SHM_SIZE 15

char* producir_anuncio();
void insertar_anuncio(char* anuncio);

int esta_vacio(char* str, int tamanho);

char* anuncios[A] = { "jabon", "autos", "motos", "arroz", "pollo", "papel", "lapiz", "sopas", "jeans" };
int anunciado[A];

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
	int i;
	
	for (i = 0; i < A; i++)
	{
		anunciado[i] = 0;
	}
	
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
		anuncio = producir_anuncio();
		down_sem(semid, VACIAS);
		down_sem(semid, MUTEX);
		insertar_anuncio(anuncio);
		up_sem(semid, MUTEX);
		up_sem(semid, LLENAS);
	}
	
	return 0;
}

char* producir_anuncio()
{
	//char* anuncio;
	int i;
	
	do
	{
		srand(time(NULL));
		i = rand() % A;
	}
	while (anunciado[i]);
	
	anunciado[i] = 1;
	
	printf("Anunciar: %s ...\n", anuncios[i]);
	
	return anuncios[i];
}

void insertar_anuncio(char* anuncio)
{
	int i, j;
    
    if (esta_vacio(shmem, A_SIZE))
    {
		for (i = 0; i < A_SIZE; i++)
		{
			shmem[i] = anuncio[i];
		}
	}
	else if (esta_vacio(&shmem[A_SIZE], A_SIZE))
	{
		j = A_SIZE;
		
		for (i = 0; i < A_SIZE; i++)
		{
			shmem[j++] = anuncio[i];
		}
	}
	else if (esta_vacio(&shmem[A_SIZE * 2], A_SIZE))
	{
		j = A_SIZE * 2;
		
		for (i = 0; i < A_SIZE; i++)
		{
			shmem[j++] = anuncio[i];
		}
	}
	else
	{
		printf("Error: memoria compartida llena\n");
	}
	/*int z;
	printf("shmem = ");
	for (z = 0; z < 15; z++) printf("%c", shmem[z] == ' ' ? '*' : shmem[z]);
	printf("\n");*/
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
