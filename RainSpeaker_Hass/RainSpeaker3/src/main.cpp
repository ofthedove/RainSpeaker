#include "Audio.h"

#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>

Audio *audio;

IPAddress mqtt_server(192, 168, 0, 15);

const char* ssid = "Shortening";
const char* password = "ThiaIsAwesome";

WiFiClient espClient;
PubSubClient client(espClient); //lib required for mqtt

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

  Serial.print("Volume is: ");
  Serial.println(volume);

  if(i2s.hasBoard())
  {
    AudioBoard &board = i2s.board();
    board.setVolume(volume);
  }

  // Haven't actually tested this \/ found out I could do that ^ and avoid float
  // float stupidFloatVolumeBcAudioKitIsDumb = (float)volume / 100.0;
  // float v = stupidFloatVolumeBcAudioKitIsDumb;
  // float clamped = v < 0.0 ? 0.0 : (v > 1.0 ? 1.0 : v);
  // i2s.setVolume(clamped);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", "mqttuser", "h\%D^2f#AQk")) {
      Serial.println("connected");
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
  audio = new Audio();

  Serial.begin(115200);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);

  // setup audiokit before SD!
  auto config = i2s.defaultConfig(TX_MODE);
  config.sd_active = true;
  i2s.begin(config);
  i2s.setVolume(1.0);

  // setup file
  SD.begin(chipSelect, SPI, 10000000);
  loopingFile.setFile(SD.open("/RAINLP1.WAV"));
  loopingFile.begin();

  // setup I2S based on sampling rate provided by decoder
  decoder.begin();

  // ---

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
  audio->loop();

  // delay(1);

  // static uint8_t count = 0;
  // static bool prev = true;
  // bool pin = digitalRead(19);
  // if(pin == prev)
  // {
  //   if(count < 200)
  //   {
  //     count++;
  //   }
  //   else if (count == 200)
  //   {
  //     count++;
  //     if(pin)
  //     {
  //       client.publish("memyself/rainSpeaker3/button","release");
  //     }
  //     else
  //     {
  //       client.publish("memyself/rainSpeaker3/button","press");
  //     }
  //   }
  //   else
  //   {
  //     // do nothing, wait for change
  //   }
  // }
  // else
  // {
  //   count = 0;
  // }
  // prev = pin;
}