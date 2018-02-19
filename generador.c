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
	srand((int) pid);
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

/* Open any logs here */

/* Create a new SID for the child process */
	int sid = setsid();
	if (sid < 0) {
		error_and_die("SID");
	}

	const char *memname = "area1";
	const size_t region_size = sizeof(char)*10;

	int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if (fd == -1){
		error_and_die("shm_open");
	}

	int r = ftruncate(fd, region_size);
	if (r != 0){
		error_and_die("ftruncate");
	}

	char *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED){
		error_and_die("mmap");
	}
	close(fd);

	int i = 0;
	while(1){
		ptr[i] = rand()%100+1;
		//printf("Escribiendo en memoria[%d]=%d \n",i,ptr[i]);
		//fflush(stdout);
		i++;
		if(i>=10)i=0;	
		sleep(3); /* wait 3 seconds */
	}

}

void ParentProcess()
{
	printf("PARENT: El id es [Process id: %d]\n",getpid());
	exit(EXIT_SUCCESS);
}	

void error_and_die(const char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}