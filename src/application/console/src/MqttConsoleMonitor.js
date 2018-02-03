const readline = require('readline');
const mqtt = require('mqtt');
const program = require('commander');



class MqttConsoleMonitor {

    constructor(config = {}) {

        this.program = program
            .version('0.1.0')
            .option('-t, --topic <n>', 'Choose topic to be subscribed', (val) => {
                return val
            })
            .option('-c, --context <n>', 'Add context to incoming messages')
            .option('-h, --host <n>', 'Overrides pre-configure host')
            .option('-p, --port <n>', 'Overrides pre-configure port', parseInt)
            .parse(process.argv);

        this.host = program.host || config.host;
        this.port = program.port || config.port
        
        this.messageCallback = null;

        this.topic = null;
        if (program.topic) {
            this.topic = program.topic;
        } else {
            console.log("Topic is required (run program with -t <topic> flag)")
            process.exit();
        }
    }

    _defaultMessageCallback(topic, message) {
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

        //program.parse(process.argv);

        this.client = mqtt.connect({
            host: this.host,
            port: this.port
        });
        this.client.on('connect',
            () => {
                console.log(`Connected, Listening to:
            host: ${this.host} 
            port: ${this.port} 
            topic: ${this.topic}`);
                this.client.subscribe(this.topic);
            });
        this.client.on('message', this.messageCallback || this._defaultMessageCallback)
    }


}

module.exports = MqttConsoleMonitor;