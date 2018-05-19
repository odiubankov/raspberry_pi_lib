//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "priority_booster.h"

#include <sched.h>
#include <string.h>

namespace {

sched_param createSchedParam(int priority)
{
    sched_param schedParam;
    ::memset(&schedParam, 0, sizeof(schedParam));
    schedParam.sched_priority = priority;
    return schedParam;
}

}

namespace raspby {

PriorityBooster::PriorityBooster()
{
    setMaxPriority();
}


PriorityBooster::~PriorityBooster()
{
    setDefaultPriority();
}


void PriorityBooster::setMaxPriority() const
{
    // Use FIFO scheduler with highest priority for the lowest chance of the kernel context switching.
    auto priority = ::sched_get_priority_max(SCHED_FIFO);
    auto schedParam = createSchedParam(priority);
    ::sched_setscheduler(0, SCHED_FIFO, &schedParam);
}


void PriorityBooster::setDefaultPriority() const
{
    // Go back to default scheduler with default 0 priority.
    auto schedParam = createSchedParam(0);
    ::sched_setscheduler(0, SCHED_OTHER, &schedParam);
}

}
