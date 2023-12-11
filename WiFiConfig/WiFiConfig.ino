#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

ESP8266WebServer server(80);
DNSServer dnsServer;

const char* ssid = ""; // Enter your default SSID here
const char* password = ""; // Enter your default password here

String htmlContent;

void handleRoot() {
  // Prepare the HTML content for the web page
  htmlContent = "<html><body>";
  htmlContent += "<h1>ESP8266 WiFi Configuration</h1>";
  htmlContent += "<form method='POST' action='save'>";
  htmlContent += "SSID: <input type='text' name='ssid'><br>";
  htmlContent += "Password: <input type='password' name='password'><br>";
  htmlContent += "<input type='submit' value='Save'>";
  htmlContent += "</form>";
  htmlContent += "</body></html>";

  // Send the HTML content to the client
  server.send(200, "text/html", htmlContent);
}

void handleSave() {
  // Get the SSID and password values from the POST request
  String newSSID = server.arg("ssid");
  String newPassword = server.arg("password");

  // Update the SSID and password variables
  ssid = newSSID.c_str();
  password = newPassword.c_str();

  // Prepare a response to indicate successful update
  String response = "Configuration saved successfully.<br>";
  response += "SSID: " + newSSID + "<br>";
  response += "Password: " + newPassword + "<br>";
  response += "Connecting to Wi-Fi network...";

  // Send the response to the client
  server.send(200, "text/html", response);

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
}


void handleNotFound() {
  // Redirect all requests to the root URL
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  // Set the ESP8266 as an access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP8266AP", ""); // Set the AP name and password

  // Start the web server
  server.on("/", handleRoot);
  server.on("/save", handleSave);
  server.onNotFound(handleNotFound);
  server.begin();

  // Enable captive portal functionality
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  dnsServer.start(53, "*", WiFi.softAPIP());

  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP8266 Web Server started");
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
  dnsServer.processNextRequest();
}