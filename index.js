require('dotenv').config()

const logger = require('./logger');
const SerialPort = require('serialport');
const createInterface = require('readline').createInterface;
const { socket, socketActions } = require('./socket')(logger);

const sensorValueKeys = [
	'temperature',
	'humidity',
	'light',
	'heat',
	'humidifier'
];

const lineReader = createInterface({
	input: new SerialPort(process.env.SERIAL_INPUT)
});

lineReader.on('line', (line) => {

	const sensorValues = line.split(' ');

	const sensorData = sensorValues.reduce((acc, val, i) => {
		acc[sensorValueKeys[i]] = val;
		return acc;
	}, {});

	socket.emit(socketActions.NEW_SENSOR_DATA, { data: sensorData }, (ack) => {
		logger.info(`New sensor data acknowledgement: ${ack}`);
	});
});
