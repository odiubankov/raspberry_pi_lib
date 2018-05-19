//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#pragma once

#include <chrono>

namespace raspby {

// Keep thread busy for the provided duration
void busyWaitFor(std::chrono::milliseconds duration);

// Keep thread busy for very short period of time
void shortBusyWait();

}
