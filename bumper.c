/*--------------------------------------------------------------------------*
 *----									----*
 *----		bumper.c						----*
 *----									----*
 *----	    This file defines most of the functionality needed by the	----*
 *----	bumper processes.						----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1a					Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

//	Compile with:
//	$ g++ bumper.c bounceBall.c -o bumper -lncurses

#include "simplePinball.h"

//  PURPOSE:  To hold the process ids of the other known bouncers:
//	the parent process (the flipper), sibling bumpers created earlier,
//	and this process itself.
pid_t bouncerPidArray[NUM_BUMPERS + 1];

//  PURPOSE:  To tell the position of this process.
//	NOTE:	position 1 means the 1st bumper
//		position 2 means the 2nd bumper
//		position 3 means the 3rd bumper
int thisPosition;

//  PURPOSE:  To hold 1 while this process should still run, or 0 otherwise.
int shouldRun = 1;

//  PURPOSE:  To note that this process should not run anymore.
//	Ignores 'sigNum'.  No return value.
void intHandler(int sigNum) {
  //  YOUR CODE HERE
   shouldRun = 0;
}

//  PURPOSE:  To note that this process just received the ball.
//	Ignores 'sigNum'.  No return value.
void ballHandler(int sigNum) {
  {
    int col = 2 * BOUNCER_LEN;

    for (int i = 1; i <= NUM_BUMPERS; i++) {
      move(BOUNCER_ROWS[i], col);
      addstr((i == thisPosition) ? BOUNCED_BUMPER : BUMPER);
      col += BOUNCER_LEN;
    }

    move(BOUNCER_ROWS[0], BOUNCER_LEN);
    addstr(FLIPPER);
    refresh();
  }

  int nextPosition = rand() % (thisPosition + 1);

  move(BALL_ROWS[0], (thisPosition + 1) * BOUNCER_LEN + BOUNCER_LEN / 2);
  addch(BALL);
  refresh();
  usleep(TRANSIT_STEP_USECS);

  move(BALL_ROWS[0], (thisPosition + 1) * BOUNCER_LEN + BOUNCER_LEN / 2);
  addch(NO_BALL);
  move(BOUNCER_ROWS[thisPosition], (thisPosition + 1) * BOUNCER_LEN);
  addstr(BUMPER);
  refresh();
  bounceBall(thisPosition, nextPosition, bouncerPidArray[nextPosition]);
}

//  PURPOSE:  To runs the program as a whole.  'argc' tells the number of items
//	on the command line, and 'argv[]' has the array of those arguments.
//	Returns 'EXIT_SUCCESS' to the OS.
int main(int argc, char *argv[]) {
  //  I.  Application validity check:
  //srand(time(NULL));
  //  II.  Do bumpers:
  //  II.A.  Turn on graphics (already done):
  initscr();

  //  II.B.  Initialize 'bouncerPidArray[]' and 'thisPosition':
  //  YOUR CODE HERE
  bouncerPidArray[0] = getppid();

  if(argc==2) {
    bouncerPidArray[1]= (pid_t) atol(argv[1]);
  }
  if(argc==3) {
    bouncerPidArray[1]= (pid_t) atol(argv[1]);
    bouncerPidArray[2] = (pid_t) atol(argv[2]);
  }

  bouncerPidArray[argc] = getpid();
  
  thisPosition = argc;

  //  II.C.  Install signal handler:
  //  YOUR CODE HERE
 

  struct sigaction intAct;
  memset(&intAct, '\0', sizeof(intAct));
  intAct.sa_handler = intHandler;
  intAct.sa_flags = SA_NOCLDSTOP | SA_RESTART; //cut this out
  sigaction(SIGINT, &intAct, NULL);
  //no need todo mem set 2x since its not using flags
  struct sigaction ballAct;
  memset(&ballAct, '\0', sizeof(ballAct));
  ballAct.sa_handler = ballHandler;
  ballAct.sa_flags = SA_NOCLDSTOP | SA_RESTART; //cut this out
  sigaction(BALL_SIGNAL, &ballAct, NULL);

  //  II.D.  Hang out and bump:
  while (shouldRun) {
    sleep(1);
  }

  //  III.  Finished:
  return (EXIT_SUCCESS);
}
