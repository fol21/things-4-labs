const mqtt = require('mqtt');
const program = require('commander');
const Gpio = require('onoff');


class MqttConsolePublisher {
    constructor(config = {}) {

        this.program = program
            .version('0.1.0')
            .option('-t, --topic <n>', 'Choose topic to be subscribed', (val) => {
                return val
            })
            .option('-m, --message <n>', 'Message to publish')
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

    publish(message='') {
        this.client.on('connect',
            () => {
                console.log(`Connected, Listening to:
                host: ${this.host} 
                port: ${this.port} 
                topic: ${this.topic}`);

                this.client.publish(this.topic, message)
            });

    }

    init() {
        this.client = mqtt.connect({
            host: this.host,
            port: this.port
        });
        if(program.message) this.publish(program.message);
    }
}

module.exports = MqttConsolePublisher;