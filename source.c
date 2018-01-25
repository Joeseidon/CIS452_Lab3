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

void closeSignalHandler (int);

int main () {
    signal (SIGINT, closeSignalHandler);
    printf ("waiting...\n");
    pause ();
    return 0;
}

void closeSignalHandler (int sigNum) {
    printf (" ^C recieved.\n");
    /* Handle Clean-Up here */

    /* End of Clean-Up */
    sleep (1);
    printf ("That's it, I'm shutting you down...\n");
    exit (0);
}
