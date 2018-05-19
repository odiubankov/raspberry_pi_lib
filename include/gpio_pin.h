//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#pragma once

namespace raspby {

// General purpose input output pin
class GpioPin
{
public:
    GpioPin(unsigned number);
    unsigned getNumber() const { return number_; }

private:
    unsigned number_;
};

}
