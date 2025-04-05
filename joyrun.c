#include "joyrun.h"
#include "250320.h"

unsigned int query_days_in_year(int year) { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 366 : 365; }

unsigned int query_days_in_month(int year, MONTH month) { 
	return (month == FEB) ? 
		(((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28) : 
		((month == APR || month == JUN || month == SEPT || month == NOV) ? 30 : 31); 
}

DayIndex get_today_dayIndex() { return time(NULL) / (60 * 60 * 24); }

Day query_day_info(DayIndex index) {
	Day result = {0, 0, 0, 0, 1970};
	
	result.weekday = (index + 7000000 + 3) % WD_LEN;
	
	int yearOffset = 0;
	int addupDays = query_days_in_year(1970);
	if (index >= 0) {
		while (addupDays < index) {
			yearOffset ++;
			addupDays += query_days_in_year(1970 + yearOffset);
		}	
	} else {
		yearOffset --;
		addupDays = query_days_in_year(1969);
		while (addupDays < -index) {
			yearOffset --;
			addupDays += query_days_in_year(1970 + yearOffset);
		}
	} 
	result.year = 1970 + yearOffset;
	// printf("yearOffset: %d, addupDays: %d.\n", yearOffset, addupDays);

	int f = 0;
	if (index < 0) { index = - index - 1; f = 1; }
	int dayPozInYear =
		index - (addupDays - query_days_in_year(result.year));
		if (f) { dayPozInYear = query_days_in_year(result.year) - dayPozInYear - 1; }
	MONTH monthOffset = JAN;
	addupDays = query_days_in_month(result.year, JAN);
	while (addupDays <= dayPozInYear) {
		monthOffset ++;
		addupDays += query_days_in_month(result.year, monthOffset);
	}
	result.month = monthOffset;
	result.monthday = dayPozInYear - (addupDays - query_days_in_month(result.year, monthOffset)) + 1;

	result.index = index;
	return result;
}



int main([[maybe_unused]] const int argc,[[maybe_unused]] const char* argv[]) {

	initscr();			// Sets up the memories needed by curses to eradicate its doubt of existance
	cbreak();			// Disables line buffering like setbuf(stdout, NULL);.
	noecho();			// Disables input echoing.
	nodelay(stdscr, TRUE); // Sets getch() to non-blocking mode. It will return ERR if no keystrokes are provided.
	start_color();		// Enables coloring. COLOR_PAIR();s are available from now on.
	clear();			// Clears the screen buffer and move the cursor to (0, 0) and forces full screen redraw.
	refresh();			// Blits the buffer.
	curs_set(0);		// Hides the cursor.

	DayIndex today = get_today_dayIndex();
	int thisPoz[2] = {5, 5};
	int topleft[2] = {5, 5};

	for (int i = 0; i < 100; i ++) {
		Day thisDay = query_day_info(today + i);
		if (thisDay.weekday == MON) { thisPoz[1] += 3; }
		thisPoz[0] = topleft[0] + thisDay.weekday * 2;
		mvprintw(thisPoz[0], thisPoz[1], "%d", thisDay.monthday);
		if (thisDay.monthday == 1) mvprintw(topleft[0] - 1, thisPoz[1], "%s", MONTHS[thisDay.month]);
	}

	init_frame_control(60);
	double wkT; double idT;
	while (getch() != 'q') { frame_control(&wkT, &idT); }
	end_frame_control();

	endwin();			// Bye bye
	putchar('\n');
	return 0;

	
}
