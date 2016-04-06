
// Includes
//{{{
#include<time.h>
#include<unistd.h> // for usleep
//}}}

// Variables & Definitions
//{{{

// Maximum buffer for lag smoothing, in milliseconds
#define MAXLAG 500

// How many milliseconds between frames FLAG if changed, change in lib.h until autogen
#define FPS 64

static int g_lastTime;
static int g_lag;
static int g_period;
//}}}

// Protottypes
//{{{
static int getTime();
//}}}


// Init Clock
//{{{
static void initClock()
{
	g_lastTime = getTime();
	g_lag = 0;
	g_period = 1000/FPS;
}

void (*initClockPtr)(void) = initClock;
//}}}

// Wait For Next Frame
//{{{
void waitForNextFrame()
{
	int wakeTime;		// Time at which function should release
	int currentTime;	// Records time at which sleep stopped
	int sleepPeriod;	// How long the function should sleep for

	// Calculate wake time
	wakeTime = g_lastTime + g_period - g_lag;

	// Calculate time to sleep between now and wake time
	sleepPeriod = (wakeTime - getTime())*999; // time in microseconds, minus small amount

	// Hang until wait time
	// FLAG is this too opaque? simplify for sake of readability?
	if(sleepPeriod>0)
	{
		usleep(sleepPeriod);
	}
	while((currentTime=getTime()) < wakeTime){}

	// Determine how much lag, if any.  Add to buffer
	g_lag+= (currentTime - g_lastTime) - g_period;
	if(g_lag > MAXLAG)
	{
		g_lag = MAXLAG;
	}

	// Record wake time for next tick
	g_lastTime = currentTime;
}
//}}}


// Get Time
//{{{
static int getTime()
{
	// Returns time in ms since game genesis

	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec*1000 + t.tv_nsec/1000000;
}
//}}}


