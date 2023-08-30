/*--------------------------------------------------------------------------*
 *----									----*
 *----		bounceBall.c						----*
 *----									----*
 *----	    This file defines bounceBall(), which is needed both by	----*
 *----	simplePinball.c and bumper.c.					----*
 *----									----*
 *----	----	----	----	----	----	----	----	----	----*
 *----									----*
 *----	Version 1a					Joseph Phillips	----*
 *----									----*
 *--------------------------------------------------------------------------*/

#include "simplePinball.h"

//  PURPOSE:  To handle the bouncing a ball from position 'fromPosition' to
//	position 'toPosition', where 'toProcess' is the process id of the
//	receiving flipper or bumper.
//
//	NOTE:	position 0 means the flipper (the parent process)
//		position 1 means the 1st bumper
//		position 2 means the 2nd bumper
//		position 3 means the 3rd bumper
void bounceBall(int fromPosition, int toPosition, pid_t toProcess) {
  int row = BALL_ROWS[1];
  int col = (fromPosition + 1) * BOUNCER_LEN + BOUNCER_LEN / 2;
  int deltaPos =
      BOUNCER_LEN * (toPosition - fromPosition) / (2 * HALF_TRANSIT_COUNT);
  beep();

  for (int i = 2; i < HALF_TRANSIT_COUNT; i++) {
    move(row, col);
    addch(BALL);
    refresh();
    usleep(TRANSIT_STEP_USECS);
    move(row, col);
    addch(NO_BALL);
    refresh();

    row = BALL_ROWS[i];
    col += deltaPos;
  }

  for (int i = HALF_TRANSIT_COUNT - 2; i >= 1; i--) {
    row = BALL_ROWS[i];
    col += deltaPos;

    move(row, col);
    addch(BALL);
    refresh();
    usleep(TRANSIT_STEP_USECS);

    move(row, col);
    addch(NO_BALL);
    refresh();
  }

  //  YOUR CODE HERE
	kill(toProcess, BALL_SIGNAL);
}
