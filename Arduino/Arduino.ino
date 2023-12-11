#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#define dhtType DHT11;
 
namespace pin {
const byte tds_sensor = A1;
const byte one_wire_bus = 6; // Dallas Temperature Sensor
const byte dht_pin = 5; 
const byte trigPin = 2;
const byte echoPin = 4;
}
 
namespace device {
  float aref = 4.3;
}
 
namespace sensor {
  float ec = 0;
  unsigned int tds = 0;
  float waterTemp = 0;
  float ecCalibration = 1;
  float humidity = 0;
  float airTemp = 0;
  float distance = 0;
  long duration;
}

OneWire oneWire(pin::one_wire_bus);
DallasTemperature dallasTemperature(&oneWire);
 
DHT dht(pin::dht_pin, DHT11); 

void setup() {
  Serial.begin(115200); // Dubugging on hardware Serial 0
  dallasTemperature.begin();
  dht.begin();
  pinMode(pin::trigPin, OUTPUT);
  pinMode(pin::echoPin, INPUT);
}
 
 
void loop() {
  readQuick();
  delay(1000);
}
 
void readQuick() {
  //DHT.readll(pin::dht_pin);
  dallasTemperature.requestTemperatures();
  sensor::waterTemp = dallasTemperature.getTempCByIndex(0);
  float rawEc = analogRead(pin::tds_sensor) * device::aref / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
  float temperatureCoefficient = 1.0 + 0.02 * (sensor::waterTemp - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
  sensor::ec = (rawEc / temperatureCoefficient) * sensor::ecCalibration; // temperature and calibration compensation
  sensor::tds = (133.42 * pow(sensor::ec, 3) - 255.86 * sensor::ec * sensor::ec + 857.39 * sensor::ec) * 0.5; //convert voltage value to tds value
  sensor::humidity = dht.readHumidity();
  sensor::airTemp = dht.readTemperature();
  //Serial.print(F("EC:")); Serial.println(sensor::ec, 2);

  //Ultrasonic code block logic
  digitalWrite(pin::trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin::trigPin, LOW);
  
  sensor::duration = pulseIn(pin::echoPin, HIGH);
  
  sensor::distance = sensor::duration * 0.034 / 2;
  //End of Ultrasonic code block logic

  Serial.print(F("TDS:")); Serial.println(sensor::tds);
  Serial.print(F("Water Temp:")); Serial.println(sensor::waterTemp,2);
  Serial.print(F("Humidity:")); Serial.print(sensor::humidity,0); Serial.println("%");
  Serial.print(F("Air Temp:")); Serial.println(sensor::airTemp,2);
  Serial.print(F("Distance in cm:")); Serial.println(sensor::distance,2);
  Serial.println("===============================================");
}