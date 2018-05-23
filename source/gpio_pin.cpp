//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "gpio_pin.h"

#include <stdexcept>

namespace raspby {

GpioPin::GpioPin(unsigned number)
    : number_{number}
{}

}
