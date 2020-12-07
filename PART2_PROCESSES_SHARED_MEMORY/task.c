/* You are to write a program that creates two shared 
 * variables of type int. One integer is the BankAccount, 
 * another is Turn. Both BankAccount and Turn are initialized
 * to 0. Your program must create two processes, one parent 
 * process and one child process, and allow the parent to 
 * deposit (i.e. add) money to the BankAccount, and also 
 * allow the child process to withdraw (i.e. subtract) money
 * from the BankAccount using Tanenbaum’s strict alternation
 * (see OS textbook Chap 4). Both the Parent and Child 
 * should loop 25 times and follow the rules below each time
 * through the loop
 */

// Imports
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main() {
  int ShmID;
  int *ShmPTR;
  pid_t pid;
  int bankAcc = 0;
  int Turn = 0;
  
  // Seed random for sleeping calls.
  srand(time(NULL)); 
  // Create ShmId for two integers. 
  ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
  
  if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
  }
  
  ShmPTR = (int *) shmat(ShmID, NULL, 0);
  if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
  }
  
  // Assign pointers for each of the integer variables.
  ShmPTR[0] = bankAcc;
  ShmPTR[1] = Turn;
  
  int index;
  pid = fork();
  if (pid < 0) {
    printf("*** fork error (server) ***\n");
    exit(1);
  } else if (pid > 0) {
    for (index = 0; index < 25; index++) {
      sleep(rand() % 6); // sleep for 0-5 seconds inclusive.
      int acc = ShmPTR[0];
      
      while (ShmPTR[1] != 0) {
          
      } // Loop while Turn != 0.
      
      if (acc <= 100) {
        int balance = rand() % 101; // Randomly generate money to deposit between 0 and 100 inclusive.
        if ((balance % 2) == 0) {
          acc += balance;
          printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, acc);
          ShmPTR[0] = acc;
        } else {
          printf("Dear old Dad: Doesn't have any money to give\n");
        }
      } else {
        printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", acc);
      }
      ShmPTR[1] = 1;
    }
  } else {
    for(index = 0; index < 25; index++) {
      sleep(rand() % 5);
      int acc = ShmPTR[0];
      
      while (ShmPTR[1] != 1) {
      } // Loop while Turn != 1
      int balance = rand() % 51; // generate a balance between 0 and 50 inclusive.
      printf("Poor Student needs $%d\n", balance);
      if (balance <= acc) {
        acc -= balance;
        printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, acc);
        ShmPTR[0] = acc;
      } else {
        printf("Poor Student: Not Enough Cash ($%d)\n", acc );
      }
      ShmPTR[1] = 0;
    }
  }
}