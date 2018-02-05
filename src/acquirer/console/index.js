const MqttConsolePublisher = require('./src/MqttConsolePublisher') 

const conf = require('./resources/config.json');


const client = new MqttConsolePublisher(
    {
        host: conf.mqtt.host,
        port: conf.mqtt.port
    });

client.init();