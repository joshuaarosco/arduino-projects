#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "0414@4";
const char* password = "Kah1tan0nga@e1";

WiFiServer server(80);  // Create a server object to listen on port 80
ESP8266WebServer webServer(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.write(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();  // Start the server

  Serial.println("Server started");
  Serial.write("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();  // Check if a client has connected
  if (client) {
    //Serial.write("New client connected");
    String currentLine = "";  // make a String to hold incoming data from the client

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();  // read a byte from the current client
        Serial.write(c);  // send it to the Serial monitor
        if (c == '\n') {
          break;
        }
      }
    }

    // Close the connection
    client.stop();
    //Serial.println("Client disconnected");
  }
}
