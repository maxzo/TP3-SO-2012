void inicializar_sem(int semid, int nro_sem, int valor)
{
	union semun arg;
	
	arg.val = valor;
	if (semctl(semid, nro_sem, SETVAL, arg) == -1)
	{
	  perror("semctl");
	}
}

void down_sem(int semid, int nro_sem)
{
	struct sembuf registro;
	
	registro.sem_num = nro_sem;
	registro.sem_op = -1;
	registro.sem_flg = 0;
	
	if (semop(semid, &registro, 1) == -1)
	{
		perror("semop");
	}
}

void up_sem(int semid, int nro_sem)
{
	struct sembuf registro;
	
	registro.sem_num = nro_sem;
	registro.sem_op = 1;
	registro.sem_flg = 0;
	
	if (semop(semid, &registro, 1) == -1)
	{
		perror("semop");
	}
}
