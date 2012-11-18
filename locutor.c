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

char* extraer_anuncio();
void publicar_anuncio(char* anuncio);

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
	
	printf("Extrayendo anuncio\n");
	sleep(1);
	
	return anuncio;
}

void publicar_anuncio(char* anuncio)
{
	
}
