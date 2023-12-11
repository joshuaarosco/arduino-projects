#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <SoftwareSerial.h>

const char* ssid = "2.4G";
const char* password = "Ew@nk0b@143!1122";
// const char* ssid = "vivo 1906";
// const char* password = "tarnatemikay";

//ThingSpeak Creds
const char* apiKey = "OFTR6NSN7P59Z91B";
const char* server = "api.thingspeak.com";

//PushingBox Creds
const char* host = "api.pushingbox.com";
const char* dev1id = "v112D64D9B15828A";
const char* dev2id = "v15F1F83B79CD217";

const int serverPort = 80;
const int updateInterval = 15000;

namespace pins {
  const byte tds_sensor = A1;
  const byte one_wire_bus = 6; // Dallas Temperature Sensor
  const byte dht_pin = 5; 
  const byte trigPin = 8;
  const byte echoPin = 9;
  const byte rxPin = 2;
  const byte txPin = 3;
  const byte trigPin2 = 13;
  const byte echoPin2 = 12;
}

namespace sensors {
  float ec = 0;
  float ecCalibration = 1;
  int tds = 0;
  float waterTemp = 0;
  float humidity = 0;
  float airTemp = 0;
  float distance = 0;
  long duration;
  float distance2 = 0;
  long duration2;
}

namespace device {
  float aref = 4.3;
}

SoftwareSerial espSerial(pins::rxPin, pins::txPin);  // RX, TX

OneWire oneWire(pins::one_wire_bus);
DallasTemperature dallasTemperature(&oneWire);
DHT dht(pins::dht_pin, DHT11); 

void setup() {
  Serial.begin(9600);         // Initialize Arduino serial communication
  espSerial.begin(9600);    // Initialize ESP8266 serial communication
  delay(2000);                // Wait for ESP8266 to initialize
  dallasTemperature.begin();
  dht.begin();
  pinMode(pins::trigPin, OUTPUT);
  pinMode(pins::echoPin, INPUT);

  pinMode(pins::trigPin2, OUTPUT);
  pinMode(pins::echoPin2, INPUT);
  
  connectToWiFi();
}

void loop() {
  readQuick();
  sendToThingSpeak();
  Serial.println("===============================================");    
  Serial.print(F("TDS:")); Serial.println(sensors::tds);
  Serial.print(F("Water Temp:")); Serial.println(sensors::waterTemp,2);
  Serial.print(F("Humidity:")); Serial.print(sensors::humidity,0); Serial.println("%");
  Serial.print(F("Air Temp:")); Serial.println(sensors::airTemp,2);
  Serial.print(F("Distance #1 in cm:")); Serial.println(sensors::distance,2);
  Serial.print(F("Distance #2 in cm:")); Serial.println(sensors::distance2,2);
  Serial.println("===============================================");
  if(sensors::distance <= 50){
    sendNotif(dev1id);
  }
  if(sensors::distance2 <= 50){
    sendNotif(dev2id);
  }
  delay(15000);
}
void readQuick() {
  dallasTemperature.requestTemperatures();
  sensors::waterTemp = dallasTemperature.getTempCByIndex(0);
  float rawEc = analogRead(pins::tds_sensor) * device::aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float temperatureCoefficient = 1.0 + 0.02 * (sensors::waterTemp - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  sensors::ec = (rawEc / temperatureCoefficient) * sensors::ecCalibration; // temperature and calibration compensation
  sensors::tds = (133.42 * pow(sensors::ec, 3) - 255.86 * sensors::ec * sensors::ec + 857.39 * sensors::ec) * 0.5; //convert voltage value to tds value
  sensors::humidity = dht.readHumidity();
  sensors::airTemp = dht.readTemperature();

  //Ultrasonic code block logic
  digitalWrite(pins::trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pins::trigPin, LOW);
  sensors::duration = pulseIn(pins::echoPin, HIGH);
  sensors::distance = sensors::duration * 0.034 / 2;
  //End of Ultrasonic code block logic

  //Ultrasonic code block logic
  digitalWrite(pins::trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(pins::trigPin2, LOW);
  sensors::duration2 = pulseIn(pins::echoPin2, HIGH);
  sensors::distance2 = sensors::duration2 * 0.034 / 2;
  //End of Ultrasonic code block logic

}

void connectToWiFi(){
  sendATCommand("AT+CWMODE=1\r\n", 2000);   // Set ESP8266 mode to Station mode
  sendATCommand("AT+CWJAP=\""+String(ssid)+"\",\""+String(password)+"\"\r\n", 5000);   // Connect to Wi-Fi network
}

void sendToThingSpeak(){
  String data = "GET /update?api_key=" + String(apiKey) + "&field1=" + String(sensors::tds)+ "&field2=" + String(sensors::waterTemp)+ "&field3=" + String(sensors::humidity)+ "&field4=" + String(sensors::airTemp)+ "&field5=" + String(sensors::distance)+ "&field6=" + String(sensors::distance2);

  sendATCommand("AT+CIPMUX=0\r\n", 2000);   // Set single connection mode
  sendATCommand("AT+CIPSTART=\"TCP\",\""+String(server)+"\",80\r\n", 5000);   // Establish TCP connection
  sendATCommand("AT+CIPSEND=" + String(data.length()+10) + "\r\n", 2000);   // Send data length for HTTP request
  sendATCommand(data + "\r\n", 5000);   // Send HTTP request
  sendATCommand("AT+CIPCLOSE=0\r\n", 2000);   // Close TCP connection
}

void sendATCommand(String command, unsigned long timeout) {
  espSerial.print(command);     // Send AT command
  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < timeout) {
    if (espSerial.available()) {
      // Read the response character by character
      char c = espSerial.read();
      response += c;
    }
  }

  // Print the complete response
  Serial.println(response);
}

void sendNotif(char* devid){
  String command = "AT+CIPSTART=\"TCP\",\""+String(host)+"\",80\r\n";
  sendATCommand(command, 2000);
  delay(1000);

  String url = "GET /pushingbox?devid="+String(devid)+" HTTP/1.1\r\n";
  url += "Host: "+String(host)+"\r\n";
  url += "Connection: close\r\n\r\n";

  command = "AT+CIPSEND=";
  command += String(url.length());
  command += "\r\n";
  sendATCommand(command, 1000);

  sendATCommand(url, 2000);

  // Close the connection
  sendATCommand("AT+CIPCLOSE\r\n", 2000);
}
