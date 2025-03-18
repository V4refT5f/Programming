#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>
#include <errno.h>

/**
 * Windows implementation of POSIX nanosleep
 * 
 * @param req The requested time to sleep
 * @param rem If the sleep is interrupted, this contains the remaining time
 * @return 0 on success, -1 on error with errno set
 */
int nanosleep(const struct timespec *req, struct timespec *rem) {
    // Create a new timer for each call instead of using a static one
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    
    // Validate input
    if (req == NULL) {
        errno = EINVAL;
        return -1;
    }
    
    if (req->tv_sec < 0 || req->tv_nsec < 0 || req->tv_nsec >= 1000000000L) {
        errno = EINVAL;
        if (timer) CloseHandle(timer);
        return -1;
    }
    
    // Check if timer creation succeeded
    if (timer == NULL) {
        errno = ENOTSUP;  // No system support
        return -1;
    }
    
    // Calculate sleep time in 100-nanosecond intervals
    // Windows uses negative values for relative time
    LARGE_INTEGER dueTime;
    dueTime.QuadPart = -((LONGLONG)req->tv_sec * 10000000LL + 
                         (LONGLONG)req->tv_nsec / 100LL);
    
    // Set and wait for timer
    if (!SetWaitableTimer(timer, &dueTime, 0, NULL, NULL, FALSE)) {
        errno = ENOTSUP;
        CloseHandle(timer);
        return -1;
    }
    
    DWORD result = WaitForSingleObject(timer, INFINITE);
    
    // Always clean up the timer
    CloseHandle(timer);
    
    if (result == WAIT_OBJECT_0) {
        // Sleep completed successfully
        if (rem) {
            rem->tv_sec = 0;
            rem->tv_nsec = 0;
        }
        return 0;
    }
    else if (result == WAIT_ABANDONED || result == WAIT_FAILED) {
        // Error occurred - no good way to get the remaining time
        if (rem) {
            rem->tv_sec = req->tv_sec;
            rem->tv_nsec = req->tv_nsec;
        }
        errno = EINTR;
        return -1;
    }
    
    return 0;
}
#endif

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

struct timespec sec_to_timespec(double secs) {
	if (secs <= 0.0) { return (struct timespec) {0, 0L}; }
	double secpart = floor(secs);
	double nanopart = (secs - secpart) * 1000000000.0;
	return (struct timespec) {(int) secpart, (long) nanopart}; 
}

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
void say(const sayfstr text, const SayConfig* config);

// Latest C standard. The proof that I'm not an outdated old guy.
int main(/*[[maybe_unused]]*/const int argc,/*[[maybe_unused]]*/ const char* argv[]) {
	printf("Beginning...\n");
	setbuf(stdout, NULL);
	SayConfig cfg = (SayConfig) {
		0.1, 0.3, 0.1 
	};
	if (argc >= 1) { say((sayfstr)argv[0], &cfg); printf("\n"); }
	say(invisible_land(), &cfg);
	return 0;
}

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

void say(const sayfstr text, const SayConfig* config) {
	for (char* c = text; *c != '\0'; c ++) {
		double rn = 0.0;
		char* rp = c;
		_control_char_test(c, &rn, &rp);
		struct timespec ttmslp = sec_to_timespec(rn * (config->baseDelaySec + (rand_double() - 0.5) * config->randRangeSec) + config->charDelay + (rand_double() - 0.5) * config->randRangeSec);
		nanosleep(&ttmslp, NULL);
		c = rp; putchar(*c); // printf("\n");
	}
	return;	
}
