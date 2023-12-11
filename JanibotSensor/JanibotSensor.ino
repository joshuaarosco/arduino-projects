namespace pin{
  const byte echoPin = 11;
  const byte trigPin = 12;
}

long duration;
long distance;

void setup() {
  Serial.begin(9600);
  pinMode(pin::echoPin, INPUT);
  pinMode(pin::trigPin, OUTPUT);
}

void loop() {
  digitalWrite(pin::trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(pin::trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(pin::trigPin, LOW);

  duration = pulseIn(pin::echoPin, HIGH);

  distance = duration/58.2;

  Serial.println(distance);

  delay(50);
  
}
