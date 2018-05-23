//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#pragma once

namespace raspby {

// General purpose input output pin
class GpioPin
{
private:
    GpioPin(unsigned number);

public:
    template<unsigned number>
    static GpioPin create()
    {
        static_assert(number >= 1 && number <= 27, "GPIO pin shall be between 1 and 27");
        return GpioPin(number);
    }

    unsigned getNumber() const { return number_; }

private:
    unsigned number_;
};

}
