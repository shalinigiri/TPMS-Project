/*
Pin Connections

Pressure Sensor - 1
vcc - 3v3
out - 12
sck - 14
gnd - gnd

Pressure Sensor - 2
vcc - vin
out - 13
sck - 26
gnd - gnd

*/



#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "HX710B.h"

const char* ssid = "Shalugiri";
const char* password = "asdfghjkl";

AsyncWebServer server(80);

const int front_left_dout_pin = 12;
const int front_left_clock_pin  = 14;
const int front_right_dout_pin = 13;
const int front_right_clock_pin  = 26;


HX710B front_left_pressure_sensor;
HX710B front_right_pressure_sensor;


float getFrontLeftAirPressure() {
  if (front_left_pressure_sensor.is_ready()) {
    Serial.print("Front Left: ");
    Serial.println(front_left_pressure_sensor.psi());
    return front_left_pressure_sensor.psi();
  } else {
    Serial.println("Front Left Pressure sensor not found.");  
    return 0.0;
  }
}

float getFrontRightAirPressure() {
  if (front_right_pressure_sensor.is_ready()) {
    Serial.print("Front Right: ");
    Serial.println(front_right_pressure_sensor.pascal());
    return front_right_pressure_sensor.pascal();
  } else {
    Serial.println("Front Right Pressure sensor not found.");
    return 0.0;
  }
}



void setup() {
  Serial.begin(19200);
  
  front_left_pressure_sensor.begin(front_left_dout_pin, front_left_clock_pin, 64);
  // front_right_pressure_sensor.begin(front_right_dout_pin, front_right_clock_pin, 64);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("GET /");

    float front_left = getFrontLeftAirPressure();
    // float front_right = getFrontRightAirPressure();
    
    String html = "{\"front_left\" : ";
    html += String(front_left);
    // html += ",";
    // html += "\"front_right\" : ";
    // html += String(front_right);
    html += "}";
    
    request->send(200, "text/json", html);
  });

  server.begin();
}

void loop() {
  // float front_left = getFrontLeftAirPressure();
    // float front_right = getFrontRightAirPressure();
    // delay(1000);
}
