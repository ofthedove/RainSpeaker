
// #include <SPI.h>
// #include <SD.h>
// #include "AudioTools.h"
// #include "AudioTools/AudioLibs/AudioBoardStream.h"
// #include "AudioTools/Disk/FileLoop.h"


// const int chipSelect=PIN_AUDIO_KIT_SD_CARD_CS;
// AudioBoardStream i2s(AudioKitEs8388V1); // final output of decoded stream
// WAVDecoder wav;
// EncodedAudioStream decoder(&i2s, &wav); // Decoding stream
// FileLoop loopingFile;
// StreamCopy copier(decoder, loopingFile, 4096);

// void setup(){
//   Serial.begin(115200);
//   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);

//   // setup audiokit before SD!
//   auto config = i2s.defaultConfig(TX_MODE);
//   config.sd_active = true;
//   i2s.begin(config);
//   i2s.setVolume(1.0);

//   // setup file
//   SD.begin(chipSelect, SPI, 10000000);
//   loopingFile.setFile(SD.open("/RAINLP1.WAV"));
//   loopingFile.begin();

//   // setup I2S based on sampling rate provided by decoder
//   decoder.begin();
// }

// void loop(){
//   copier.copy();
// }

#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>

// const char* mqtt_server = "192.168.0.15"; //mqtt server
IPAddress mqtt_server(192, 168, 0, 15);

const char* ssid = "Shortening";
const char* password = "ThiaIsAwesome";

WiFiClient espClient;
PubSubClient client(espClient); //lib required for mqtt

int volume = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  char payloadString[20] = {0};
  for (int i=0;i<length && i<19;i++) {
    payloadString[i] = (char)payload[i];
  }

  if(strcmp(topic, "memyself/rainSpeaker3/volumeStatus") == 0)
  {
    client.unsubscribe("memyself/rainSpeaker3/volumeStatus");
    volume = atoi(payloadString);
  }

  if(strcmp(topic, "memyself/rainSpeaker3/volumeCommand") == 0)
  {
    volume = atoi(payloadString);
    client.publish("memyself/rainSpeaker3/volumeStatus", payloadString, true);
  }

  // if(length > 0)
  // {

  //   // snprintf(myString, sizeof(myString), "Volume %i", payload[0]);
  //   client.publish("memyself/rainSpeaker3/volumeStatus", myString, true);

  //   Serial.print("length: ");
  //   Serial.println(length);
  //   Serial.print("sending: ");
  //   Serial.println(myString);
  // }
  Serial.print("Volume is: ");
  Serial.println(volume);
}

// void callback(char* topic, byte* payload, unsigned int length) {   //callback includes topic and payload ( from which (topic) the payload is comming)
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
//   for (int i = 0; i < length; i++)
//   {
//     Serial.print((char)payload[i]);
//   }
//   if ((char)payload[0] == 'O' && (char)payload[1] == 'N') //on
//   {
//     digitalWrite(LED, HIGH);
//     Serial.println("on");
//     client.publish("outTopic", "LED turned ON");
//   }
//   else if ((char)payload[0] == 'O' && (char)payload[1] == 'F' && (char)payload[2] == 'F') //off
//   {
//     digitalWrite(LED, LOW);
//     Serial.println(" off");
//     client.publish("outTopic", "LED turned OFF");
//   }
//   Serial.println();
// }


// void reconnect() {
//   while (!client.connected()) {
//     Serial.println("Attempting MQTT connection...");
//     if (client.connect("ESP32_clientID")) {
//       Serial.println("connected");
//       // Once connected, publish an announcement...
//       client.publish("outTopic", "Nodemcu connected to MQTT");
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

// void connectmqtt()
// {
//   client.connect("ESP32_clientID");  // ESP will connect to mqtt broker with clientID
//   {
//     Serial.println("connected to MQTT");
//     // Once connected, publish an announcement...

//     // ... and resubscribe
//     client.subscribe("inTopic"); //topic=Demo
//     client.publish("outTopic",  "connected to MQTT");

//     if (!client.connected())
//     {
//       reconnect();
//     }
//   }
// }


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", "mqttuser", "h\%D^2f#AQk")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("memyself/rainSpeaker3/volumeCommand");
      client.subscribe("memyself/rainSpeaker3/volumeStatus");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  // pinMode(LED, OUTPUT);
  // digitalWrite(LED, LOW);

  pinMode(19, INPUT);

  Serial.println("connecting wifi...");

  WiFi.begin(ssid, password);
  while(!WiFi.isConnected())
  {
    delay(200);
  }

  Serial.println("connected");

  client.setServer(mqtt_server, 1883);//connecting to mqtt server
  client.setCallback(callback);
  //delay(5000);
  delay(1500);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();

  delay(1);

// static int count = 0;
// count++;
// if (count > 500)
// {
//   count = 0;

//   static int innerCount = 0;
//   innerCount++;
//   innerCount = innerCount % 10;
//   char myString[20] = {0};
//   snprintf(myString, sizeof(myString), "test %d", innerCount);
//   client.publish("memyself/rainSpeaker3/button",myString);
// }

  static uint8_t count = 0;
  static bool prev = true;
  bool pin = digitalRead(19);
  if(pin == prev)
  {
    if(count < 200)
    {
      count++;
    }
    else if (count == 200)
    {
      count++;
      if(pin)
      {
        client.publish("memyself/rainSpeaker3/button","release");
      }
      else
      {
        client.publish("memyself/rainSpeaker3/button","press");
      }
    }
    else
    {
      // do nothing, wait for change
    }
  }
  else
  {
    count = 0;
  }
  prev = pin;
}