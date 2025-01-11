
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>

double getsec() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec + (double) ts.tv_nsec / 1e9;
}

// A 		for upwards.
// B 		for downwards.
// [num]C 	for right.
// [num]D 	for left.
void escseq(const char* s) {
	printf("\033[%s", s); return;
}

size_t getSpeedIndColor( double input,
	   size_t redPair, size_t yellowPair, size_t greenPair, 
	   double redThres, double yellowThres) {
	if (input <= redThres) { return redPair; }
	else if (input <= yellowThres) { return yellowPair; }
	else { return greenPair; }
}

int main(const int argc, const char* argv[]) {
	printf("\n%d ... Where are you?", argc);
	sleep(1);
	printf("\nAh! You are here: ");
	sleep(1);
	printf("%s", argv[0]);
	sleep(1);

	printf("\n\nAlright, let's test your typing speed.");
	sleep(1);
	printf("\nYou may start typing after the screen refreshes  ... \n[ :Q ] to exit.\n[ :R ] to reset speed.");
	sleep(1);
	printf("\n");
	sleep(1);

	initscr();
	cbreak();
	noecho();
	start_color();
	clear();
	refresh();

	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_RED,  COLOR_BLACK);
	init_pair(3, COLOR_YELLOW,COLOR_BLACK);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);

	double startSec = getsec();
	double thisSec = getsec();
	double lastSec = getsec();
	double wstartSec = getsec();
	size_t charCount = 0;
	size_t wordCount = 0;
	size_t wordLen = 0;
	double cps = 0;
	double wps = 0;

	#define WB 128
	char input = ' ';
	char lastInput = ' ';
	char word[WB] = {'\0'};

	short breakout = 0;

	while (!breakout) {
		clear();
		mvprintw( 1, 0,
			   "-------------------------------------\n\n"\
			   "   Last key press  :      [ %c ]     \n"\
			   "   Your last word  :      [ %s ]     \n"\
			   "   Keypress / Min  :                 \n"\
			   "   Word / Min      :                 \n\n"\
			   "-------------------------------------\n",
			   input, word);
		attron(COLOR_PAIR(getSpeedIndColor(cps*60, 2, 3, 4, 210., 350.)));
		mvprintw( 5, 27, "%g", cps*60);
		attroff(COLOR_PAIR(getSpeedIndColor(cps*60, 2, 3, 4, 210., 350.)));
		attron(COLOR_PAIR(getSpeedIndColor(wps*60, 2, 3, 4, 30., 50.)));
		mvprintw( 6, 27, "%g", wps*60);
		attroff(COLOR_PAIR(getSpeedIndColor(wps*60, 2, 3, 4, 30., 50.)));
		refresh();
		
		lastInput = input; input = getch();
		lastSec = thisSec; thisSec = getsec();

		if (input == ' ') {
			if (lastInput != ' ') { wordCount ++; }
		}
		else {
			if (lastInput == ':') { 
				if (input == 'Q') { breakout = 1; } 
				if (input == 'R') { startSec = thisSec - 0.1; charCount = -1; wordCount = 0; }
			}
			else if (lastInput == ' ') { wordLen = 0; wstartSec = thisSec;  }
			charCount ++;
			wordLen = wordLen % (WB - 3); wordLen ++;
			word[wordLen - 1] = input; word[wordLen] = '\0';
		}

		cps = charCount / (thisSec - startSec);
		wps = wordCount / (thisSec - startSec);
		
	}

	endwin();
	printf("\n");
	return 0;
	
}
