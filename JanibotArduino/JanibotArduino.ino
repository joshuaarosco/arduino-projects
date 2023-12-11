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
  espSerial.begin(115200);
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

    //IncomingString.trim();
    automatic = false;
    if (IncomingString == "F") {
      moveForward();
    } else if (IncomingString == "B") {
      moveBackward();
    } else if (IncomingString == "L") {
      moveLeft();
    } else if (IncomingString == "R") {
      moveRight();
    } else if (IncomingString == "S") {
      stopMoving();
    } else if (IncomingString == "T") {
      move180Turn();
    }else if (IncomingString == "A") {
      automatic = true;
      Serial.println("Janibot has been set to Automatic.");
    } else if (IncomingString == "O") {
      onOffCounter += 1;
      if(onOffCounter%2 ==0){
        motor3.setSpeed(255);
        motor3.run(FORWARD);
        Serial.println("Janibot vacuum has been turned on.");
      }else{
        motor3.run(RELEASE);
        Serial.println("Janibot vacuum has been turned off.");
      }
    } else if (IncomingString == "K") {
      moveSmallLeft();
    } else if (IncomingString == "E") {
      moveSmallRight();
    } 
  }
  
  if(automatic){
    automaticLogic();
  }
  delay(10);
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
  delay(550);
  stopMoving();
}

void moveRight(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  Serial.println("Moving Right");
  delay(600);
  stopMoving();
}

void moveSmallLeft(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  Serial.println("Moving Small Left");
  delay(100);
  stopMoving();
}

void moveSmallRight(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  Serial.println("Moving Small Right");
  delay(100);
  stopMoving();
}

void move180Turn(){
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  Serial.println("Moving 180 Turn Left");
  delay(1150);
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
    autoTurn();
  }
  if(distance > 50){
    autoForward();
  }
}

void autoTurn(){
  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void autoForward(){
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}