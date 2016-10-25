##Node addon for hardware SI1145 sensor

#####This addon should work on any Linux platform, but has only been tested on BBB

###Install

```
npm install @agilatech/si1145
```
OR
```
git clone https://github.com/Agilatech/si1145.git
node-gyp configure build
```
###Usage
#####Load the module and create and instance
```
const addon = require('si1145');

// create an instance on the I2C bus 2 at address 0x60
const si1145 = new addon.Si1145('/dev/i2c-2', 0x60);
```
#####Get basic device info
```
const name = si1145.si1145();  // returns string with name of device
const version = si1145.deviceVersion(); // returns this software version
const active = si1145.deviceActive(); // true if active, false if inactive
const numVals =  si1145.deviceNumValues(); // returns the number of paramters sensed
```
####Get paramter info and values
The SI1145 Sensor has IR, Visible, and UV parameters, so there are three corresponding indicies.
```
// index 0 is IR light in lux
const paramName0 = si1145.nameAtIndex(0);
const paramType0 = si1145.typeAtIndex(0);
const paramVal0  = si1145.valueAtIndexSync(0);

// index 1 is visible light in lux
const paramName1 = si1145.nameAtIndex(1);
const paramType1 = si1145.typeAtIndex(1);
const paramVal1  = si1145.valueAtIndexSync(1);

// index 2 is UV index weighted according to the CIE Erythemal Action Spectrum
const paramName2 = si1145.nameAtIndex(2);
const paramType2 = si1145.typeAtIndex(2);
const paramVal2  = si1145.valueAtIndexSync(2);
```
####Asynchronous value collection is also available
```
si1145.valueAtIndex(0, function(err, val) {
    if (err) {
        console.log(err);
    }
    else {
        console.log(`Asynchronous call return: ${val}`);
    }
});
```
###Operation Notes
The driver software does not include any functionality for proximity sensing.  This feature may be enabled in a future
release, but don't count on it. The main purpose of this addon is for simple light sensing.

The UV Index provided by the SI1145 sensor is not based on direct UV measurement, but is calculated by the linear
releationship of combined IR and Visible sunlight and is weighted according to the CIE Erythemal Action Spectrum.
The resulting UV index is the standardized World Health Organization simplified consumer UV exposure level ranging
from 1 to 11.

In any case, this sensor is uncalibrated, so none of the returned values should be considered accurate. The values
returned are in the right ballpark withing a 15-20% tolerance.

###Dependencies
* node-gyp

###Copyright
Copyright © 2016 Agilatech. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

