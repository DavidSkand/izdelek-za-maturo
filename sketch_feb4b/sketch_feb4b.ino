int x;
int pozicija;
int currentAngle = 90;     
int deadband = 3;         

#include <Servo.h>

Servo myservo;  

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  myservo.attach(9);
  myservo.write(currentAngle);
}

void  loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();
  pozicija = map(x, 0, 639, 30, 150);
  
  if (pozicija < 30) {
  pozicija = 30;
  }

  if (pozicija > 150) {
    pozicija = 150;
  }

  if (abs(pozicija - currentAngle) > deadband) {
  currentAngle = pozicija;
  myservo.write(currentAngle);
}


}