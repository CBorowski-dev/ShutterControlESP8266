#include <Arduino.h>
#include <PrivateData.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

#define UP 1
#define DOWN 0

#define DOWN_LED_PIN 8
#define UP_LED_PIN 9
#define WIFI_LED_PIN 10

#define TRUE 1
#define FALSE 0

// -----------------------------------------------------------
// From the Tasmota configuration file
// #define PROJECT                "home/shutters/sd1"
// #define MQTT_GRPTOPIC          "home/shutters/all" | "home/lights/all"
// #define MQTT_FULLTOPIC         "%prefix%/%topic%/"
// 
// #define SUB_PREFIX             "cmnd"
// #define PUB_PREFIX             "stat"
// #define PUB_PREFIX2            "tele"
//
// MQTT Messages:
// Rollo Küche:           home/shutters/sd1
// Rollo Esszimmer:       home/shutters/sd2
// Rollo Wohnzimmer:      home/shutters/sd3
// Rollo Wohnzimmer:      home/shutters/sd4
// 
// Rollo Ankleide:  		  home/shutters/su1
// Rollo Schlafzimmer:	  home/shutters/su2
// Rollo Kinderzimmer 1: 	home/shutters/su3
// Rollo Kinderzimmer 2:  home/shutters/su4
//
// Lichter:
// Licht Garderobe: home/lights/wardrobe
// Licht Küche: home/lights/floor1/light1 <-- Regalboden über Spüle
//
// After flashing new firmware on Sonoff Dual R2 the following steps have to be performed:
//
// 1. Change the configuration to 'Sonoff Dual R2 (39)'
// 2. Configure GPIO1 & GPIO9:
//    GPIO0 --> Switch1 (9)
//    GPIO9 --> Switch2 (10)
// 3. Set 'Interlock on' per Tasmota console
// 4. Deactivate old rules:
//    rule1 0 // old rule: rule1 on Power1#state=1 do backlog delay 280; Power1 off endon
//    rule2 0 // old rule: rule2 on Power2#state=1 do backlog delay 280; Power2 off endon
// 5. Set PulseTime to 28 sec
//    PulseTime1 128
//    PulseTime2 128
// 6. Set SwitchMode for Button 0 & 1 to mode 3
//    SwitchMode1 3
//    SwitchMode2 3
// -----------------------------------------------------------

// #define MQTT_SUBSCRIBE "stat/home/downstairs/shutters/#"
#define MQTT_TOPIC_CMND_BASE_SHUTTERS_FLOOR1 "cmnd/home/shutters/sd"
#define MQTT_TOPIC_CMND_BASE_SHUTTERS_FLOOR2 "cmnd/home/shutters/su"
#define MQTT_TOPIC_CMND_BASE_LIGHTS_FLOOR1 "cmnd/home/lights/floor1/light"

// SSID/Password for WLAN
const char* ssid = SSIDx;
const char* password = PASSWORD;

// MQTT Broker IP address
const char* mqtt_server = "192.168.188.47";

WiFiClient wifiClient;
IPAddress ipAddress;
PubSubClient mqttClient(wifiClient);
Adafruit_MCP23X17 mcp;

// holds the control level: 0=shutters down; 1==shutters up; 2=lights
uint8_t controlLevel = 0;
// holds the devices the user points to
uint8_t deviceSelection = 0;
// holds the selected devices... every bit represents one shutter
uint8_t selectedDevices = 0;
// remembers the timestamp of the last button press
long lastUsage = 0;

void setup_wifi() {
  delay(10);
  // Connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ipAddress = WiFi.localIP();
  Serial.println();
  Serial.print("WiFi connected, IP address: ");
  Serial.println(ipAddress);

  // Switch Wifi LED on
  mcp.digitalWrite(10, HIGH);
}

void moveShutters(uint8_t direction) {
  // the following code works correct when 'interlock' is 
  // set on Tasmota (enter 'interlock on' in Tasmota console)

  uint8_t z = 1;

  uint8_t currentShutter = 8;
  uint8_t minShutter = 1;

  while (currentShutter >= minShutter) {
    if ((selectedDevices & currentShutter) > 0) {
      String cmnd;
      if ( controlLevel == 0 ) {
        cmnd = MQTT_TOPIC_CMND_BASE_SHUTTERS_FLOOR1;
      } else {
        cmnd = MQTT_TOPIC_CMND_BASE_SHUTTERS_FLOOR2;
      }
      cmnd += z + '0' - 48;

      if (direction == UP) {
        // up --> power2
        Serial.println((cmnd + "/POWER2").c_str());
        if(!mqttClient.publish((cmnd + "/POWER2").c_str(), "TOGGLE")) {
          Serial.println("Could not send up MQTT message");
        }
      } else {
        // down --> power1
        Serial.println((cmnd + "/POWER1").c_str());
        if(!mqttClient.publish((cmnd + "/POWER1").c_str(), "TOGGLE")) {
          Serial.println("Could not send up MQTT message");
        }
      }
    }
    currentShutter /= 2;
    z += 1;
  }
}

