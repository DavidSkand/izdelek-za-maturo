#include <Servo.h>

// Nastavitve za servomotorje
Servo myservo;  // Pan (X os)
Servo myservo1; // Tilt (Y os)

// Spremenljivke za pozicijo
float trenutni_kot = 90.0;
float trenutni_kot1 = 90.0;

float cilj_X = 90.0;
float cilj_Y = 90.0;


// Kalibracija - prilagodi te številke, da laser zadane robove kamere
int pan_min = 160;   // Kot ko je X = 0
int pan_max = 20;    // Kot ko je X = 640
int tilt_min = 160;   // Kot ko je Y = 480
int tilt_max = 20;  // Kot ko je Y = 0
int prejsnji_x = 0;
int prejsnji_y = 0;
int deadband = 5;

int centerX = 320;  // center 640px wide camera
int centerY = 240;  // center 480px high camera

// "Smoothing" faktor (0.05 - 0.2). Manjše je bolj tekoče, večje je hitrejše.
float smoothing = 0.1; 

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  
  myservo.attach(9);
  myservo1.attach(10);
  
  // Postavi na začetno sredino
  myservo.write(trenutni_kot);
  myservo1.write(trenutni_kot1);
}

void loop() {
  if (Serial.available() > 0) {
    // 1. Prebere do \n (newline) in očisti presledke
    String data = Serial.readStringUntil('\n');
    data.trim();
    
    // 2. Najde comma
    int commaIndex = data.indexOf(',');
    
    if (commaIndex != -1) {
      // Pretvori v int
      int x_pixel = data.substring(0, commaIndex).toInt();
      int y_pixel = data.substring(commaIndex + 1).toInt();

      // Če Python pošlje -1 (ko ne vidi taga), ne naredimo nič
      if (x_pixel < 0 || y_pixel < 0) {
        return; 
      }

      if (abs(x_pixel - prejsnji_x) > deadband || abs(y_pixel - prejsnji_y) > deadband) {

          cilj_X = map(x_pixel, 0, 640, pan_min, pan_max);
          cilj_Y = map(y_pixel, 0, 480, tilt_min, tilt_max);
          
          // Shrani trenutne piksle za naslednjo primerjavo
          prejsnji_x = x_pixel;
          prejsnji_y = y_pixel;
      } 

      // spremenimo hitrost 
      
      trenutni_kot = (cilj_X * smoothing) + (trenutni_kot * (1.0 - smoothing));
      trenutni_kot1 = (cilj_Y * smoothing) + (trenutni_kot1 * (1.0 - smoothing));

      // Omejimo kot na varno območje 
      trenutni_kot = constrain(trenutni_kot, 10, 170);
      trenutni_kot1 = constrain(trenutni_kot1, 10, 170);

      //Premaknemo servote
      myservo.write((int)trenutni_kot);
      myservo1.write((int)trenutni_kot1);

      // debug
      Serial.print("Received X: "); Serial.print(x_pixel);
      Serial.print(" Y: "); Serial.println(y_pixel);
    }
  }
}
