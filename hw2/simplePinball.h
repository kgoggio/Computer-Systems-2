#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#include	<unistd.h>
#include	<wait.h>
#include	<ncurses.h>

const int	RESPONSE_TIME_USECS	= 250000;

const int	BALL_SIGNAL		= SIGUSR1;

const int	NUM_BUMPERS		= 3;

const int	LINE_LEN		= 256;

const int	NUM_BALLS		= 3;

const int	HALF_TRANSIT_COUNT	= 4;

const int	TOP_ROW			= 10;

const int	BOUNCER_ROWS[NUM_BUMPERS+1]
					= {20,20,20,20};

const int	BALL_ROWS[HALF_TRANSIT_COUNT]
					= {19, 15, 12, 10};

//  PURPOSE:  To increase or decrease the speed, change this constant.
//	(It is in microseconds: 1/1,000,000th of a second)
const int	TRANSIT_STEP_USECS	= 150000;

const int	BOUNCER_LEN		= 8;

const char	BALL			= 'O';

const char	NO_BALL			= ' ';

#define		BUMPER			"======="

#define		BOUNCED_BUMPER		"-------"

#define		NO_BUMPER		"       "

#define		FLIPPER			"==   =="

#define		BUMPER_PROGNAME		"bumper"


//  PURPOSE:  To handle the bouncing a ball from position 'fromPosition' to
//	position 'toPosition', where 'toProcess' is the process id of the
//	receiving flipper or bumper.
//
//	NOTE:	position 0 means the flipper (the parent process)
//		position 1 means the 1st bumper
//		position 2 means the 2nd bumper
//		position 3 means the 3rd bumper
extern
void		bounceBall	(int	fromPosition,
				 int	toPosition,
				 pid_t	toProcess
				);
