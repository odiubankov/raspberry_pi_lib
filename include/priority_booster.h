//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#pragma once

namespace raspby {

// RAII class
// Increases priority of the process to max in Construct and resets to default in Destruct
class PriorityBooster
{
public:
    PriorityBooster();
    ~PriorityBooster();
    PriorityBooster(const PriorityBooster&) = delete;
    PriorityBooster(PriorityBooster&&) = delete;
    PriorityBooster& operator=(const PriorityBooster&) = delete;
    PriorityBooster& operator=(PriorityBooster&&) = delete;

private:
    void setMaxPriority() const;
    void setDefaultPriority() const;
};

}
