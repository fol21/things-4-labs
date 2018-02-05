const MqttConsoleMonitor = require('./src/MqttConsoleMonitor.js') 

const conf = require('./resources/config.json');
const monitor = new MqttConsoleMonitor(
    {
        host: conf.mqtt.host,
        port: conf.mqtt.port
    });
    
monitor.init();

