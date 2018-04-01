require('dotenv').config();

const logger = require('./logger');
const lineReader = require('./reader');
const socket = require('./socket')(logger, lineReader);
const serial = require('./serial')(logger, lineReader, socket);
