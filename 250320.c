
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h>
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
#include <time.h>

double getsec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + (double) ts.tv_nsec / 1e9;
}
#endif

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

/***********************************************
 *
 *	250320.c
 *
 *	It has been every child's dream to make an
 *	interactive program in command line interface.
 *	Much of it was unfulfilled due to the confusing
 *	nature of CLI IO handling until recently when
 *	NCurses took place. 
 *
 *
 ***********************************************/


/**
 * A blocking and io-dominating routine to check the non-buffered input functionality.
 * Require ncurses to be initialized to use.
 */
int test_nbinput() {
	clear(); refresh();
	init_pair(10, COLOR_BLUE, COLOR_BLACK);
	return 0;		
}

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
	initscr();			// Sets up the memories needed by curses to eradicate its doubt of existance
	cbreak();			// Disables line buffering like setbuf(stdout, NULL);.
	noecho();			// Disables input echoing.
	nodelay(stdscr, TRUE); // Sets getch() to non-blocking mode. It will return ERR if no keystrokes are provided.
	start_color();		// Enables coloring. COLOR_PAIR();s are available from now on.
	clear();			// Clears the screen buffer and move the cursor to (0, 0).
	refresh();			// Blits the buffer.

	endwin();			// Bye bye
	putchar('\n');
	return 0;
}
