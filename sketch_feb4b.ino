int x;
int y;
int pozicija_x;
int pozicija_y;
float trenutni_kot = 90.0;
float trenutni_kot1 = 90.0;
int deadband = 3;         

#include <Servo.h>

Servo myservo;
Servo myservo1;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  myservo.attach(9);
  myservo1.attach(10);
  myservo.write(trenutni_kot);
  myservo1.write(trenutni_kot1);
}

void  loop() {
  if (Serial.available()) {
  String line = Serial.readStringUntil('\n');

  int commaIndex = line.indexOf(',');

  if (commaIndex > 0) {
    x = line.substring(0, commaIndex).toInt();
    y = line.substring(commaIndex + 1).toInt();

    pozicija_x = map(x, 0, 639, 150, 30);
  pozicija_y = map(y, 0, 479, 150, 30);

  if (pozicija_y < 30) {
  pozicija_y = 30;
  }

  if (pozicija_y > 150) {
    pozicija_y = 150;
  }
  
  
  if (pozicija_x < 30) {
  pozicija_x = 30;
  }

  if (pozicija_x > 150) {
    pozicija_x = 150;
  }

  if (abs(pozicija_x - (int)trenutni_kot) > deadband) {
  trenutni_kot = 0.75 * trenutni_kot + 0.25 * pozicija_x; 
  myservo.write((int)trenutni_kot);
  }

  if (abs(pozicija_y - (int)trenutni_kot1) > deadband) {
  trenutni_kot1 = 0.75 * trenutni_kot1 + 0.25 * pozicija_y; 
  myservo1.write((int)trenutni_kot1);
  }
    
  }
}

  
}