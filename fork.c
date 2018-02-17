//standard input output
#include <stdio.h>
//unix standard
#include <unistd.h>

#include <stdlib.h>

void ChildProcess();
void ParentProcess();

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
        
        /* The Big Loop */
        while (1) {
           
           sleep(3); /* wait 3 seconds */
        }
	
}

void ParentProcess()
{
	printf("El padre es [Process id: %d]\n",getpid());
	exit(EXIT_SUCCESS);
}	
	