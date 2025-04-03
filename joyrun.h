#ifndef JOYRUN_H
#define JOYRUN_H

#include <time.h>

typedef enum {JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEPT, OCT, NOV, DEC, M_LEN} MONTH;
const char* MONTHS[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

typedef enum {MON, TUE, WED, THU, FRI, SAT, SUN, WD_LEN} WEEKDAY;
const char* WEEKDAYS[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

/**
 * The day count since Jan 1, 1970.
 */
typedef int DayIndex;

/**
 * A struct representing a day with all the attached information.
 */
typedef struct {
	DayIndex index;
	WEEKDAY weekday;
	MONTH month;
	int monthday;
	int year;
} Day;

unsigned int query_days_in_year(int year);

unsigned int query_days_in_month(int year, MONTH month);

DayIndex get_today_dayIndex();

Day query_day_info(DayIndex index);




#endif
