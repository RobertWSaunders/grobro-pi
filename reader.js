const SerialPort = require('serialport');
const createInterface = require('readline').createInterface;

const lineReader = createInterface({
	input: new SerialPort(process.env.SERIAL_INPUT),
	output: new SerialPort(process.env.SERIAL_OUTPUT)
});

module.exports = lineReader;
