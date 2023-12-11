#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

DNSServer dnsServer;
WiFiServer server(80);
ESP8266WebServer webServer(80);

const char* ssid = ""; // Enter your default SSID here
const char* password = ""; // Enter your default password here

String htmlContent;



void handleRoot() {
  // Prepare the HTML content for the web page
  htmlContent ="<!DOCTYPE html> <html> <head> <title>Wi-Fi Configuration</title> <style> body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f0f0f0; } h1 { text-align: center; margin-bottom: 20px; } form { max-width: 400px; margin: 0 auto; background-color: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); } label { display: block; margin-bottom: 10px; font-weight: bold; } input[type=\"text\"], input[type=\"password\"] { width: 100%; padding: 10px; border: 1px solid #ccc; border-radius: 4px; font-size: 16px; margin-bottom: 20px; box-sizing: border-box; } input[type=\"submit\"] { background-color: #4CAF50; color: #fff; padding: 12px 20px; border: none; border-radius: 4px; cursor: pointer; font-size: 16px; } input[type=\"submit\"]:hover { background-color: #45a049; } </style> </head> <body> <h1>Wi-Fi Configuration</h1> <form action=\"/save\" method=\"post\"> <label for=\"ssid\">SSID:</label> <input type=\"text\" id=\"ssid\" name=\"ssid\" required> <label for=\"password\">Password:</label> <input type=\"password\" id=\"password\" name=\"password\" required> <input type=\"submit\" value=\"Configure\"> </form> </body> </html>";


  // Send the HTML content to the client
  webServer.send(200, "text/html", htmlContent);
}

void handleSave() {
  // Get the SSID and password values from the POST request
  String newSSID = webServer.arg("ssid");
  String newPassword = webServer.arg("password");

  // Update the SSID and password variables
  ssid = newSSID.c_str();
  password = newPassword.c_str();

  // Connect to the new Wi-Fi network
  WiFi.begin(ssid, password);

  // Wait for the ESP8266 to connect to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print the ESP8266's IP address
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  String response = "Wi-Fi connected <br> IP address: ";
  response +=  WiFi.localIP().toString();
  webServer.send(200, "text/html", response);
}

void handleNotFound() {
  // Redirect all requests to the root URL
  webServer.sendHeader("Location", "/", true);
  webServer.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  delay(10);
  // Set the ESP8266 as an access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP("JANIBOT", ""); // Set the AP name and password

  // Start the web server
  webServer.on("/", handleRoot);
  webServer.on("/save", handleSave);
  webServer.onNotFound(handleNotFound);
  webServer.begin();

  // Enable captive portal functionality
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  dnsServer.start(53, "*", WiFi.softAPIP());

  Serial.println();
  Serial.println("ESP8266 Web Server started");
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Handle incoming client requests
  webServer.handleClient();
  dnsServer.processNextRequest();
  WiFiClient client = server.available();

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