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
	
	int yearOffset = 0;
	int addupDays = query_days_in_year(1970);
	if (index >= 0) {
		while (addupDays < index) {
			yearOffset ++;
			addupDays += query_days_in_year(1970 + yearOffset);
		}	
	} else {
		yearOffset --; addupDays = -query_days_in_year(1969);
		while (-addupDays < -index) {
			yearOffset --;
			addupDays -= query_days_in_year(1970 + yearOffset);
		}
	}
	result.year = 1970 + yearOffset;
	
	int dayPozInYear = index - (addupDays - query_days_in_year(result.year));
	if (index < 0) { dayPozInYear = query_days_in_year(result.year) - (index - (addupDays + query_days_in_year(result.year))); }
	MONTH monthOffset = JAN;
	addupDays = query_days_in_month(result.year, JAN);
	while (addupDays <= dayPozInYear) {
		monthOffset ++;
		addupDays += query_days_in_month(result.year, monthOffset);
	}
	result.month = monthOffset;
	result.monthday = dayPozInYear - (addupDays - query_days_in_month(result.year, monthOffset));

	result.index = index;
	result.weekday = (index + 7000000 + 3) % WD_LEN;
	return result;
}


int main([[maybe_unused]] const int argc,[[maybe_unused]] const char* argv[]) {
/*
	initscr();			// Sets up the memories needed by curses to eradicate its doubt of existance
	cbreak();			// Disables line buffering like setbuf(stdout, NULL);.
	noecho();			// Disables input echoing.
	nodelay(stdscr, TRUE); // Sets getch() to non-blocking mode. It will return ERR if no keystrokes are provided.
	start_color();		// Enables coloring. COLOR_PAIR();s are available from now on.
	clear();			// Clears the screen buffer and move the cursor to (0, 0) and forces full screen redraw.
	refresh();			// Blits the buffer.
	curs_set(0);		// Hides the cursor.

	// Main loop here.

	endwin();			// Bye bye
	putchar('\n');
	return 0;
*/	
	Day day;
	for (DayIndex d = -500; d < 500; d ++) {
		day = query_day_info(d);
		printf("Day %d, %s %-2d, %d, %s.\n", d, MONTHS[day.month], day.monthday, day.year, WEEKDAYS[day.weekday]);
	}
	return 0;
}
