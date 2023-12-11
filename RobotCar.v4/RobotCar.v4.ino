#include <AFMotor.h>

AF_DCMotor motor1(1); // Motor 1
AF_DCMotor motor2(2); // Motor 2

void setup() {
  // Set motor speed
  motor1.setSpeed(255); // Adjust the speed value as needed
  motor2.setSpeed(255); // Adjust the speed value as needed

  // Initialize Serial communication
  Serial.begin(9600);
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    executeCommand(command);
  }
}

void executeCommand(char command) {
  switch (command) {
    case 'F': // Move forward
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      break;
    case 'B': // Move backward
      motor1.run(BACKWARD);
      motor2.run(BACKWARD);
      break;
    case 'L': // Turn left
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      break;
    case 'R': // Turn right
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      break;
    case 'S': // Stop
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      break;
    default:
      break;
  }
}
