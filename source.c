/*******************************************************************************
 @author   - Joseph Cutino, Brendon Murthum
 @version  - Winter 2018 

This program installs signal handlers(s) for the user-defined signals (SIGUSR1/SIGUSR2).
A child process is then created which will enter into an infinity loop. Within this loop
it will wait for a random amount of time then randomly send either SIGUSR1 or SIGUSR2 to
the parent process. The parent process will wait for the signal from the child, handle the 
signal, re-esstablish the signal handler, then enter back into a wait state. At any point 
the user can enter ctrl-c to exit the process. This action is handle by the parent process
who will close the child process properly.
*******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

//Function Prototypes
void closeSignalHandler (int);
void signalOneHandler (int);
void signalTwoHandler (int);
void killfunc(int);

pid_t child = 0; //global for signal interrupts

int main () {

    /* Assign Signal Handlers */ 
    signal (SIGUSR1, signalOneHandler);
    signal (SIGUSR2, signalTwoHandler);
    

    /* Create Child Process */
    pid_t pid, parentPID;
    parentPID = getpid();
    if((pid = fork()) < 0){
    	perror("fork failed");
    	exit(1);
    }
    
    else if(pid == 0){
    	//child process
    	srand(time(NULL));
	
	//Assign kill function to child process so that parent can close it upon exit	
	signal (SIGKILL, killfunc);
    	
    	while(1){
    		//loop and produce signals until ctrl-c
    		int randTime = rand() % 5; //generate random number 
    		//random amount of time
    		sleep (randTime);

    		int randSig = rand() % 2; //generate random number 
    		
		fflush(stdout);
    		//raise signal for random number
    		if(randSig == 0){
    			//raise signal one
    			kill(parentPID, SIGUSR1);
    		}else if(randSig == 1){
    			//raise signal two
    			kill(parentPID, SIGUSR2);
    		}  
    	}
    }
    
    else{
	//Esstablish close signal handler for parent process only
	signal (SIGINT, closeSignalHandler);

    	//parent process
    	printf("Spawned child: PID# %i\n", pid);
    	child = pid;
	
    	while(1){
    		//loop until ctrl-c
		printf("Waiting...  ");
		fflush(stdout);
    		pause();
    	}
    
    }
    return 0;
}

void signalOneHandler(int sigNum){
	printf(" received a SIGUSR1 signal\n");

	/* Re-esstablish Signal Handler */
	signal (SIGUSR1, signalOneHandler);
	return;
}

void signalTwoHandler(int sigNum){
	printf(" received a SIGUSR2 signal\n");

	/* Re-esstablish Signal Handler */
	signal (SIGUSR2, signalTwoHandler);
	return;
}

void killfunc(int sigNum){
	exit(0);
}

void closeSignalHandler (int sigNum) {
	pid_t p = getpid();
	if(p==child){
		/* Handle Clean-Up here */
		kill(child, SIGKILL);
    		/* End of Clean-Up */
	}else{
		printf (" received.\nThat's it, I'm shutting you down...\n");
    		exit (0);
	}
}
