// Pin connections
const int trigPin = 2;  // Trigger pin connected to Arduino digital pin 2
const int echoPin = 4;  // Echo pin connected to Arduino digital pin 3

// Variables
long duration;
int distance;

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);
  
  // Set trigger pin as output and echo pin as input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Trigger a pulse by setting the trigger pin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the duration of the echo pulse
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in centimeters
  distance = duration * 0.034 / 2;
  
  // Print the distance to the Serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Delay before taking the next measurement

  delay(500);
}
