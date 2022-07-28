#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


static char **parserCmd(char *buff, char *delimiter) {

               char *token = NULL;
	       char **cmd = NULL;
	       size_t idex = 0;

	       token = strtok(buff, delimiter);

	       while(token) {
                    cmd = (char **)realloc(cmd, ((idex+1)*sizeof(char *)));
		    cmd[idex] = strdup(token);
		    token = strtok(NULL, delimiter);
		    ++idex;
	       }

            // On alloue un element qu'on met a NULL a la fin du tableau
	    
	       cmd = (char **)realloc(cmd, ((idex +1 )*sizeof(char *)));
	       cmd[idex] = NULL;
	       return cmd;

}


static void free_array(char **array) {

	for( int i=0; array[i]; i++) {
              free(array[i]);
	      array[i] = NULL;

	}

	free(array);
	array = NULL;


}

static void exec_cmd(char **cmd) {

    pid_t pid =0;
    int status =0;


    pid = fork();
    if (pid == -1) 
	    perror("fork failed");
    else if(pid > 0) {

         waitpid(pid, &status, 0);// &status = NULL
	 kill(pid, SIGTERM);
    }else {

           if (execve(cmd[0], cmd, NULL) == -1)
		   perror("execve() failed");
	   exit(EXIT_FAILURE);
    }
        

}


int main() {

       
	char *buff = NULL;
	size_t buf_size = 2048;

	buff = (char *)calloc(sizeof(char), buf_size);
	if(buff == NULL) {
		perror("allocation failed!");
		return (EXIT_FAILURE);
	}
	
	write(1, "~$", 3);
      
        while( getline(&buff, &buf_size, stdin) > 0) {
		printf("cmd = %s\n", buff);
               write(1, "~$", 3);

	}


      free(buff);


}
