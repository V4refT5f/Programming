#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NCURSES_STATIC
#ifdef _WIN32
#include <ncurses/ncurses.h>
#else
#include <ncurses.h>
#endif
#include <math.h>
#include <stdint.h>

/* ------------------------------------- **

	Forewords:

	There are 100,000 pcs of hair per person on average.
	People lose 50 ~ 100 pcs of hair per day.
	The growth rate of hair typically ranges within 0.5 ~ 2 centimeters per month.
	
	Do you want to get bald?
	Whether or not, it's not up to you to decide (or at least partially.)
	This program will predict the fate of your hair, the accuracy of which largely depends on the parameters you give.

							Hairy, 24/12/26

** ------------------------------------- */

static uint32_t RNGSEED = 5;
uint32_t randInt() {
	RNGSEED ^= RNGSEED << 13;
	RNGSEED ^= RNGSEED >> 17;
	RNGSEED ^= RNGSEED << 5;
	return RNGSEED;
}

double randFloat() {
	return (double) randInt() / UINT32_MAX;
}

// Returns random numbers that follow normal distribution.
double randNormal() {
	double u1 = (double) randInt() / UINT32_MAX;
	double u2 = (double) randInt() / UINT32_MAX;
	return sqrt(-2. * log(u1)) * sin(2. * M_PI * u2);
}

typedef struct Hair {
	double age;			// Days
	double length;		// Centimeters
	double growthSpd;	// Centimeters per Day
	double fallProb;	// Possibility per Day
} Hair;

typedef struct HairStat {
	double avrLen;		
	double varLen;		// sigma ^ 2
	double avrAge;
	double varAge;		// sigma ^ 2
	double avrSpd;
	double varSpd;		// sigma ^ 2
	double totalLen;
	double hairlossSpd; // Pcs per Day
	double hairAmount;
} HairStat;

typedef struct SkinQuality {
	double gspddcoef; // Determines how the grow speed varies over time. (Per Day) Ref: 1 [No Change]
	double gspdvcoef; // Determines amount of randomness to be introduced in the degradation of growth speed. (Per Day) Ref: 0
	double fprbicoef; // Determines how the fall probability varies over time.
	double fprbvcoef; // Same
} SkinQuality;

typedef struct HeadSkin {
	size_t 		hCount;
	Hair* 		hairs;
	HairStat	hs;
	SkinQuality sq;
} HeadSkin;

void calculate_stat(HeadSkin* hskn, double days) {
	HairStat hs = {0., 0., 0., 0., 0., 0., 0., 0., 0.};
	for (size_t i = 0; i < hskn->hCount; i ++) {
		hs.avrLen += hskn->hairs[i].length;
		hs.avrAge += hskn->hairs[i].age;
		hs.avrSpd += hskn->hairs[i].growthSpd;
		if (hskn->hairs[i].age <= 0.) { hs.hairlossSpd += 1; }
		else { hs.hairAmount += 1; }
	}
	hs.hairlossSpd /= days;
	hs.totalLen = hs.avrLen;
	hs.avrLen /= hskn->hCount;	
	hs.avrAge /= hskn->hCount;	
	hs.avrSpd /= hskn->hCount;
	for (size_t i = 0; i < hskn->hCount; i ++) {
		hs.varLen += pow((hskn->hairs[i].length - hs.avrLen), 2);
		hs.varAge += pow((hskn->hairs[i].age - hs.avrAge), 2);
		hs.varSpd += pow((hskn->hairs[i].growthSpd - hs.avrSpd), 2);
	}	
	hs.varLen /= hskn->hCount;
	hs.varAge /= hskn->hCount;
	hs.varSpd /= hskn->hCount;
	hskn->hs = hs;
	return;
}

void init_headskin(HeadSkin* hskn, size_t hCount, HairStat refhs, SkinQuality refsq) {
	hskn->hCount = hCount;
	hskn->hairs = (Hair*) calloc(sizeof(Hair), hCount);
	for (size_t i = 0; i < hskn->hCount; i ++) {
		hskn->hairs[i].age = randNormal() * refhs.varAge + refhs.avrAge;
		hskn->hairs[i].growthSpd = randNormal() * refhs.varSpd + refhs.avrSpd;
		hskn->hairs[i].length = randNormal() * refhs.varLen + refhs.avrLen;
		hskn->hairs[i].fallProb = refhs.hairlossSpd / (double) hskn->hCount;
	}	
	hskn->sq = refsq;
	return;
}

