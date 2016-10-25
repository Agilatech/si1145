/**
 * \file Si1145Drv.cpp
 *
 *  Created by Scott Erholm on 10/24/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "Si1145Drv.h"

Si1145Drv::Si1145Drv(std::string devfile, uint32_t addr):i2cbus::I2CDevice(devfile,addr) {
    
    if (initialize()) {
        this->active = true;
    }
    else {
        std::cerr << name << " did not initialize. " << name << " is inactive" << std::endl;
    }
    
}

std::string Si1145Drv::getVersion() {
    return name + " " + version;
}

std::string Si1145Drv::getSi1145() {
    return name;
}

int Si1145Drv::getNumValues() {
    return numValues;
}

std::string Si1145Drv::getTypeAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueTypes[index];
}

std::string Si1145Drv::getNameAtIndex(int index) {
    if ((index < 0) || (index > (numValues - 1))) {
        return "none";
    }
    
    return valueNames[index];
}

bool Si1145Drv::isActive() {
    return this->active;
}

std::string Si1145Drv::getValueAtIndex(int index) {
    
    if (!this->active) {
        return "none";
    }

    if (index == 0) {
        return this->readValue0();
    }
    else if (index == 1) {
        return this->readValue1();
    }
    else if (index == 2) {
        return this->readValue2();
    }
    else {
        return "none";
    }
    
}

bool Si1145Drv::initialize() {
    
    reset();
    
    // Calibration is performed by setting the coefficients in UCOEF [0:3]
    // registers to the proper values.  The problem is that the proper values
    // are derived by measuring values when the hardware is held in absolute
    // darkness, and then subjecting the resuling values to a complex set of
    // calculations. However, if we wanted to collect scientifically accurate
    // light measurements, we would not use this crappy hardware sensor in the
    // first place, so estimating the coefficients is good enough.
    
    /* The default values for UCOEF[0:3] are 0x7B, 0x6B, 0x01, and 0x00.
    writeRegister(SI1145_REG_UCOEFF0, 0x7B);
    writeRegister(SI1145_REG_UCOEFF1, 0x6B);
    writeRegister(SI1145_REG_UCOEFF2, 0x01);
    writeRegister(SI1145_REG_UCOEFF3, 0x00);
    */
    
    /* These are the coefficeint values provided by Adafruit
     writeRegister(SI1145_REG_UCOEFF0, 0x29);
     writeRegister(SI1145_REG_UCOEFF1, 0x89);
     writeRegister(SI1145_REG_UCOEFF2, 0x02);
     writeRegister(SI1145_REG_UCOEFF3, 0x00);
    */
    
    // Split the difference and call it good!
    writeRegister(SI1145_REG_UCOEFF0, 0x52);
    writeRegister(SI1145_REG_UCOEFF1, 0x7A);
    writeRegister(SI1145_REG_UCOEFF2, 0x02);
    writeRegister(SI1145_REG_UCOEFF3, 0x00);
    
    // enable UV sensor
    writeParam(SI1145_PARAM_CHLIST, SI1145_PARAM_CHLIST_ENUV |
               SI1145_PARAM_CHLIST_ENALSIR | SI1145_PARAM_CHLIST_ENALSVIS |
               SI1145_PARAM_CHLIST_ENPS1);
    
    // enable interrupt on every sample
    writeRegister(SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);
    writeRegister(SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE);
    
    writeParam(SI1145_PARAM_ALSIRADCMUX, SI1145_PARAM_ADCMUX_SMALLIR);
    // fastest clocks, clock div 1
    writeParam(SI1145_PARAM_ALSIRADCGAIN, 0);
    // take 511 clocks to measure
    writeParam(SI1145_PARAM_ALSIRADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
    // in high range mode
    writeParam(SI1145_PARAM_ALSIRADCMISC, SI1145_PARAM_ALSIRADCMISC_RANGE);
    
    // fastest clocks, clock div 1
    writeParam(SI1145_PARAM_ALSVISADCGAIN, 0);
    // take 511 clocks to measure
    writeParam(SI1145_PARAM_ALSVISADCOUNTER, SI1145_PARAM_ADCCOUNTER_511CLK);
    // in high range mode (not normal signal)
    writeParam(SI1145_PARAM_ALSVISADCMISC, SI1145_PARAM_ALSVISADCMISC_VISRANGE);

    // measurement rate for auto
    writeRegister(SI1145_REG_MEASRATE0, 0xFF); // 255 * 31.25uS = 8ms
    
    // auto run
    writeRegister(SI1145_REG_COMMAND, SI1145_PSALS_AUTO);
    
    return true;
}

// returns IR light levels
std::string Si1145Drv::readValue0() {
    
    if (!this->active) {
        return "none";
    }
    
    uint16_t ir = read16(SI1145_REG_ALSIRDATA0);
    
    return DataManip::dataToString(ir);
}

// returns visible light levels
std::string Si1145Drv::readValue1() {
    
    if (!this->active) {
        return "none";
    }
    
    uint16_t vis = read16(SI1145_REG_ALSVISDATA0);
    
    return DataManip::dataToString(vis);
}

// returns UV index
std::string Si1145Drv::readValue2() {
    
    if (!this->active) {
        return "none";
    }
    
    // read the raw value, divide by 100, and then round to get the UV index
    uint16_t roundedUV = std::round(read16(SI1145_REG_UVINDEX0) / 100);
    
    return DataManip::dataToString(roundedUV);
}

void Si1145Drv::reset() {
    writeRegister(SI1145_REG_MEASRATE0, 0);
    writeRegister(SI1145_REG_MEASRATE1, 0);
    writeRegister(SI1145_REG_IRQEN, 0);
    writeRegister(SI1145_REG_IRQMODE1, 0);
    writeRegister(SI1145_REG_IRQMODE2, 0);
    writeRegister(SI1145_REG_INTCFG, 0);
    writeRegister(SI1145_REG_IRQSTAT, 0xFF);
    
    writeRegister(SI1145_REG_COMMAND, SI1145_RESET);
    usleep(10000);
    writeRegister(SI1145_REG_HWKEY, SI1145_REG_PARAMWR);
    
    usleep(10000);
}

uint8_t Si1145Drv::writeParam(uint8_t p, uint8_t v) {
    writeRegister(SI1145_REG_PARAMWR, v);
    writeRegister(SI1145_REG_COMMAND, p | SI1145_PARAM_SET);
    return readRegister(SI1145_REG_PARAMRD);
}

uint8_t Si1145Drv::readParam(uint8_t p) {
    writeRegister(SI1145_REG_COMMAND, p | SI1145_PARAM_QUERY);
    return readRegister(SI1145_REG_PARAMRD);
}

uint16_t Si1145Drv::read16(uint32_t reg) {
    uint16_t h; uint16_t l;
    
    l = readRegister(reg);
    h = readRegister(reg+1);
    
    return ((short)h<<8)|(short)l;
}

