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

int producir_anuncio();
int insertar_anuncio(char* anuncio);

void enlazar_shmem();
int esta_vacio(char* str);

char* anuncios[A] = { "jabon", "autos", "motos", "arroz", "pollo", "papel", "lapiz", "sopas", "jeans" };
int anunciado[A];

char* shmem1, * shmem2, * shmem3; // Memoria compartida

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
	int cant_producidos = 0;
	
	for (i = 0; i < A; i++)
	{
		anunciado[i] = 0;
	}
	
	enlazar_shmem();
	
	while (cant_producidos < A)
	{
		anuncio = anuncios[(i = producir_anuncio())];
		down_sem(semid, VACIAS);
		down_sem(semid, MUTEX);
		if (insertar_anuncio(anuncio) == 1)
		{
			printf("Anunciar: %s ...\n", anuncios[i]);
			anunciado[i] = 1;
			cant_producidos++;
		}
		up_sem(semid, MUTEX);
		up_sem(semid, LLENAS);
	}
	
	return 0;
}

int producir_anuncio()
{
	int i;
	
	do
	{
		srand(time(NULL));
		i = rand() % A;
	}
	while (anunciado[i]);
	
	return i;
}

int insertar_anuncio(char* anuncio)
{
    if (esta_vacio(shmem1))
    {
		strcpy(shmem1, anuncio);
	}
	else if (esta_vacio(shmem2))
	{
		strcpy(shmem2, anuncio);
	}
	else if (esta_vacio(shmem3))
	{
		strcpy(shmem3, anuncio);
	}
	else
	{
		//printf("Error: memoria compartida llena\n");
		return 0;
	}
	
	return 1;
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
	int shmid1, shmid2, shmid3;
	
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
