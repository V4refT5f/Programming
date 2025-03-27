#ifndef _250313_H
#define _250313_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include "win_nanosleep.h"


#ifndef _RAND_BLOCK
#define _RAND_BLOCK

static int RANDSEED = 5;
int rand_int() {
	RANDSEED ^= RANDSEED << 3;
	RANDSEED ^= RANDSEED << 7;
	RANDSEED ^= RANDSEED << 11;
	return RANDSEED;
}

double rand_double() {
	return (double) rand_int() / (double) INT_MAX;
}

#endif

// It's said that programming is just naming things.
// This is just a null terminated char array, nothing more.
typedef char* sayfstr;

typedef struct SayConfig {
	double charDelay;
	double baseDelaySec;
	double randRangeSec;
} SayConfig;

// Try a different naming scheme...
// Always return a char* that needs freeing.
sayfstr invisible_land();

// Says the stuff.
// Is blocking.
void say(const sayfstr text, const SayConfig* config, int (*_putchar)(int));

// Latest C standard. The proof that I'm not an outdated old guy.
//int main([[maybe_unused]]const int argc, [[maybe_unused]] const char* argv[]) {
	// printf("Beginning...\n");
//	setbuf(stdout, NULL);
	// SayConfig cfg = (SayConfig) {
	// 	0.07, 0.3, 0.1 
	// };
	// if (argc >= 1) { say((sayfstr)argv[0], &cfg); printf("\n"); }
	// say(invisible_land(), &cfg);
//	return 0;
//}

sayfstr invisible_land() {
	const sayfstr words = 
	"Today is ^3.5...^4 Monday, no, Thursday...!^7 ^^_^^233\n";
	return words;	
}

int _is_number(char c) { return (c >= '0' && c <= '9') ? 1 : (c == '.') ? 2 : 0; }

void _control_char_test(char* c, double* reportedNumber, char** reportedPoz) {
	if (*(c+1) == '\0' || *c != '^') { r: *reportedPoz = c; *reportedNumber = 0.0; return; }
	if (*(c+1) == '^') { *reportedPoz = c + 1; *reportedNumber = 0.0; return; }
	if (!_is_number(*(c+1))) { goto r; }
	double rawNumber = 0.0;
	double divideRatio = 1.0;
	int startedCounting = 0;
	c ++;
	for (int sw = 0; _is_number(*c) == 1 || (_is_number(*c) == 2 && startedCounting == 0); c ++) {
		if (startedCounting) divideRatio *= 10;
		sw = _is_number(*c);
		if (sw == 1) rawNumber = rawNumber * 10 + *c - '0';
		if (sw == 2) startedCounting = 1;
	}  
	rawNumber /= divideRatio;
	*reportedNumber = rawNumber;
	*reportedPoz = c;
	// printf("Control Char encountered at %p. *RP: %c, RN: %g.\n", c, **reportedPoz, rawNumber);
	return;
}

void say(const sayfstr text, const SayConfig* config, int (*_putchar)(int)) {
	for (char* c = text; *c != '\0'; c ++) {
		double rn = 0.0;
		char* rp = c;
		_control_char_test(c, &rn, &rp);
		struct timespec ttmslp = sec_to_timespec(rn * (config->baseDelaySec + (rand_double() - 0.5) * config->randRangeSec) + config->charDelay + (rand_double() - 0.5) * config->randRangeSec);
		nanosleep(&ttmslp, NULL);
		c = rp; _putchar(*c); // printf("\n");
	}
	return;	
}

typedef struct {
	char* ch;
	double time;
} SayCharTiming;

static SayCharTiming* SCNB_timing = NULL;
static double SCNB_beginSec = -1.0;
static size_t SCNB_progress = 0;
// static double SCNB_elapsedSec = -1.0;

void say_nonbuf_init(const sayfstr text, const SayConfig* config, double beginSec) {
	if (SCNB_timing) { free(SCNB_timing); }
	SCNB_timing = (SayCharTiming*) calloc(strlen(text) + 1, sizeof(SayCharTiming));
	SCNB_beginSec = beginSec;
	SCNB_progress = 0;
	// SCNB_elapsedSec = 0.0;
	
	size_t index = 0;
	double sec = 0.0;
	
	for (char* c = text; *c != '\0'; c ++) {
		double rn = 0.0;
		char* rp = c;
		_control_char_test(c, &rn, &rp);
		sec += rn * (config->baseDelaySec + (rand_double() - 0.5) * config->randRangeSec) + config->charDelay + (rand_double() - 0.5) * config->randRangeSec;
		c = rp; SCNB_timing[index].ch = c; SCNB_timing[index].time = sec;
		index ++;
	}
	SCNB_timing[index].time = 99999999.0;
	return;
}

void say_nonbuf_process(int (*_putchar)(int), double sec) {
	if (!SCNB_timing) { return; }
	double target = sec - SCNB_beginSec;
	while (SCNB_timing[SCNB_progress].time <= target) {
		_putchar(*(SCNB_timing[SCNB_progress].ch));
		SCNB_progress ++;
	}
	if (SCNB_timing[SCNB_progress].time >= 99999998.0) { free(SCNB_timing); }
	return;
}

void say_nonbuf_skip(int (*_putchar)(int)) {
	if (!SCNB_timing) { return; }
	while (SCNB_timing[SCNB_progress].time < 99999998.0) {
		_putchar(*(SCNB_timing[SCNB_progress].ch));
		SCNB_progress ++;
	}
	free(SCNB_timing);
	return;
}

#endif
