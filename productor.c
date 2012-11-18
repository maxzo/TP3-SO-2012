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

#define A 9 // Cantidad de anuncios

char* producir_anuncio();
void insertar_anuncio(char* anuncio);

char* anuncios[A] = { "jabon", "autos", "motos", "arroz", "pollo", "papel", "lapiz", "sopas", "jeans" };
int anunciado[A];

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
	char* anuncio;
	
	return anuncio;
}

void insertar_anuncio(char* anuncio)
{
	printf("Insertando anuncio\n");
	sleep(1);
}
