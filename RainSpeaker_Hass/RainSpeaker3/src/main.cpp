// #include "Audio.h"

// #include <WiFi.h>
// #include <WebServer.h>
// #include <PubSubClient.h>

// #define TOPIC_VOLUMESTATUS "volumeStatus"
// #define TOPIC_VOLUMECOMMAND "volumeCommand"
// #define TOPIC_PLAYSTATUS "plabyackStatus"
// #define TOPIC_PLAYCOMMAND "plabyackCommand"

// #define VOLUME_STATUS TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_VOLUMESTATUS
// #define VOLUME_COMMAND TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_VOLUMECOMMAND
// #define PLAY_STATUS TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_PLAYSTATUS
// #define PLAY_COMMAND TOPIC_MANUFACTURER "/" TOPIC_DEVICE "/" TOPIC_PLAYCOMMAND

// // TODO: Default volume to 0, wait for server to set it
// // If we never connect to server, eventually set it to 1.

// Audio *audio;

// IPAddress mqtt_server(192, 168, 0, 15);

// const char* ssid = "Shortening";
// const char* password = "ThiaIsAwesome";

// const char* mqttUser = "mqttuser";
// const char* mqttPass = "h\%D^2f#AQk";

// const char* topicVolumeStatus = VOLUME_STATUS;
// const char* topicVolumeCommand = VOLUME_COMMAND;
// const char* topicPlaybackStatus = PLAY_STATUS;
// const char* topicPlaybackCommand = PLAY_COMMAND;

// WiFiClient espClient;
// PubSubClient client(espClient); //lib required for mqtt

// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.print("] ");
//   for (int i=0;i<length;i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();

//   char payloadString[20] = {0};
//   for (int i=0;i<length && i<19;i++) {
//     payloadString[i] = (char)payload[i];
//   }

//   bool setVolumeFlag = false;

//   if(strcmp(topic, topicVolumeStatus) == 0)
//   {
//     client.unsubscribe(topicVolumeStatus);
//     setVolumeFlag = true;
//   }

//   if(strcmp(topic, topicVolumeCommand) == 0)
//   {
//     client.publish(topicVolumeStatus, payloadString, true);
//     setVolumeFlag = true;
//   }

//   if(setVolumeFlag)
//   {
//     int volume = atoi(payloadString);
//     audio->setVolume(volume);

//     Serial.print("Volume is: ");
//     Serial.println(volume);
//   }
// }

// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     // Attempt to connect
//     if (client.connect("arduinoClient", mqttUser, mqttPass)) {
//       Serial.println("connected");
//       client.subscribe(topicVolumeCommand);
//       client.subscribe(topicVolumeStatus);
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

// void setup()
// {
//   Serial.begin(115200);
//   audio = new Audio(Serial);

//   // ---

//   pinMode(19, INPUT);

//   Serial.println("connecting wifi...");

//   WiFi.begin(ssid, password);
//   while(!WiFi.isConnected())
//   {
//     delay(200);
//   }

//   Serial.println("connected");

//   client.setServer(mqtt_server, 1883);//connecting to mqtt server
//   client.setCallback(callback);
//   //delay(5000);
//   delay(1500);
// }

// void loop()
// {
//   // put your main code here, to run repeatedly:
//   if (!client.connected())
//   {
//     reconnect();
//   }

//   client.loop();
//   audio->loop();

//   // delay(1);

//   // static uint8_t count = 0;
//   // static bool prev = true;
//   // bool pin = digitalRead(19);
//   // if(pin == prev)
//   // {
//   //   if(count < 200)
//   //   {
//   //     count++;
//   //   }
//   //   else if (count == 200)
//   //   {
//   //     count++;
//   //     if(pin)
//   //     {
//   //       client.publish("memyself/rainSpeaker3/button","release");
//   //     }
//   //     else
//   //     {
//   //       client.publish("memyself/rainSpeaker3/button","press");
//   //     }
//   //   }
//   //   else
//   //   {
//   //     // do nothing, wait for change
//   //   }
//   // }
//   // else
//   // {
//   //   count = 0;
//   // }
//   // prev = pin;
// }




/**
 * @file player-sd-audiokit.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-audiokit/player-sdmmc-audiokit/README.md
 * Make sure that the pins are set to on, on, on, on, on
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

 #include "AudioTools.h"
 #include "AudioTools/AudioLibs/AudioBoardStream.h"
 #include "AudioTools/Disk/AudioSourceSDMMC.h" // or AudioSourceIdxSDMMC.h
 #include "AudioTools/AudioCodecs/CodecMP3Helix.h"

 const char *startFilePath="/";
 const char* ext="mp3";
 AudioSourceSDMMC source(startFilePath, ext);
 AudioBoardStream kit(LyratV43);
 MP3DecoderHelix decoder;  // or change to MP3DecoderMAD
 AudioPlayer player(source, kit, decoder);

 void setup() {
   Serial.begin(115200);
   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);

   // setup output
   auto cfg = kit.defaultConfig(TX_MODE);
   cfg.sd_active = false;
   kit.begin(cfg);


   // setup player
   player.setVolume(0.7);
   player.begin();

   player.setVolume(1.0);
   kit.setVolume(1.0);

   // select file with setPath() or setIndex()
   //player.setPath("/ZZ Top/Unknown Album/Lowrider.mp3");
   //player.setIndex(1); // 2nd file

 }

 void loop() {
   player.copy();
 }
