
const driver = require('../driver/si1145');

var si1145 = new driver();

const test = function() {
    var output = {};
    var data = {};
    var names = [];
    var types = [];
    var values = [];

    output['name'] = si1145.deviceName();
    output['type'] = si1145.deviceType();
    output['version'] = si1145.deviceVersion();
    output['active'] = si1145.deviceActive();
        
    for (var i = 0; i < si1145.deviceNumValues(); i++) {
        names.push(si1145.nameAtIndex(i));
        types.push(si1145.typeAtIndex(i));
        values.push(si1145.valueAtIndexSync(i));
    }

    data['names'] = names;
    data['types'] = types;
    data['values'] = values;

    output['data'] = data;

    var jsonOut = JSON.stringify(output);

    console.log(jsonOut);

    si1145.valueAtIndex(0, function(err, val) {
        if (!err) {
            console.log(`Asynchronous call 0 return: ${val}`);
        }
    });

    si1145.valueAtIndex(1, function(err, val) {
        if (!err) {
            console.log(`Asynchronous call 1 return: ${val}`);
        }
    });

    si1145.valueAtIndex(2, function(err, val) {
        if (!err) {
            console.log(`Asynchronous call 2 return: ${val}`);
        }
    });
    
}
test();