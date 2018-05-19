//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "timing.h"

#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <thread>

namespace raspby {

void busyWaitFor(std::chrono::milliseconds duration)
{
    // Set delay time period.
    struct timeval deltatime;
    deltatime.tv_sec = duration.count() / 1000;
    deltatime.tv_usec = (duration.count() % 1000) * 1000;
    struct timeval walltime;
    // Get current time and add delay to find end time.
    gettimeofday(&walltime, NULL);
    struct timeval endtime;
    timeradd(&walltime, &deltatime, &endtime);
    // Tight loop to waste time (and CPU) until enough time as elapsed.
    while (timercmp(&walltime, &endtime, <)) {
      gettimeofday(&walltime, NULL);
    }

}


void shortBusyWait()
{
    for (volatile int i = 0; i < 100; ++i);
}

}
