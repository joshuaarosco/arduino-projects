#include <SoftwareSerial.h>
#include <AFMotor.h>

AF_DCMotor motor1(4);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);

int onOffCounter = 1;
bool automatic = false;
long duration;
long distance;

namespace pin{
  const byte echoPin = 11;
  const byte trigPin = 12;
}

SoftwareSerial espSerial(3, 2);

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  pinMode(pin::echoPin, INPUT);
  pinMode(pin::trigPin, OUTPUT);
  delay(1000);
}

void loop() {
  sonarSensor();
  String IncomingString = "";
  boolean StringReady = false;

  while(espSerial.available()){
    IncomingString = espSerial.readString();
    StringReady = true;
  }

  if (StringReady) {
    Serial.println("Received String: "+IncomingString);

    IncomingString.trim();
    automatic = false;
    if (IncomingString.indexOf('/F') > 0) {
      moveForward();
    } else if (IncomingString.indexOf('/B') > 0) {
      moveBackward();
    } else if (IncomingString.indexOf('/L') > 0) {
      moveLeft();
    } else if (IncomingString.indexOf('/R') > 0) {
      moveRight();
    } else if (IncomingString.indexOf('/S') > 0) {
      stopMoving();
    } else if (IncomingString.indexOf('/A') > 0) {
      automatic = true;
      Serial.println("Janibot has been set to Automatic.");
    } else if (IncomingString.indexOf('/O') > 0) {
      onOffCounter += 1;
      if(onOffCounter%2 ==0){
        motor3.setSpeed(255);
        motor3.run(FORWARD);
        Serial.println("Janibot vacuum has been turned on.");
      }else{
        motor3.run(RELEASE);
        Serial.println("Janibot vacuum has been turned off.");
      }
    }
  }
  
  if(automatic){
    automaticLogic();
  }

}

void moveForward(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  Serial.println("Moving Forward");
  delay(500);
  stopMoving();
}

void moveBackward(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  Serial.println("Moving Backward");
  delay(500);
  stopMoving();
}

void moveLeft(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  Serial.println("Moving Left");
  delay(100);
  stopMoving();
}

void moveRight(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  Serial.println("Moving Right");
  delay(100);
  stopMoving();
}

void stopMoving(){
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  Serial.println("Stop Moving");
}

void sonarSensor(){
  digitalWrite(pin::trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pin::trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(pin::trigPin, LOW);

  duration = pulseIn(pin::echoPin, HIGH);

  distance = duration/58.2;
  // Serial.println(distance);
}

void automaticLogic(){
  if(distance <= 50){
    stopMoving();
    moveLeft();
  }
  if(distance > 50){
    moveForward();
  }
}
