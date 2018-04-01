
const sensorValueKeys = [
	'temperature',
	'humidity',
	'light',
	'heat',
	'humidifier'
];

module.exports = (logger, lineReader, { socket, socketActions }) => {

	lineReader.on('line', (line) => {

		const sensorValues = line.split(' ');
		console.log(line);
		const sensorData = sensorValues.reduce((acc, val, i) => {
			acc[sensorValueKeys[i]] = val;
			return acc;
		}, {});

		socket.emit(socketActions.NEW_SENSOR_DATA, { data: sensorData }, (ack) => {
			logger.info(`New sensor data acknowledgement: ${ack}`);
		});
	});
}