void offset_time(HeadSkin* hskn, double days) {	
	SkinQuality sq = hskn->sq;
	for (size_t i = 0; i < hskn->hCount; i ++) {
		Hair* h = &(hskn->hairs[i]);
		h->length += h->growthSpd * days;
		h->age += days;
		h->growthSpd *= pow(sq.gspddcoef * (1 + sq.gspdvcoef * (randFloat() - 0.5)), days);
		h->fallProb *= pow(sq.fprbicoef * (1 + sq.fprbvcoef * (randFloat() - 0.5)), days);
		if (randFloat() >= pow(1 - h->fallProb, days)) {
			h->length = 0.;
			h->age = -0.00001;
			h->growthSpd = hskn->hs.avrSpd;
			h->fallProb = (h->fallProb >= 0.999) ? 0.999 : h->fallProb;
		}
	}
	calculate_stat(hskn, days);
	return;
}

void various_tests() {
	// TEST 1: Normally Distributed Sample Generator
	while (1) {
		clear();
		init_pair(10, COLOR_GREEN, COLOR_BLACK);
		init_pair(11, COLOR_YELLOW, COLOR_BLACK);
		init_pair(12, COLOR_RED, COLOR_BLACK);
		init_pair(13, COLOR_CYAN, COLOR_WHITE);
		const size_t indent = 4;
		const size_t voffset = 2;
		double rN;
		size_t colorpair = 0;
		size_t colorstat[4] = {0};
		char inp = ' ';
		for (size_t i = 1; inp != 'q'; i ++) {
			inp = getch();
			rN = randNormal();
			mvprintw(i % 16 + voffset, indent, "<%5ld>  randInt:  %12d, randNormal:  %9g", i, randInt(), rN);
			move(i % 16 + 21 + voffset, 0); clrtoeol();
			colorpair = (size_t) (floor(fabs(rN))) % 4 + 10;
			colorstat[colorpair - 10] ++;
			attron(COLOR_PAIR(colorpair));
			mvprintw(i % 16 + 21 + voffset, indent + 30 + floor(rN * 10), "#");
			attroff(COLOR_PAIR(colorpair));
			for (size_t j = 0; j < 4; j ++) {
				attron(COLOR_PAIR(10 + j));
				mvprintw(22 + j + voffset, indent + 55, "%4ld / %4ld, %6g %%", colorstat[j], i, colorstat[j] / (double) i * 100.);
				attroff(COLOR_PAIR(10 + j));
				if (j == 2 && colorpair != 13) { break; }
			}
			if (i == 1) mvprintw(19 + voffset, indent, "-3.......-2........-1.........0.........1.........2.........3.");
			refresh();
		} break;
	}
	clear(); refresh();
	return;
}

int main(const int argc, const char* argv[]) {
	initscr();
	cbreak();
	noecho();
	start_color();
	curs_set(0);
	clear();
	refresh();

	various_tests();

	SkinQuality refsq = {0.999999, 0.01, 1.001, 0.01};
	HairStat refhs = {2., .5, 0., 0., .1, .05, 0., 500., 0.};
	HeadSkin myhsk;
	init_headskin(&myhsk, 100000, refhs, refsq);
	double day = 0;	
	double deltaDay = 0.25;
	while (1) {
		offset_time(&myhsk, deltaDay);
		day += deltaDay;
		mvprintw(3, 0, 
			 "    STATS\n\n"\
			 "    dycount  |     %g\n\n"\
			 "    avr-len  |     %g\n"\
			 "    var-len  |     %g\n"\
			 "    avr-age  |     %g\n"\
			 "    var-age  |     %g\n"\
			 "    avr-spd  |     %g\n"\
			 "    var-spd  |     %g\n\n"\
			 "    ttl-len  |     %g\n"\
			 "    HLpcspd  |     %g\n\n",
			 day,
			 myhsk.hs.avrLen,
			 myhsk.hs.varLen,
			 myhsk.hs.avrAge,
			 myhsk.hs.varAge,
			 myhsk.hs.avrSpd,
			 myhsk.hs.varSpd,
			 myhsk.hs.totalLen,
			 myhsk.hs.hairlossSpd
			 );
		refresh();
	}
	

	endwin();
	return 0;
}