void switchLights() {
  uint8_t z = 1;

  uint8_t currentLight = 8;
  uint8_t minLight = 1;

  while (currentLight >= minLight) {
    if ((selectedDevices & currentLight) > 0) {
      String cmnd = MQTT_TOPIC_CMND_BASE_LIGHTS_FLOOR1;
      cmnd += z + '0' - 48;

      Serial.println((cmnd + "/POWER1").c_str());
      if(!mqttClient.publish((cmnd + "/POWER1").c_str(), "TOGGLE")) {
        Serial.println("Could not send up MQTT message");
      }
    }
    currentLight /= 2;
    z += 1;
  }
}

void setup_MQTT() {
  mqttClient.setServer(mqtt_server, 1883);
}

void setup_MCP23017() {
  mcp.begin_I2C();      // use default address 0

  // How MCP23017 pins are addressed
  // Pin 0 <-> GPA0
  // ...
  // Pin 7 <-> GPA7
  // Pin 8 <-> GPB0
  // ...
  // Pin 15 <-> GPb7

  // LEDs
  mcp.pinMode(DOWN_LED_PIN, OUTPUT); // Downstairs LED
  mcp.pinMode(UP_LED_PIN, OUTPUT); // Upstairs LED
  mcp.pinMode(WIFI_LED_PIN, OUTPUT); // WiFi information LED

  // Buttons
  mcp.pinMode(0, INPUT); // 1'st shutter up <-> GPA0
  mcp.pinMode(1, INPUT); // 1'st shutter down <-> GPA1
  mcp.pinMode(2, INPUT); // 2'nd shutter up <-> GPA2
  mcp.pinMode(3, INPUT); // 2'nd shutter down <-> GPA3
  mcp.pinMode(4, INPUT); // 3'rd shutter up <-> GPA4
  mcp.pinMode(5, INPUT); // 3'rd shutter down <-> GPA5
  mcp.pinMode(6, INPUT); // 4'th shutter up <-> GPA6
  mcp.pinMode(7, INPUT); // 4'th shutter down <-> GPA7
  mcp.pinMode(15, INPUT); // upstairs/downstairs button <-> GPB7
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("ShutterControlClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void set_Level_LEDs() {
  // Switch LEDs on/off
  mcp.digitalWrite(DOWN_LED_PIN, (controlLevel+1) & 1);
  mcp.digitalWrite(UP_LED_PIN, (controlLevel & 1) | ((controlLevel >> 1) & 1));
}

void changeControlLevel() {
  // controlLevel == 0 --> shutters in the first floor
  // controlLevel == 1 --> shutters in the second floor
  // controlLevel == 2 --> lights in the first floor
  controlLevel = (controlLevel + 1) % 3;
  set_Level_LEDs();
}

void setup() {
  Serial.begin(115200);
  setup_MCP23017();
  setup_wifi();
  setup_MQTT();
  set_Level_LEDs();
}

void loop() {
  // Check every 500ms if any button is pressed
  long now = millis();
  if (now - lastUsage > 500) {
    uint8_t buttonPressed = FALSE;
    uint8_t i = 0;
    // Check Up and Down buttons
    while (i<8 && !buttonPressed) {
      if (mcp.digitalRead(i)) {
        buttonPressed = TRUE;
      }
      i++;
    }
    // Check Upstairs/Downstairs/Lights button
    if (!buttonPressed && mcp.digitalRead(15)) {
      buttonPressed = TRUE;
    }
    
    // Button(s) are pressen, so evaluate and control associated devices
    if (buttonPressed) {
      lastUsage = now;
      if (!mqttClient.connected()) {
        reconnect();
      }
      mqttClient.loop();

      // Port A pins GPA0, GPA2, GPA4, GPA6 are for devices (shutters 1-4 direction up / lights 1-4 on)
      // shutters/lights 1-4 are mapped to the first 4 bits: 1->bit 0; 2->bit 2; 3->bit 4; 4->bit 6
      selectedDevices = 0;
      for (uint8_t i = 0; i<7; i+=2) {
        if (mcp.digitalRead(i)) {
          selectedDevices = selectedDevices ^ (uint8_t) pow(2, 3 - (i/2));
        }
      }
      if (selectedDevices != 0) {
        if (controlLevel <2) {
          // controlLevel == 0 --> shutters in the first floor
          // controlLevel == 1 --> shutters in the second floor
          moveShutters(UP);
        } else {
          // no lights are avaiable to switch in the second floor
        }
      }

      // Port A pins GPA1, GPA3, GPA5, GPA7 are for devices (shutters 1-4 direction down / lights 1-4 off)
      // shutters/lights 1-4 are mapped to the first 4 bits: 1->bit 1; 2->bit 3; 3->bit 5; 4->bit 7
      selectedDevices = 0;
      for (uint8_t i = 1; i<8; i+=2) {
        if (mcp.digitalRead(i)) {
          selectedDevices = selectedDevices ^ (uint8_t) pow(2, 3 - ((i-1)/2));
        }
      }
      if (selectedDevices != 0) {
        if (controlLevel <2) {
          // controlLevel == 0 --> shutters in the first floor
          // controlLevel == 1 --> shutters in the second floor
          moveShutters(DOWN);
        } else {
          // Switch lights in the first floor
          switchLights();
        }
      }

      if (mcp.digitalRead(15)) {
        Serial.println("Change upstairs <-> downstairs");
        changeControlLevel();
      }
    }
  }
}