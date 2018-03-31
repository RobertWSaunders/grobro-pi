const SerialPort = require('serialport');

const createInterface = require('readline').createInterface;

const port = new SerialPort('/dev/ttyACM1');

const lineReader = createInterface({
  input: port
});

lineReader.on('line', function(line) {
  console.log(line);
});
