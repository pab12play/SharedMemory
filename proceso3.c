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
char* open_mem(char* memname, int region_size);
char* create_mem(char* memname, int region_size);

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

    int sid = setsid();
	if (sid < 0) {
		error_and_die("SID");
	}

	const size_t region_size = sizeof(char)*10;

	char *ptr1 = open_mem("area1", region_size);
	char *ptr2 = create_mem("area2", region_size);

    int a = 0;
	while(1){
        for(int i=0;i<10;i++){
            if(ptr1[i]!=0){
                if(ptr1[i]%2==0){
                    ptr2[a]=ptr1[i];
                    ptr1[i]=0;
                    //printf("valor de temp[%d]=%d \n",a,ptr2[a]);
                    a++;
                    if(a>=10) a=0;
                }
            }
        }
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

char* create_mem(char* memname, int region_size){
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
	return ptr;
}

char* open_mem(char* memname, int region_size){
	int fd = shm_open(memname, O_RDWR, 0666);
	if (fd == -1){
		error_and_die("shm_open");
	}

	char *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED){
		error_and_die("mmap");
	}
	close(fd);
	return ptr;
}