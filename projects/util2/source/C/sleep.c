#include "util2/C/sleep.h"
#include <stdlib.h>
#include "util2/C/macro.h"
#include "util2/C/tinycthread.h"


#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#else
#   include <time.h>
#   include <errno.h>
#   ifdef __APPLE__
#       include <mach/clock.h>
#       include <mach/mach.h>
#   endif
#endif // _WIN32


#ifdef _WIN32


static HANDLE   s_wintimer        = NULL;
static uint64_t s_wintimerfreq    = 0;
static double   s_wintimerinvfreq = 0;
static mtx_t    s_exitMutex;


static void closeTimerHandle() {
    CloseHandle(s_wintimer);
    return;
}

void microsleep(uint32_t microseconds) {
    Sleep(microseconds);
    return;
}


void millisleep(uint32_t milliseconds) {
    LARGE_INTEGER ft;
    ft.QuadPart = -1 * __scast(int64_t, milliseconds);  // '-' using relative time

    
    if(unlikely(s_wintimer == NULL)) {
        /* first time we pay the price for the branch, next time will be fine */
        mtx_init(&s_exitMutex, mtx_plain | mtx_recursive);
        s_wintimer = CreateWaitableTimer(NULL, TRUE, NULL);
        if(atexit(closeTimerHandle) != 0) {
            while(mtx_trylock(&s_exitMutex) != thrd_success) {}
            exit(-1);
            mtx_unlock(&s_exitMutex);
        }
    }


    SetWaitableTimer(s_wintimer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(s_wintimer, INFINITE);
    return;
}


uint64_t nowms() {
    LARGE_INTEGER tmp;
    double        accurateResult = 0;

    if( unlikely(s_wintimerinvfreq == 0) ) {
        /* first time we pay the price for the branch, next time will be fine */
        QueryPerformanceFrequency(&tmp);
        s_wintimerfreq = tmp.QuadPart;
        s_wintimerinvfreq = 1.0 / s_wintimerinvfreq;
    }


    QueryPerformanceCounter(&tmp);
    accurateResult = tmp.QuadPart * 1000000;
    accurateResult *= s_wintimerinvfreq;
    return __scast(uint64_t, accurateResult);
}
#else


void microsleep(uint32_t microseconds) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = ms % 1000 * 1000000;

    while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
    return;
}


void millisleep(uint32_t milliseconds) {
    struct timespec ts;
    ts.tv_sec = us / 1000000;
    ts.tv_nsec = us % 1000000 * 1000;

    while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
    return;
}


#ifdef __APPLE__
uint64_t nowms() {
    clock_serv_t cs;
    mach_timespec_t ts;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cs);
    clock_get_time(cs, &ts);
    mach_port_deallocate(mach_task_self(), cs);

    return __scast(uint64_t, ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
}
#else
uint64_t nowms() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return __scast(uint64_t, now.tv_sec) * 1000000 + now.tv_nsec / 1000;
}
#endif /* __APPLE__ */


#endif /* _WIN32 */
