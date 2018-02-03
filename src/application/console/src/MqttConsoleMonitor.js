const mqtt = require('mqtt');
const program = require('commander');


class MqttConsoleMonitor {

    constructor(config = {}) {
        this.host = config.host;
        this.port = config.port;
        this.client = mqtt.connect({
            host: program.host || config.host,
            port: program.port || config.port
        });
        this.program = program
            .version('0.1.0')
            .option('-t, --topic <n>', 'Choose topic to be subscribed', (val) => {
                return val
            })
            .option('-c, --context <n>', 'Add context to incoming messages')
            .option('-h, --host <n>', 'Overrides pre-configure host')
            .option('-p, --port <n>', 'Overrides pre-configure port', parseInt);

        this.messageCallback = null;
    }

    _defaultMessageCallback(topic, message)
    {
        // message is Buffer
        if (program.context) console.log(`${program.context} ${message}`);
        else console.log(message.toString());

    }

    /**
     * 
     * 
     * @param {function (topic,message)} callback 
     * @memberof MqttConsoleMonitor
     */
    onMessage(callback) {
        this.messageCallback = callback;
    }

    /**
     * Initialize console application
     * Use after setup every callback
     * Must be called only once
     * @memberof MqttConsoleMonitor
     */
    init() {

        program.parse(process.argv);

        if (program.topic) {
            this.client.on('connect',
                () => {
                    console.log(`Connected, Listening to:
            host: ${program.host || conf.mqtt.host } 
            port: ${program.port|| conf.mqtt.port} 
            topic: ${program.topic}`);
                    this.client.subscribe(program.topic);
                });

            this.client.on('message', this.messageCallback || this._defaultMessageCallback)
        } else {
            console.log("You must specify a topic to listen (use -t 'your topic').");
            process.exit();
        }
    }


}

module.exports = MqttConsoleMonitor;