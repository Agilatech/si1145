/**
 * \file Si1145Drv.h
 *
 *  Created by Scott Erholm on 10/24/16.
 *  Copyright (c) 2016 Agilatech. All rights reserved.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __Si1145Drv__
#define __Si1145Drv__

#include <iostream>
#include <fstream>
#include <cmath>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "I2CDevice.h"
#include "DataManip.h"
#include "Si1145DrvDefines.h"

#ifdef DEBUG
#  define DPRINT(x) do { std::cerr << x; std::cerr << std::endl; } while (0)
#else
#  define DPRINT(x) do {} while (0)
#endif


static const std::string name = "SI1145";
static const std::string version = "0.8.0";

static const int numValues = 3;

static const std::string valueNames[numValues] = {"ir", "visible", "uv"};
static const std::string valueTypes[numValues] = {"integer", "integer", "integer"};

class Si1145Drv : public i2cbus::I2CDevice {
    
public:
    Si1145Drv(std::string devfile, uint32_t addr);
    
    static std::string getVersion();
    static std::string getSi1145();
    static int getNumValues();
    static std::string getTypeAtIndex(int index);
    static std::string getNameAtIndex(int index);
    
    bool isActive();
    std::string getValueAtIndex(int index);
    
protected:
    
    bool initialize();
    std::string readValue0();
    std::string readValue1();
    std::string readValue2();
    
private:
    void reset();
    uint8_t writeParam(uint8_t p, uint8_t v);
    uint8_t readParam(uint8_t p);
    uint16_t read16(uint32_t reg);
    
    bool active = false;
        
};

#endif /* defined(__Si1145Drv__) */
