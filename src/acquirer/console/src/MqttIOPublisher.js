const Gpio = require('onoff').Gpio;

const MqttPublisher = require('./MqttPublisher');


class MqttIOPublisher extends MqttPublisher {
    constructor(config = {}) {
        super(config);
        super.program
            .option('-g, --gpio <n>', 'Chooses Gpio port to listen to', parseInt);

        super.program.message = null; //This publisher doesn't allow option message;

        this.gpio = new Gpio(super.program.gpio || config.gpio, 'in');

        process.on('exit', function () {
            this.gpio.unexport();
        });
    }

    init() {
        super.init();
        this.gpio.watch(
            (err, value) =>
            {
                if(err) console.log(err)
                else super.publish(value);
            }
        );
    }
}

module.exports = MqttIOPublisher;