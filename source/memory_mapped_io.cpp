//
// Copyright(c) 2018 Oleksii Diubankov
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//
#include "memory_mapped_io.h"
#include "gpio_pin.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdexcept>
#include <array>
#include <fstream>

namespace  {
    constexpr uint32_t  GpioBaseOffset              = 0x200000;
    constexpr size_t    GpioLength                  = 4096;
    constexpr unsigned  NumberOfBitsPerPinConfig    = 3;
    constexpr unsigned  PinConfigBitsMask           = 0b111;
    constexpr unsigned  NumberOfPinsPerRegister     = 10;
    constexpr unsigned  PinOutputBitsMask           = 0b001;
    constexpr long      PerBaseRegsInfoOffset       = 4;
}

namespace raspby {

MemoryMappedIO::MemoryMappedIO()
{
    // Check for GPIO and peripheral addresses from device tree.
    // proc - folder in 'Linux Filesystem Hierarchy'
    //        process information pseudo-file system
    //        http://www.tldp.org/LDP/Linux-Filesystem-Hierarchy/html/proc.html
    //
    // device-tree/soc/ranges - path, specific for raspberry pi
    // for example, device-tree/model file contains text 'Raspberry Pi 3 Model B Rev 1.2'
    // Here is link to information about raspberry pi device tree https://www.raspberrypi.org/documentation/configuration/device-tree.md
    //
    // 'soc' states for System on a Chip. More information here: https://www.wikiwand.com/en/System_on_a_chip
    //
    // 'ranges' - access to gpio - general purpose input output.
    //            more information here
    // https://www.iot-programmer.com/index.php/books/22-raspberry-pi-and-the-iot-in-c/chapters-raspberry-pi-and-the-iot-in-c/59-raspberry-pi-and-the-iot-in-c-memory-mapped-gpio?showall=&limitstart=
    std::ifstream registersInfoFile;
    registersInfoFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    registersInfoFile.open("/proc/device-tree/soc/ranges",
                           std::ios_base::in | std::ios_base::binary);
    // Peripheral base registers info is located at bytes 5-8. Skip first 4 bytes
    registersInfoFile.seekg(PerBaseRegsInfoOffset);

    // buffer to persist values of 4 bytes
    std::array<char, 4> buf;
    registersInfoFile.read(&buf[0], sizeof(buf));

    // compound from these 4 bytes a 32 bit value
    uint32_t peri_base = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0;
    // peri_base on 'raspberry pi 3' equals in binary 00111111 00000000 00000000 00000000
    //                                      in hex    3f000000
    auto gpio_base = peri_base + GpioBaseOffset;
    auto fd = ::open("/dev/gpiomem", O_RDWR | O_SYNC);
    if (fd == -1)
        throw std::runtime_error{ "Error opening /dev/gpiomem" };

    // Map GPIO memory to location in process space.
    mmioGpio_ = static_cast<uint32_t*>(::mmap(nullptr, GpioLength, PROT_READ | PROT_WRITE, MAP_SHARED, fd, gpio_base));
    ::close(fd);

    if (mmioGpio_ == MAP_FAILED)
    {
      // Don't save the result if the memory mapping failed.
      mmioGpio_ = nullptr;
      throw std::runtime_error{ "Memory mapping failed" };
    }
}


void MemoryMappedIO::setOutput(const GpioPin& pin) const
{
    resetPinBitsToZero(pin.getNumber());

    volatile auto pinRegisterAddress = getPinRegisterAddress(pin.getNumber());
    auto pinBitsMask = getPinBitsMask(PinOutputBitsMask, pin.getNumber());
    *pinRegisterAddress |= pinBitsMask;
}


void MemoryMappedIO::setInput(const GpioPin& pin) const
{
    resetPinBitsToZero(pin.getNumber());
}


void MemoryMappedIO::setHigh(const GpioPin& pin) const
{
//    volatile auto setHighAddress = mmioGpio + 7;
//    std::cout << "setHighAddress" << const_cast<void*>(static_cast<volatile void*>(setHighAddress)) << std::endl;
    *(mmioGpio_+7) = 1 << pin.getNumber();
}


void MemoryMappedIO::setLow(const GpioPin& pin) const
{
//    volatile auto setLowAddress = mmioGpio + 10;
//    std::cout << "setLowAddress" << const_cast<void*>(static_cast<volatile void*>(setLowAddress)) << std::endl;
    *(mmioGpio_+10) = 1 << pin.getNumber();
}


void MemoryMappedIO::resetPinBitsToZero(const GpioPin& pin) const
{
    volatile auto pinRegisterAddress = getPinRegisterAddress(pin);
    auto pinBitsMask = getPinBitsMask(PinConfigBitsMask, pin);
    auto invertedPinBitsMask = ~pinBitsMask;
    *pinRegisterAddress &= invertedPinBitsMask;
}


unsigned MemoryMappedIO::getPinBitsMask(const unsigned configBits, const GpioPin& pin)
{
    auto pinIndexWithinTheRegister = pin.getNumber() % NumberOfPinsPerRegister;
    auto pinFirstBitIndex = pinIndexWithinTheRegister * NumberOfBitsPerPinConfig;
    return configBits << pinFirstBitIndex;
}


volatile uint32_t* MemoryMappedIO::getPinRegisterAddress(const GpioPin& pin) const
{
    auto pinRegisterNumber = pin.getNumber() / NumberOfPinsPerRegister;
    return mmioGpio_ + pinRegisterNumber;
}


uint32_t MemoryMappedIO::input(const GpioPin& pin) const
{
//    volatile auto inputAddress = mmioGpio_ + 13;
//    std::cout << "inputAddress" << const_cast<void*>(static_cast<volatile void*>(inputAddress)) << std::endl;
    return *(mmioGpio_+13) & (1 << pin.getNumber());
}

}
