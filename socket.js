const io = require('socket.io-client');

// Socket events from grobro server (includes standard socket events)
const socketEvents = {
	SOCKET_CONNECTED: 'connect',
	SOCKET_DISCONNECTED: 'disconnect',
	SOCKET_CONNECTION_ERROR: 'connect_error',
	SOCKET_CONNECTION_TIMEOUT: 'connect_timeout',
	SOCKET_ERROR: 'error',
	SOCKET_RECONNECTING: 'reconnecting',
	SOCKET_RECONNECT_ERROR: 'reconnect_error',
	SOCKET_RECONNECT_FAILED: 'reconnect_failed',

	GROW_LIGHT_OFF: 'toggleGrowOff',
	GROW_LIGHT_ON: 'toggleGrowOn',

	SET_VEGETATIVE_CYCLE: 'setVegetativeCycle',
	SET_FLOWERING_CYCLE: 'setFloweringCycle'
}

// Socket actions to emitted from this client
const socketActions = {
	NEW_SENSOR_DATA: 'new_sensor_data'
};

module.exports = (logger) => {

	const socket = io(process.env.SOCKET_HOST, {
		path: '/socket',
		transports: ['websocket']
	});

	// Standard Socket Events

	socket.on(socketEvents.SOCKET_CONNECTED, () => {
		logger.info('A socket has been connected!');
	});

	socket.on(socketEvents.SOCKET_DISCONNECTED, () => {
		logger.warn('Socket was disconnected!');
	});

	socket.on(socketEvents.SOCKET_CONNECTION_ERROR, (error) => {
		logger.error('There was a socket connection error!', error);
	});

	socket.on(socketEvents.SOCKET_CONNECTION_TIMEOUT, (timeout) => {
		logger.error('There was a connection timeout!', error);
	});

	socket.on(socketEvents.SOCKET_ERROR, (error) => {
		logger.error('There was a socket error!', error);
	});

	socket.on(socketEvents.SOCKET_RECONNECTING, (attemptNumber) => {
		logger.verbose(`Attempting to reconnect to the socket! Attempt #: ${attemptNumber}`);
	});

	socket.on(socketEvents.SOCKET_RECONNECT_ERROR, (error) => {
		logger.error(`There was an error while trying to reconnect to the socket!`, error);
	});

	socket.on(socketEvents.SOCKET_RECONNECT_FAILED, (error) => {
		logger.error(`The socket reconnection attempt failed!`, error);
	});

	// Grobro Socket Server Events

	socket.on(socketEvents.SET_FLOWERING_CYCLE, () => {
		logger.info('Set flowering cycle event received!');

		// tell arduino to change cycle to flowering
	});

	socket.on(socketEvents.SET_VEGETATIVE_CYCLE, () => {
		logger.info('Set vegetative cycle event received!');

		// tell arduino to change cycle to vegetative
	});

	socket.on(socketEvents.GROW_LIGHT_ON, () => {
		logger.info('Set grow light on event received!');

		// tell arduino to turn on light
	});

	socket.on(socketEvents.GROW_LIGHT_OFF, () => {
		logger.info('Set grow light off event received!');

		// tell arduino to turn off light
	});

	return {
		socket,
		socketActions,
		socketEvents
	};
}
