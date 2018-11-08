## Driver for SI1145 sensor

##### This driver should work on any Linux platform, and has been thoroughly tested on BBB and RPi

### Install
```
npm install @agilatech/si1145
```

### Usage
##### Load the module and create an instance
```
const Si1145 = require('@agilatech/si1145');
const si1145 = new Si1145();
// creates an driver with all default options, including i2c-1 bus at 0x60 address.

if (si1145.isActive()) {
    si1145.getDataFromDevice((err) => {
        if (!err) {
            const visible = si1145.device.parameters[0].value;
            const ir      = si1145.device.parameters[1].value;
            const uvIdx   = si1145.device.parameters[2].value;
        }
    });
}
```

**Options**
```
Si1145([options])
```
The constructor can be supplied with options to specify certain driver characteristics. The options object allows the following parameters:
* bus : The number of the I2C bus. The number 1 indicates '/dev/i2c-1'. Defaults to 1.
* addr : The device address on the bus. Defaults to 0x60
* name : The name given to this particular driver instance. Defaults to 'Si1145'.
* type : The type given to this particular driver instance. Defaults to 'sensor'.


### Get basice device info
```
const name = si1145.deviceName();  // returns string with name of device
const type = si1145.deviceType();  // returns string with type of device
const version = si1145.deviceVersion(); // returns this driver software version
const active = si1145.deviceActive(); // true if initialized and acgtive, false if inactive
const numVals =  si1145.deviceNumValues(); // returns the number of paramters sensed
```

### Take measurement and load results in device object
```
Si1145.getDataFromDevice(callback)
```
Asyncronously polls the device, stores the results in the device parameters object, and then calls the optional callback. The given callback function takes an error paramter through which to report errors.  Upon completion, the device object may be examined for new values.

```
Si1145.getDataFromDeviceSync()
```
Synchronously polls the device, stores the results in the device parameters object, returning true on success and false on failure.

#### device object
The Si1145.device object contains basic information about the device itself, and also the 'parameters' array. The 'parameters' array contains the name, data type, and current value of each parameter for the device.
```
Si1145.device.parameters[0] = {
    name: 'visible',
    type: 'integer',
    value: <current visible light lux>
    }

Si1145.device.parameters[1] = {
    name: 'ir',
    type: 'integer',
    value: <current infrared lux>
    }
 
Si1145.device.parameters[2] = {
    name: 'uv',
    type: 'integer',
    value: <current UV index from 0-11>
    }
```


### Get individual parameter values by index
Asynchronously:
```
si1145.valueAtIndex(index, (err, value) => {
    if (!err) {
        val = value;
    }
});
```
Synchronously:
```
val = si1145.getValueAtIndexSync(index);
// returns NaN if upon error
```


#### Get parameter info and values
The SI1145 Sensor has Visible, IR, and UV parameters, so there are three corresponding indicies.
```
// index 0 is visible light in lux
const paramName0 = si1145.nameAtIndex(0);
const paramType0 = si1145.typeAtIndex(0);
const paramVal0  = si1145.valueAtIndexSync(0);

// index 1 is IR light in lux
const paramName1 = si1145.nameAtIndex(1);
const paramType1 = si1145.typeAtIndex(1);
const paramVal1  = si1145.valueAtIndexSync(1);

// index 2 is UV index weighted according to the CIE Erythemal Action Spectrum
const paramName2 = si1145.nameAtIndex(2);
const paramType2 = si1145.typeAtIndex(2);
const paramVal2  = si1145.valueAtIndexSync(2);
```


### Startup time
The constructor initializes the hardware device, but this takes some time. Usually less than 25ms is required for startup, and during this time other operations will fail.  Therefore, it is a best practice to check 'deviceActive()' returns true before initiating any measurement.


### Reset
The device is reset and initialized on startup, and it is not normally necessary to reset the device after this time.  However, the initialize function can be explicitly called:
```
si1145.initialize()
```
Will reset the device, and initialize. Normal startup time delay applies.


### Operation Notes
The driver software does not include any functionality for proximity sensing.  This feature may be enabled in a future release, but don't count on it. The main purpose of this addon is for simple light sensing.

The UV Index provided by the SI1145 sensor is not based on direct UV measurement, but is calculated by the linear releationship of combined IR and Visible sunlight and is weighted according to the CIE Erythemal Action Spectrum. The resulting UV index is the standardized World Health Organization simplified consumer UV exposure level ranging from 1 to 11.

In any case, this sensor is uncalibrated, so none of the returned values should be considered accurate. The values returned are in the right ballpark within a 15-20% tolerance.


### Dependencies
* i2c-bus is used to communicate with the device on the i2c bus


### Copyright
Copyright © 2017-2018 Agilatech. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
