#ifndef WIN_NANOSLEEP_H

#define WIN_NANOSLEEP_H
#include <time.h>
#include <math.h>

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

/**
 * Convert seconds to struct timespec.
 * 
 * @param secs Seconds
 */
struct timespec sec_to_timespec(double secs) {
	if (secs <= 0.0) { return (struct timespec) {0, 0L}; }
	double secpart = floor(secs);
	double nanopart = (secs - secpart) * 1000000000.0;
	return (struct timespec) {(int) secpart, (long) nanopart}; 
}

#endif
