/*******************************************************************************
- Initially prompts the user
- 

 @author   - Joseph Cutino, Brendon Murthum
 @version  - Winter 2018 


write a parent program that:
installs signal handler(s) for the predefined user-defined signals (SIGUSR1/SIGUSR2)
spawns off a child process
when a user-defined signal is raised, it reports the type of signal sent
note: it may be necessary to reinstall your signal handler after a signal is received
terminates gracefully upon receiving a Control-C (i.e. the parent should notify the child to also stop gracefully)
the child process should repeatedly:
wait a random amount of time (e.g. one to five seconds)
randomly send one of the user-defined signals to its parent

*******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

void closeSignalHandler (int);
void signalOneHandler (int);
void signalTwoHandler (int);

int main () {

    /* Assign Signal Handlers */
    signal (SIGINT, closeSignalHandler);
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
    	
    	sleep (1);
    	while(1){
    		//loop and produce signals until ctrl-c
    		int randTime = rand() % 5; //generate random number 
    		//random amount of time
    		sleep (randTime);

    		int randSig = rand() % 2; //generate random number 
    		
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
    	//parent process
    	printf("Spawned child: PID# %i", pid);
    	
    	while(1){
    		//loop until ctrl-c
    		
    		pause();
    	}
    
    }

    //printf ("waiting...\n");
    //pause ();
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

void closeSignalHandler (int sigNum) {
    printf (" ^C recieved.\n");
    /* Handle Clean-Up here */

    /* End of Clean-Up */
    sleep (1);
    printf ("That's it, I'm shutting you down...\n");
    exit (0);
}
