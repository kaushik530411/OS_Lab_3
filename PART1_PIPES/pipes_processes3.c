/*Modify the code in a new file (pipes_processes3.c) 
 * to take one argument from the command line using argc
 *  and argv that is the argument passed to the grep program
 *  (e.g. Lakers or 28), then create three processes P1 
 * (Parent - executes cat scores) and P2 (Child - execute 
 * grep <your argument>) and P2 (Child’s Child - execute 
 * sort). The output of cat scores would be the input to 
 * grep Lakers, and int output from grep Lakers would be 
 * the input for sort.
 */

// Imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h> 

int main(int argc, char **argv)
{
  int pipefd[2];
  int pipefd2[2];
  int pid, pid2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};

  // Make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);
  pipe(pipefd2);
  
  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "fork Failed" ); 
    return 1; 
  } else if (pid > 0) { // Parent process.
    close(pipefd[0]);
    close(pipefd2[0]);
    close(pipefd2[1]);
    
    // replace standard output with output part of pipe
    dup2(pipefd[1], 1);
    
    // execute cat and stores output into first pipe.
    execvp("cat", cat_args);
    
    close(pipefd[1]);
  } else { // Child process.
    pid2 = fork();
    if (pid2 < 0) {
      fprintf(stderr, "fork Failed" ); 
      return 1; 
    } else if (pid2 > 0) {
      dup2(pipefd[0], 0);
      dup2(pipefd2[1], 1);
      
      close(pipefd[1]);
      close(pipefd2[0]);
      
      // execute grep
      execvp("grep", grep_args);
        
      close(pipefd[0]);
      close(pipefd2[1]);
      exit(0);
    } else {
      // replace standard input with input part of pipe
      dup2(pipefd2[0], 0);
    
      close(pipefd2[1]);
      close(pipefd[1]);
      close(pipefd[0]);
      
      char *sort_args[] = {"sort", NULL};
      execvp("sort", sort_args);
      close(pipefd2[0]); // Close pipe ends.
      exit(0);
    }
  }
}