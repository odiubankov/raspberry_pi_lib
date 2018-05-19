//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#pragma once

#include <stdint.h>

namespace raspby {

class GpioPin;

// Memory mapped input output. Communication with GPIO pins of Raspberry PI
class MemoryMappedIO
{
public:
    MemoryMappedIO();

    void setOutput(const GpioPin& pin) const;
    void setInput(const GpioPin& pin) const;

    void setHigh(const GpioPin& pin) const;
    void setLow(const GpioPin& pin) const;

    uint32_t input(const GpioPin& pin) const;

private:
    void resetPinBitsToZero(const GpioPin& pin) const;
    volatile uint32_t* getPinRegisterAddress(const GpioPin& pin) const;
    static unsigned getPinBitsMask(unsigned configBits, const GpioPin& pin);

    volatile uint32_t* mmioGpio_{ nullptr };
};

}
