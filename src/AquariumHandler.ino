#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include "config.h"
#include "../lib/airPump/airPump.h"
#include "../lib/filter/filter.h"

const char *ssid = SSID;
const char *password = PASSWORD;
const int rgbPin1 = D1;
const int rgbPin2 = D2;
const int rgbPin3 = D3;
AirPump airPump(D4);
Filter filter(D5);

ESP8266WebServer server(80);

int targetStartHour = 9;
int targetEndHour = 17;
int targetStartHourNight = 17;
int targetEndHourNight = 20;

bool daylightOn = false;
bool nightlightOn = false;
bool forceLedOn = false;
bool airPumpOn = false;
bool forceAirPumpOff = false;
bool filterOn = true;

void setup() {
  Serial.begin(115200);
  connectToWifi();

  airPump.setupAirPump();
  filter.setupFilter();
  
  pinMode(rgbPin1, OUTPUT);
  pinMode(rgbPin2, OUTPUT);
  pinMode(rgbPin3, OUTPUT);
  configureAndStartServer(airPump, filter);
}

void loop() {
  server.handleClient();

  int currentHour = hour();

  airPump.handleAirPump();
  filter.handleFilter();

  // if (!forceLedOn) {
  //   if (currentHour > targetStartHour && currentHour < targetEndHour) {
  //     daylightOn = true;
  //     nightlightOn = false;
  //   } else if (currentHour > targetStartHourNight && currentHour < targetEndHourNight){
  //     daylightOn = false;
  //     nightlightOn = true;
  //   } else {
  //     daylightOn = false;
  //     nightlightOn = false;
  //   }
  // }
  // if (!forceAirPumpOff){
  //   if (currentHour > targetStartHour && currentHour < targetEndHour) {
  //     airPumpOn = true;
  //   } else {
  //     airPumpOn = false;
  //   }
  // }

  // if (daylightOn) {
  //   handleDaylight();
  // } else if (nightlightOn) {
  //   handleNightlight();
  // } else {
  //   handleLedOff();
  // }

  // if (airPumpOn) {
  //   digitalWrite(airPumpPin, HIGH);
  // } else {
  //   digitalWrite(airPumpPin, LOW);
  // }
  
  // if (filterOn) {
  //   digitalWrite(filterPin, HIGH);
  // } else {
  //   digitalWrite(filterPin, LOW);
  // }
}

void connectToWifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
}

void configureAndStartServer(AirPump airPump, Filter filter) {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/gettimes", HTTP_GET, handleGetTimes);
  server.on("/settimes", HTTP_POST, handleSetTimes);
  server.on("/toggleled", HTTP_POST, handleToggleLed);
  server.on("/toggleairpump", HTTP_POST, handleToggleAirPump);
  server.on("/togglefilter", HTTP_POST, handleToggleFilter);

  server.begin();
}

void handleRoot() {
  Serial.println("handle /");
  server.send(200, "text/plain", "Hello from Aquarium!");
}

void handleGetTimes() {
  Serial.println("handle /gettimes");

  String responseString = "targetStartHour=" + String(targetStartHour)
    + "&targetEndHour=" + String(targetEndHour) + "&targetStartHourNight="
    + String(targetStartHourNight) + "&targetEndHourNight=" + String(targetEndHourNight);
  
  Serial.println(responseString);
  server.send(200, "text/plain", responseString);
}

void handleSetTimes() {
  Serial.println("handle /settimes");

  int newTargetStartHour = server.arg("targetStartHour").toInt();
  int newTargetEndHour = server.arg("targetEndHour").toInt();
  int newTargetStartHourNight = server.arg("targetStartHourNight").toInt();
  int newTargetEndHourNight = server.arg("targetEndHourNight").toInt();

  if(newTargetStartHour >= newTargetEndHour || newTargetStartHourNight >= newTargetEndHourNight) {
    String errorString = "ERROR - newTargetStartHour(" + String(newTargetStartHour)
      + ") newTargetEndHour(" + String(newTargetEndHour) + ") newTargetStartHourNight("
      + String(newTargetStartHourNight) + ") newTargetEndHourNight(" + String(newTargetEndHourNight) + ")";
    
    Serial.println(errorString);
    server.send(400, "text/plain", errorString);
  } else{
    targetStartHour = newTargetStartHour;
    targetEndHour = newTargetEndHour;
    targetStartHourNight = newTargetStartHourNight;
    targetEndHourNight = newTargetEndHourNight;
    
    String returnString = "Times set successfully to " + 
    String(targetStartHour) + "-" + String(targetEndHour) + " daylight and " + 
    String(targetStartHourNight) + "-" + String(targetEndHourNight) + " nightlight";
  
    Serial.println(returnString);
    server.send(200, "text/plain", returnString);
  }
}

void handleToggleLed() {
  Serial.println("handle /toggleled");
  forceLedOn = true;

  String responseString = "Toggle LED successfully to ";

  String ledMode = server.arg("ledmode");
  if (ledMode == "daylight"){
    daylightOn = true;
    nightlightOn = false;
    responseString += "daylight";
  } else if (ledMode == "nightlight"){
    daylightOn = false;
    nightlightOn = true;
    responseString += "nightlight";
  } else {
    daylightOn = false;
    nightlightOn = false;
    forceLedOn = false;
    responseString += "off";
  }
  responseString += " mode!";

  Serial.println(responseString);
  server.send(200, "text/plain", responseString);
}

void handleDaylight() {
  digitalWrite(rgbPin1, HIGH);
  digitalWrite(rgbPin2, HIGH);
  digitalWrite(rgbPin3, HIGH);
  Serial.println("Daylight LED turned on!");
}

void handleNightlight() {
  digitalWrite(rgbPin1, HIGH);
  digitalWrite(rgbPin2, LOW);
  digitalWrite(rgbPin3, LOW);
  Serial.println("Nightlight LED turned on!");
}

void handleLedOff() {
  digitalWrite(rgbPin1, LOW);
  digitalWrite(rgbPin2, LOW);
  digitalWrite(rgbPin3, LOW);
  Serial.println("LED turned off!");
}

void handleToggleAirPump() {
  Serial.println("handle /toggleairpump");

  airPump.toggleAirPumpOn();
  
  String returnString = "Toggled air pump - " + String(airPump.getAirPumpOn());
  Serial.println(returnString);
  server.send(200, "text/plain", returnString);
}

void handleToggleFilter() {
  Serial.println("handle /togglefilter");

  filter.toggleFilterOn();
  
  String returnString = "Toggled filter - " + String(filter.getFilterOn());
  Serial.println(returnString);
  server.send(200, "text/plain", returnString);
}
