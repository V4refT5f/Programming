
#ifndef _250320_H
#define _250320_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "win_nanosleep.h"
#define NCURSES_STATIC

/**
 * Get the timespan from the beginning of the program to call time.
 * 
 * @return Time in seconds
 */
double getsec();

#ifdef _WIN32
#include <ncurses/ncurses.h>
#include <windows.h>

double getsec() {
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / frequency.QuadPart;
}
#else
#include <ncurses.h>

double getsec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (double) ts.tv_nsec / 1e9;
}
#endif

#ifndef _RAND_BLOCK
#define _RAND_BLOCK

static uint32_t RANDSEED = 5;
uint32_t rand_int() {
	RANDSEED ^= RANDSEED << 3;
	RANDSEED ^= RANDSEED << 7;
	RANDSEED ^= RANDSEED << 11;
	return RANDSEED;
}

double rand_double() {
	return (double) rand_int() / (double) UINT32_MAX;
}

#endif

/***********************************************
 *
 *	250320.h
 *
 *	It has been every child's dream to make an
 *	interactive program in command line interface.
 *	Much of it was unfulfilled due to the confusing
 *	nature of CLI IO handling until recently when
 *	NCurses took place. 
 *
 *
 ***********************************************/

void _spin_sleep(double secs) { double destination = getsec() + secs; while (getsec() < destination) {  } return; }

/**
 * Try to sleep as accurately as possible and return the actual sleep time.
 * 
 * @param secs How long you want to sleep. Must be positive
 * @return The actual sleep time
 */
double _accu_sleep(double secs) {
	double calltime = getsec();
	struct timespec ts = sec_to_timespec(secs);
	nanosleep(&ts, NULL);
	if (getsec() - calltime < secs) { _spin_sleep(calltime + secs - getsec()); }
	return getsec() - calltime;
}

static double frameBeginTime = 0.0;
static double targetFrameTime = 0.0;

/**
 * Initialize frame controlling.
 * 
 * @param fps The target frames per second. Must be positive
 */
void init_frame_control(double fps) { frameBeginTime = getsec(); targetFrameTime = 1.0 / (fps + 0.00001); return; }

/**
 * Terminate frame controlling.
 */
void end_frame_control() { frameBeginTime = 0.0; targetFrameTime = 0.0; return; }

/**
 * Try to maintain a consistent time between calls as accurate as possible.
 * 
 * @param workTime Give the time already consumed. Must not be NULL
 * @param idleTime Give the time managed by frame_control(). Must not be NULL
 */
void frame_control(double *workTime, double *idleTime) { 
	*workTime = getsec() - frameBeginTime;
	*idleTime = 0.0;
	if (*workTime < targetFrameTime) {
		*idleTime = targetFrameTime - *workTime;
		*idleTime = _accu_sleep(*idleTime); 
	}
	frameBeginTime = getsec();
}

/**
 * A blocking and io-dominating routine to check the non-buffered input functionality.
 * Require ncurses to be initialized to use.
 * 
 * @param HEIGHT Height. Positive
 * @param WIDTH Width. Positive
 */
int test_nbinput(const uint8_t HEIGHT, const uint8_t WIDTH) {
	clear(); refresh();
	init_pair(10, COLOR_BLUE,  COLOR_BLACK);
	init_pair(11, COLOR_BLACK, COLOR_WHITE);
	
	#define CHARSTATES  4
	char charAnimation[CHARSTATES] = {'-', '\\', '|', '/'};
	const int LOOPC = HEIGHT * WIDTH;
	size_t index = 0;
	signed char ch = ERR;
	int colorPair = 10;
	double wkT = 0.0; 
	double idT = 0.0;

	init_frame_control(30.0);

	while (ch != 'q') {
		refresh(); /*clear();*/
		frame_control(&wkT, &idT);
		if (!index) { erase(); } 		// erase() is much, much more friendlier than clear().
		ch = getch();
		if (ch == ERR) { ch = charAnimation[index % CHARSTATES]; colorPair = 10; }
		else { colorPair = 11; }
		attron(COLOR_PAIR(colorPair));
		mvprintw(2 + (index / WIDTH) * 2, 4 + (index % WIDTH) * 2, " %c", ch);
		attroff(COLOR_PAIR(colorPair));
		mvprintw(4 + HEIGHT * 2    , 4, "workTime: %g   ", wkT);
		mvprintw(4 + HEIGHT * 2 + 1, 4, "idleTime: %g   ", idT);
		index = (index + 1) % LOOPC;
	}

	end_frame_control();
	return 0;		
}

/*
int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
	initscr();			// Sets up the memories needed by curses to eradicate its doubt of existance
	cbreak();			// Disables line buffering like setbuf(stdout, NULL);.
	noecho();			// Disables input echoing.
	nodelay(stdscr, TRUE); // Sets getch() to non-blocking mode. It will return ERR if no keystrokes are provided.
	start_color();		// Enables coloring. COLOR_PAIR();s are available from now on.
	clear();			// Clears the screen buffer and move the cursor to (0, 0) and forces full screen redraw.
	refresh();			// Blits the buffer.
	curs_set(0);		// Hides the cursor.

	test_nbinput(19, 41);

	endwin();			// Bye bye
	putchar('\n');
	return 0;
}
*/

#endif
