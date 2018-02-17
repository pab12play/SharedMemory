//standard input output
#include <stdio.h>
//unix standard
#include <unistd.h>

#include <stdlib.h>

#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>

void ChildProcess();
void ParentProcess();
void error_and_die(const char *msg);

int main()
{
	int id;
	int vector[5];
	printf("Iniciando!\n");

	id=fork();
	if(id>0){
		ParentProcess();
	}
	else if(id==0){
		ChildProcess();
	}
	else{
		printf("Fallo la creacion\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}

void ChildProcess()
{
	//printf("Fork creado [Process id: %d]\n",getpid());
	//printf("Fork padre es [Process id: %d]\n",getppid());
	printf("murio");

/* Daemon-specific initialization goes here */
	const char *memname = "sample";
	const size_t region_size = sizeof(int)*5;

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
		error_and_die("shm_open");

	printf("murio");

	char *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
		error_and_die("mmap");
	close(fd);

	printf("murio");

/* The Big Loop */
	while (1) {
		printf("murio");
		printf("el mapa contiene %c",ptr[0]);
		sleep(3); /* wait 3 seconds */
	}

}

void ParentProcess()
{
	int status;
	printf("El padre es [Process id: %d]\n",getpid());
	waitpid(getpid()+1, &status, 0);
}	

void error_and_die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}