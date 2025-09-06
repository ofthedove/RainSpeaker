#include "AudioWithRamp.h"

#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>

#define TOPIC_VOLUMESTATUS "volumeStatus"
#define TOPIC_VOLUMECOMMAND "volumeCommand"
#define TOPIC_RAMPSTATUS "rampTimeMsStatus"
#define TOPIC_RAMPCOMMAND "rampTimeMsCommand"
#define TOPIC_PLAYSTATUS "plabyackStatus"
#define TOPIC_PLAYCOMMAND "plabyackCommand"

#define VOLUME_STATUS TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_VOLUMESTATUS
#define VOLUME_COMMAND TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_VOLUMECOMMAND

#define RAMP_STATUS TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_RAMPSTATUS
#define RAMP_COMMAND TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_RAMPCOMMAND

#define PLAY_STATUS TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_PLAYSTATUS
#define PLAY_COMMAND TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_PLAYCOMMAND

// TODO: Default volume to 0, wait for server to set it
// If we never connect to server, eventually set it to 1.

AudioWithRamp *audio;

IPAddress mqtt_server(192, 168, 0, 15);

const char* ssid = "Shortening";
const char* password = "ThiaIsAwesome";

const char* mqttUser = "mqttuser";
const char* mqttPass = "h\%D^2f#AQk";

const char* topicVolumeStatus = VOLUME_STATUS;
const char* topicVolumeCommand = VOLUME_COMMAND;
const char* topicRampStatus = RAMP_STATUS;
const char* topicRampCommand = RAMP_COMMAND;
const char* topicPlaybackStatus = PLAY_STATUS;
const char* topicPlaybackCommand = PLAY_COMMAND;

WiFiClient espClient;
PubSubClient client(espClient); //lib required for mqtt

bool callbackVolume(char* topic, char payloadString[20])
{
  bool setVolumeFlag = false;

  if(strcmp(topic, topicVolumeStatus) == 0)
  {
    client.unsubscribe(topicVolumeStatus);
    setVolumeFlag = true;
  }

  if(strcmp(topic, topicVolumeCommand) == 0)
  {
    client.publish(topicVolumeStatus, payloadString, true);
    setVolumeFlag = true;
  }

  if(setVolumeFlag)
  {
    int volume = atoi(payloadString);
    audio->setVolume(volume);

    Serial.print("Volume is: ");
    Serial.println(volume);
  }

  return setVolumeFlag;
}

bool callbackRamp(char* topic, char payloadString[20])
{
  bool setRampFlag = false;

  if(strcmp(topic, topicRampStatus) == 0)
  {
    client.unsubscribe(topicRampStatus);
    setRampFlag = true;
  }

  if(strcmp(topic, topicRampCommand) == 0)
  {
    client.publish(topicRampStatus, payloadString, true);
    setRampFlag = true;
  }

  if(setRampFlag)
  {
    int rampTimeMs = atoi(payloadString);
    audio->setRampTime(rampTimeMs);

    Serial.print("Ramp time is: ");
    Serial.println(rampTimeMs);
  }

  return setRampFlag;
}

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

  if(callbackVolume(topic, payloadString)) { }
  else if(callbackRamp(topic, payloadString)) { }
  else {
    Serial.println("Unknown topic");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient", mqttUser, mqttPass)) {
      Serial.println("connected");
      client.subscribe(topicVolumeCommand);
      client.subscribe(topicVolumeStatus);
      client.subscribe(topicRampCommand);
      client.subscribe(topicRampStatus);
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
  audio = new AudioWithRamp(Serial, 0, 2000);

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

  // In case we never connect, default to 50%
  // But do this _after_ connecting WiFi to avoid a noise blip on reset
  audio->setVolume(50);
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