#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// Replace with your Wi-Fi credentials
const char* ssid = "0414@4";
const char* password = "Kah1tan0nga@e1";

const char* channelId = "2183185";
const char* server = "api.thingspeak.com";
const char* apiKey = "D32IFNGVWOY7MWY3";
const int httpsPort = 80;

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");

  // Fetch data from ThingSpeak
  fetchThingSpeakData();
}

void loop() {
  // Do other tasks if needed
}

void fetchThingSpeakData() {
  if (!client.connect(server, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }

  // HTTP GET request to fetch data from ThingSpeak
  String url = "/channels/"+ String(channelId) +"/feeds.json?api_key=" + String(apiKey)+"&results=2";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Fetching data from ThingSpeak...");

  // Read the response
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Response headers received");
      break;
    }
  }

  // Read and print the response body (data)
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  Serial.println("Data fetched from ThingSpeak");
  client.stop();
}