const mqtt = require('mqtt');
const program = require('commander');

class MqttPublisher {
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

        this.host = program.host || config.host;
        this.port = program.port || config.port
        this.allowExit = config.allowExit || true;
        this.topic = null;
    }

    publish(message) {
        try {
            if (this.client.connected) {
                this.client.publish(this.topic, message)
            } else console.log('Unable to publish. No connection avaible.')
        } catch (err) {
            console.log(err);
        }
    }

    init() {

        this.program.parse(process.argv);

        if (program.topic) {
            this.topic = program.topic;
        } else {
            console.log("Topic is required (run program with -t <topic> flag)")
            process.exit();
        }

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

                if (program.message) {
                    this.publish(program.message);
                    if (this.allowExit) process.exit();
                }
            });
    }
}

module.exports = MqttPublisher;