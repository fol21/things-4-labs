const mqtt = require('mqtt');
const program = require('commander');

const conf = require('./resources/config.json');

program
    .version('0.1.0')
    .option('-t, --topic <n>','Choose topic to be subscribed',(val) => {return val})
    .option('-c, --context <n>', 'Add context to incoming messages')
    .option('-h, --host <n>', 'Overrides pre-configure host')
    .option('-p, --port <n>', 'Overrides pre-configure port',parseInt)
    .parse(process.argv);


const client  = mqtt.connect(
    {
        host: program.host || conf.mqtt.host, 
        port: program.port || conf.mqtt.port
    });    

if(program.topic)
{
    client.on('connect', 
        () => 
        {
            console.log(`Connected, Listening to:
            host: ${program.host || conf.mqtt.host } 
            port: ${program.port|| conf.mqtt.port} 
            topic: ${program.topic}`);  
            client.subscribe(program.topic);
        });
      
    client.on('message', 
        function (topic, message) {
        // message is Buffer
        if(program.context) console.log(`${program.context} ${message}`);
        else console.log(message.toString());
      })
}
else 
{
    console.log("You must specify a topic to listen (use -t 'your topic').");
    process.exit();
}
