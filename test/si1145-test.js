const chai = require('chai');
const si1145 = require('../si1145');
const expect = chai.expect;
var Si1145;

describe('SI1145', function() {
  before(function(done) {
    
    Si1145 = new si1145();

    // takes just a wee bit of time to startup
    var waiting = setTimeout(function wait() {
      if (Si1145.deviceActive()) {
          clearTimeout(waiting);
          done();
      }
      else {
          waiting = setTimeout(wait, 25);
      }
    }, 25);

  });

  it ('should activate the sensor device', function() {
    expect(Si1145.deviceActive()).to.be.true;
  });

  it ('should set basic device info', function() {
    const ver = require('../package').version;
    expect(Si1145.deviceName()).to.equal('Si1145');
    expect(Si1145.deviceType()).to.equal('sensor');
    expect(Si1145.deviceVersion()).to.equal(ver);
  });

  it ('should have three parameters', function() {
    expect(Si1145.deviceNumValues()).to.equal(3);
  });

  it ('should set parameter names', function() {
    expect(Si1145.nameAtIndex(0)).to.equal('visible');
    expect(Si1145.nameAtIndex(1)).to.equal('ir');
    expect(Si1145.nameAtIndex(2)).to.equal('uv');
  });

  it ('should set parameter types', function() {
    for (let i = 0; i < Si1145.deviceNumValues(); i++) {
      expect(Si1145.typeAtIndex(i)).to.equal('integer');
    }
  });

  it ('should asynchronously get data from the sensor device without error', function(done) {
    Si1145.getDataFromDevice(function(err) {
      if (err) done (err);
      else done();
    });
  });

  it ('should have collected a valid visible light lux', function(){
    expect(Si1145.device.parameters[0].value).to.be.within(0, 2000);
  });

  it ('should have collected a valid IR lux', function(){
    expect(Si1145.device.parameters[1].value).to.be.within(0, 20000);
  });

  it ('should have collected a valid UV index', function(){
    expect(Si1145.device.parameters[2].value).to.be.within(0, 11);
  });

  it ('should synchronously get data from the sensor device without error', function() {
    Si1145.getDataFromDeviceSync();
    expect(Si1145.device.parameters[0].value).to.be.within(0, 2000);
    expect(Si1145.device.parameters[1].value).to.be.within(0, 20000);
    expect(Si1145.device.parameters[2].value).to.be.within(0, 11);
  });

  it ('should asyncronously collect visible light lux', function(done){
    Si1145.valueAtIndex(0, function(err, value) {
      if (err) {
        done(err);
      }
      else {
        expect(value).to.be.within(0, 2000);
        done();
      }
    });
  });

  it ('should asyncronously collect IR lux', function(done){
    Si1145.valueAtIndex(1, function(err, value) {
      if (err) {
        done(err);
      }
      else {
        expect(value).to.be.within(0, 20000);
        done();
      }
    });
  });

  it ('should asyncronously collect UV index', function(done){
    Si1145.valueAtIndex(2, function(err, value) {
      if (err) {
        done(err);
      }
      else {
        expect(value).to.be.within(0, 11);
        done();
      }
    });
  });

  it ('should synchronously collect visible light lux', function(){
    expect(Si1145.valueAtIndexSync(0)).to.be.within(0, 2000);
  });

  it ('should synchronously collect IR lux', function(){
    expect(Si1145.valueAtIndexSync(1)).to.be.within(0, 20000);
  });

  it ('should synchronously collect UV index', function(){
    expect(Si1145.valueAtIndexSync(2)).to.be.within(0, 11);
  });

  it ('should asyncronously error if an out-of-bounds index is requested', function(done) {
    Si1145.valueAtIndex(4, function(err, value) {
      if (err) {
        done();
      }
      else {
        done('Error: out-of-bounds index was not rejected');
      }
    });
  })

  it ('should synchronously error if an out-of-bounds index is requested', function(){
    expect(Si1145.valueAtIndexSync(-1)).to.be.NaN;
  }); 

});