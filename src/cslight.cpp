#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESP32WebServer.h>
#include <ESPmDNS.h>
#include <Streaming.h>
#include <Adafruit_NeoPixel.h>

#include "wificreds.h"
// Create a separate src/wificreds.h with the following (with actual values, of course):
// const char* ssid = "MY SSID";
// const char* password = "MY SUPERSECRET WIFI PASSWORD";

void handleLED();
void handleLEDon();
void handleLEDoff();
void handleRoot();
void handleNotFound();
void handleRingOn();
void handleRingOff();
void ringOnTest();
void ringOffTest();
void toggleRing();

String response = "";
const int led = 13;  // the on-board red-LED. Use it to indicate activity.
const int button1 = 12;
bool state = 0;
bool ringstate = 0;
int button1State = 0;
#define PIN       21
#define NUMPIXELS 24
#define DELAYVAL 0

MDNSResponder mdns;
ESP32WebServer server(80);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void toggleRing() {
  if (ringstate == 0) {
    ringOnTest();
    ringstate = 1;
  }
  else {
    ringOffTest();
    ringstate = 0;
  }
}

void handleRingOn() {
  ringOnTest();
  server.send(200, "application/json", "{\"ringon\": true}");
  ringstate = 1;
}

void handleRingOff() {
  ringOffTest();
  server.send(200, "application/json", "{\"ringon\": false}");
  ringstate = 0;
}

void ringOnTest() {
  pixels.clear();

for(int i=0; i<NUMPIXELS; i++) {

  // Gee, it would be nice if we could set this with the rest call...
  //pixels.setPixelColor(i, pixels.Color(255, 214, 170));
  //pixels.setPixelColor(i, pixels.Color(255, 0, 0));
  //pixels.setPixelColor(i, pixels.Color(255, 140, 140));
  //pixels.setPixelColor(i, pixels.Color(255, 147, 47));
  //(255, 166.71997008820716, 87.40549175897922) - 3000 
  //(255, 228.0124999191247, 205.93042000867) - 5000
  pixels.setPixelColor(i, pixels.Color(255, 228, 206));
  pixels.show();
  delay(DELAYVAL);
  }
}

void ringOffTest() {
  pixels.clear();

  
for(int i=0; i<NUMPIXELS; i++) {

  pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  pixels.show();
  delay(DELAYVAL);
  }
}

void handleLED() {
  if(state == 0) {
    digitalWrite(led,1);
    state = 1;
    server.send(200, "application/json", "{\"ledon\": true}");
  }
  else {
    digitalWrite(led, 0);
    state = 0;
    server.send(200, "application/json", "{\"ledon\": false}");
  }
}

void handleLEDon() {
  digitalWrite(led,1);
  server.send(200, "application/json", "{\"ledon\": true}");
}

void handleLEDoff() {
  digitalWrite(led, 0);
  server.send(200, "application/json", "{\"ledon\": false}");
}

void handleRoot() {
  String message = "This is ESP7 - using the esp32 to drive/control an accent/work light.\n\n";
  message += "To turn the onboard LED on, GET /ledon\n";
  message += "To turn the onboard LED off, GET /ledoff\n\n";
  message += "To turn the ring on, GET /ringon\n";
  message += "To turn the ring off, GET /ringoff\n\n";
  message += "There *might* be some other URIs to play with...\n";
  printf("This should appear on the serial console when root is called.\n");
  server.send(200, "text/plain", message);
}

void handleNotFound(){
  String message = "404\n\n";
  message += "Good luck with that, but it\'s not here.\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  digitalWrite(led, 1);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp7")) {
    Serial.println("MDNS responder started");

  }

  server.on("/", handleRoot);
  server.on("/led", handleLED);
  server.on("/ledon", handleLEDon);
  server.on("/ledoff", handleLEDoff);

  server.on("/ringon", handleRingOn);
  server.on("/ringoff", handleRingOff);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  pixels.begin();
  digitalWrite(led, 0);
}

void loop(void){
  // listening for web traffic for REST-y calls
  server.handleClient();

  // scan for button-presses for local control
  button1State = digitalRead(button1);

  if(button1State == LOW) {
    Serial.println("button 1 pressed");
    digitalWrite(led, 1);
    while(digitalRead(button1) == LOW) {
       // debounce. wait until button is released.
    }
    toggleRing();
  }
  else {
    digitalWrite(led, 0);
  }

  // ringOnTest();
}
