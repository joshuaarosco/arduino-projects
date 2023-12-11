#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#define EEPROM_SIZE 128

// Define the access point network
const char* apSsid = "ESP-01";
const char* apPassword = "smartplug";

// Define the pins for the relay and pushbutton
const int ledPin = 1;
const int relayPin = 0;
const int buttonPin = 2;
unsigned long buttonPressedTime = 0;
unsigned long lastMillis = 0;
unsigned long last_flash_time = 0;
unsigned long flash_interval = 300;
unsigned long timer = 0;
bool isAPEnabled = false;
bool buttonState = LOW;
bool relayState = false;

// Create an instance of the web server
ESP8266WebServer server(80);

// Initialize the relay and button pins
void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(relayPin, LOW); // Make sure the relay is initially off
  EEPROM.begin(EEPROM_SIZE);

  // Retrieve WiFi credentials from Emulated EEPROM (if available)
  
  String ssid = "";
  String password = "";
  char c;
  int i = 0;
  
  while (true) {
    c = EEPROM.read(i++);
    if (c == '\0') break;
    ssid += c;
  }
  while (true) {
    c = EEPROM.read(i++);
    if (c == '\0') break;
    password += c;
  }
    
    // Connect to WiFi using retrieved credentials
  timer =  millis();
  if (ssid.length() > 0 && password.length() > 0) {
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED && millis() - timer <= 10000) {
//      ledFlash();
        digitalWrite(ledPin, !digitalRead(ledPin));
        delay(300);
    }
  }

  // Set up the web server routes
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/state", handleState);
  server.on("/wifi", HTTP_POST, handleWiFi);
  server.on("/ipaddress", handleIP);

  // Start the web server
  server.begin();
  digitalWrite(ledPin, HIGH);
}

void loop() {
  // Handle any incoming HTTP requests
  server.handleClient();
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW && buttonPressedTime == 0){
    lastMillis = millis();
    buttonPressedTime = millis();
  } else if (buttonState == LOW){
    buttonPressedTime = millis();
    if (buttonPressedTime - lastMillis >= 5000){
      handleAP();
      isAPEnabled = true;
      timer = millis();
    }
  } else if(buttonState == HIGH && buttonPressedTime > 0){
    if (buttonPressedTime - lastMillis <= 5000){
      digitalWrite(relayPin, !digitalRead(relayPin));
    }
    buttonPressedTime = 0;
  }
  
  if(isAPEnabled && millis() - timer <=60000){
    ledFlash();
  } else if (isAPEnabled){
    WiFi.softAPdisconnect();
    isAPEnabled = false;
    digitalWrite(ledPin, HIGH);
  }
}

void handleAP(){
  // Start the access point
  WiFi.softAP(apSsid, apPassword);
}

// Handle the /on route
void handleOn() {
  digitalWrite(relayPin, HIGH);
  Serial.println("Relay turned on");
  server.send(200, "text/plain", "Relay turned on");
}

// Handle the /off route
void handleOff() {
  digitalWrite(relayPin, LOW);
  Serial.println("Relay turned off");
  server.send(200, "text/plain", "Relay turned off");
}

// Handle the /state route
void handleState() {
  String state = digitalRead(relayPin) ? "on" : "off";
  Serial.print("Relay state is ");
  Serial.println(state);
  server.send(200, "text/plain", state);
}

void handleIP() {
  if (server.method() == HTTP_GET) {
    // Check if the ESP8266 is connected to a WiFi network
    if (WiFi.status() == WL_CONNECTED) {
      // Get the IP address assigned to the ESP8266 in the new WiFi network
      String ip = WiFi.localIP().toString();

      // Send the IP address as the response
      server.send(200, "text/plain", ip);
      delay(1000);
      WiFi.softAPdisconnect();
      digitalWrite(ledPin, HIGH);
      isAPEnabled = false;
    } else {
      // ESP8266 not connected to a WiFi network
      server.send(400, "text/plain", "Not connected to a WiFi network");
    }
  } else {
    // Invalid method
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

// Handle the /wifi route
void handleWiFi() {
  if (server.method() == HTTP_POST) {
    // Check if the ssid and password arguments were sent
    if (!server.hasArg("ssid") || !server.hasArg("password")) {
      // Arguments missing
      server.send(400, "text/plain", "Bad Request");
      return;
    }
    
    // Get the values of the ssid and password arguments
    String ssid = server.arg("ssid");
    String password = server.arg("password");

    // Connect to the specified WiFi network
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    // Wait for the connection to be established
    int wait = 0;
    while (WiFi.status() != WL_CONNECTED) {
      wait++;
      delay(50);
      digitalWrite(ledPin, !digitalRead(ledPin));
      if (wait > 200){
        server.send(408, "text/plain", "Can't connect to WiFi.");
        WiFi.softAPdisconnect();
        return;
      }
    }

    // Print the IP address assigned to the ESP8266
    Serial.println("");
    Serial.print("WiFi connected, IP address: ");
    Serial.println(WiFi.localIP());

    // Store the WiFi credentials in the Emulated EEPROM
    int ssidLen = ssid.length();
    int passwordLen = password.length();
    for (int i = 0; i < ssidLen; i++) {
      EEPROM.write(i, ssid[i]);
    }
    EEPROM.write(ssidLen, '\0');
    ssidLen++;
    for (int i = 0; i < passwordLen; i++) {
      EEPROM.write(ssidLen + i, password[i]);
    }
    EEPROM.write(ssidLen + passwordLen, '\0'); // Add a null terminator at the end
    EEPROM.commit(); // Save changes to Emulated EEPROM
    
    // Send a response to the client
    server.send(200, "text/plain", "WiFi connected");
    
  } else {
    // Invalid method
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void ledFlash() {
  unsigned long current_time = millis();
  
  // Check if it's time to flash the LED
  if (current_time - last_flash_time >= flash_interval) {
    // Toggle the LED state
    digitalWrite(ledPin, !digitalRead(ledPin));
    
    // Store the current time for the next interval
    last_flash_time = current_time;
  }
}