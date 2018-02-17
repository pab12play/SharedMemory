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
	printf("Fork creado [Process id: %d]\n",getpid());
	printf("Fork padre es [Process id: %d]\n",getppid());
/* Change the file mode mask */
	umask(0);       

/* Open any logs here */

/* Create a new SID for the child process */
	int sid = setsid();
	if (sid < 0) {
/* Log any failures here */
		exit(EXIT_FAILURE);
	}


/* Daemon-specific initialization goes here */
	const char *memname = "sample";
	const size_t region_size = sizeof(int)*5;

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1)
		error_and_die("shm_open");

	int r = ftruncate(fd, region_size);
	if (r != 0)
		error_and_die("ftruncate");

	char *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
		error_and_die("mmap");
	close(fd);

/* The Big Loop */
	while (1) {
		
		ptr[0] = 'a';
		sleep(3); /* wait 3 seconds */
	}

}

void ParentProcess()
{
	printf("El padre es [Process id: %d]\n",getpid());
	exit(EXIT_SUCCESS);
}	

void error_and_die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}