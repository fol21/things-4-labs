#include <ESP8266WiFi.h>
#include <MqttPublisher.h>

struct MqttConfiguration config = {"FOL", "21061992", "ESP8266-test", "192.168.15.5", 1883};
WiFiClient espClient;
MqttPublisher publisher = MqttPublisher(espClient, config);

StaticJsonBuffer<500> jsonBuffer;
JsonObject& object1 = jsonBuffer.createObject();

void callback( char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println((char*) payload);
}



void setup()
{
  Serial.begin(115200);
  delay(3000);
  object1["millis"] = 1000;

  publisher.onMessage(callback);

  publisher.check_network(
    [=]() -> bool
    {
      if(WiFi.status() == WL_CONNECTED)
        return true;
      else 
        return false;
  });
  
  publisher.init_network(
    [=]() -> bool
    {
      delay(10);
      // We start by connecting to a WiFi network
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(config.ssid);

      WiFi.begin(config.ssid, config.password);

      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println("[Network] : Connected");
      return true;
  });
}

void loop()
{
   
   publisher.reconnect(
      [=]()
      {
        Serial.println(publisher.Publisher_state());
      });
   publisher.publish_stream("/test", PERIODIC_STREAM,"data", object1);
}


// #include <ESP8266WiFi.h>
// #include <PubSubClient.h>

// // Update these with values suitable for your network.

// const char* ssid = "FOL";
// const char* password = "21061992";
// const char* mqtt_server = "192.168.15.5";

// WiFiClient espClient;
// PubSubClient client(espClient);
// long lastMsg = 0;
// char msg[50];
// int value = 0;

// void setup_wifi() {

//   delay(10);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }

//   randomSeed(micros());

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();

//   // Switch on the LED if an 1 was received as first character
//   if ((char)payload[0] == '1') {
//     digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
//     // but actually the LED is on; this is because
//     // it is acive low on the ESP-01)
//   } else {
//     digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
//   }

// }

// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Create a random client ID
//     String clientId = "ESP8266Client-";
//     clientId += String(random(0xffff), HEX);
//     // Attempt to connect
//     if (client.connect(clientId.c_str())) {
//       Serial.println("connected");
//       // Once connected, publish an announcement...
//       client.publish("outTopic", "hello world");
//       // ... and resubscribe
//       client.subscribe("inTopic");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

// void setup() {
//   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
//   Serial.begin(115200);
//   setup_wifi();
//   client.setServer(mqtt_server, 1883);
//   client.setCallback(callback);
// }

// void loop() {

//   if (!client.connected()) {
//     reconnect();
//   }
//   client.loop();

//   long now = millis();
//   if (now - lastMsg > 2000) {
//     lastMsg = now;
//     ++value;
//     snprintf (msg, 75, "hello world #%ld", value);
//     Serial.print("Publish message: ");
//     Serial.println(msg);
//     client.publish("outTopic", msg);
//   }
// }