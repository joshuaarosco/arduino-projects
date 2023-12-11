#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "0414@4";
const char* password = "Kah1tan0nga@e1";

WiFiServer server(80);  // Create a server object to listen on port 80
ESP8266WebServer webServer(80);

namespace pins {
  int enA = 9;
  int in1 = 8;
  int in2 = 7;
}

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Set dc motor pins
  //setPin();
  //moveForward();
  //setSpeed();

  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();  // Start the server

  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();  // Check if a client has connected

  if (client) {
    Serial.println("New client connected");
    String currentLine = "";  // make a String to hold incoming data from the client

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();  // read a byte from the current client
        Serial.write(c);  // send it to the Serial monitor

        // If the HTTP request has ended, break from the loop
        if (c == '\n') {
          break;
        }

        // Check for specific commands to control the robot car
        if (c == 'F') {
          // Move forward
          // Code to control the motors and move the car forward
          //moveForward();
          Serial.println("Moving forward.");
        } else if (c == 'B') {
          // Move backward
          // Code to control the motors and move the car backward
          //moveBackward();
          Serial.println("Moving Backward.");
        } else if (c == 'L') {
          // Turn left
          // Code to control the motors and turn the car left
          //turnLeft();
          Serial.println("Moving Left.");
        } else if (c == 'R') {
          // Turn right
          // Code to control the motors and turn the car right
          //turnRight();
          Serial.println("Moving Right.");
        } else if (c == 'S') {
          // Stop
          // Code to stop the motors and halt the car
          //stopMotor();
          Serial.println("Stop Moving.");
        } else if (c == 'A') {
          // Stop
          // Code to stop the motors and halt the car
          Serial.println("Janibot has been set to Automatic.");
        } else if (c == 'O') {
          // Stop
          // Code to stop the motors and halt the car
          Serial.println("Janibot has been turned on.");
        }
      }
    }

    // Close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}

void setSpeed(){
  // Set the motor speed (0-255)
  analogWrite(pins::enA, 200);
}

void setPin(){
  pinMode(pins::enA, OUTPUT);
  pinMode(pins::in1, OUTPUT);
  pinMode(pins::in2, OUTPUT);
}

void moveForward() {
  digitalWrite(pins::in1, HIGH);
  digitalWrite(pins::in2, LOW);
  delay(2000);
}

void moveBackward() {
  digitalWrite(pins::in1, LOW);
  digitalWrite(pins::in2, HIGH);
  delay(2000);
}

void turnRight() {
  digitalWrite(pins::in1, HIGH);
  digitalWrite(pins::in2, HIGH);
  delay(2000);
}

void turnLeft() {
  digitalWrite(pins::in1, LOW);
  digitalWrite(pins::in2, LOW);
  delay(2000);
}

void stopMotor() {
  digitalWrite(pins::in1, LOW);
  digitalWrite(pins::in2, LOW);
  delay(1000);
}
