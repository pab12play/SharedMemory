#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>


void ChildProcess();
void ParentProcess();
void error_and_die(const char *msg);


int main()
{
	printf("Iniciando!\n");
	pid_t pid=fork();
	if(pid>0){
		ParentProcess();
	}
	else if(pid==0){
		ChildProcess();
	}
	else{
		error_and_die("fork");
	}
	return 0;
}


void ChildProcess()
{
	printf("CHILD: Fork creado [Process id: %d]\n",getpid());
	printf("CHILD: id del padre es [Process id: %d]\n",getppid());

	const char *memname = "sample";
	const size_t region_size = sizeof(char)*10;

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1){
		error_and_die("shm_open");
	}

	char *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED){
		error_and_die("mmap");
	}
	close(fd);


	while(1){
		printf("\n1. Mostrar listado de numeros \n");
		printf("2. Limpiar listado de numeros \n");
		printf("3. Detener proceso \n");
		printf("4. Mostrar estado \n");
		printf("5. Salir \n");
		scanf("Escoja una opcion: ");
		char input = getchar();
		if(input=='1'){
			for(int i = 0; i < 10; i++){
				printf("Posicion %d: Valor: %d\n", i, ptr[i]);
			}
		}
		if(input=='2'){
			for(int i = 0; i < 10; i++){
				ptr[i] = 0;
				printf("Posicion %d: Valor: %d\n", i, ptr[i]);
			}	
		}
		if(input=='3'){
			system("pkill generador.o");
		}
		if(input=='4'){
			printf("Proceso A\n");
			system("pgrep generador.o");
			printf("Proceso B\n");
			printf("%d\n", getppid());
			printf("%d\n", getpid());
		}
		if(input=='5'){
			exit(EXIT_SUCCESS);
		}
		getchar();
	}

}

void ParentProcess()
{
	printf("PARENT: El id es [Process id: %d]\n",getpid());
	printf("PARENT: Waiting for child\n");
	int status = 0;
	pid_t childpid = wait(&status);
	printf("Parent knows child %d finished with status %d.\n", (int)childpid, status);
	printf("PARENT: ending.\n");
}	

void error_and_die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}