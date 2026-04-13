#include <Servo.h>

Servo panServo;  // Pan os (X) na pinu 9
Servo tiltServo; // Tilt os (Y) na pinu 10

const int laserPin = 7;

// Spremenljivke za pozicijo
float currentPan = 90.0;
float currentTilt = 90.0;
float targetPan = 90.0;
float targetTilt = 90.0;

// Shranjevanje zadnjih koordinat za mrtvo območje
int last_x_pixel = 320;
int last_y_pixel = 240;

// Kalibracija - prilagodi glede na tvojo nastavitev
int pan_min = 125;   // Kot ko je X = 0
int pan_max = 58;    // Kot ko je X = 640
int tilt_min = 100; 
int tilt_max = 50;

// Mrtvo območje (Deadband) - število pikslov, ki jih ignoriramo
int deadband = 4; 

int pan_offset = 0;   // tune this: + moves laser right, - moves left
int tilt_offset = 20;  // tune this: + moves laser down, - moves up

// Glajenje premikanja (Smoothing)
float smoothing = 0.15; 
unsigned long lastSmoothUpdate = 0;
const unsigned long smoothInterval = 20; // Posodobitev na 20ms (50 Hz)

// Funkcija za preračun pikslov v kote
float floatMap(float x, float inMin, float inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(5); // Hitra odzivnost serijske povezave
  
  panServo.attach(10);
  tiltServo.attach(9);
  
  // Postavi servomotorje na sredino ob zagonu
  panServo.write(90);
  tiltServo.write(90);

  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
}

void loop() {
  // Preveri, če so prišli novi podatki iz računalnika
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    
    int commaIndex = data.indexOf(',');
    
    // Preveri, če je prejeti niz pravilen
    if (commaIndex > 0) {
      int x_pixel = data.substring(0, commaIndex).toInt();
      int y_pixel = data.substring(commaIndex + 1).toInt();

      // Mrtvo območje: posodobi cilj le, če je premik večji od deadband-a
      if (abs(x_pixel - last_x_pixel) > deadband || abs(y_pixel - last_y_pixel) > deadband) {
        
        // Izračunaj nove ciljne kote
        targetPan  = floatMap(x_pixel, 0, 640, pan_min, pan_max)  + pan_offset;
        targetTilt = floatMap(y_pixel, 0, 480, tilt_min, tilt_max) + tilt_offset;
        
        // Shrani zadnje piksle
        last_x_pixel = x_pixel;
        last_y_pixel = y_pixel;
      }
    }
  }

  // Glajenje in premikanje servomotorjev neodvisno od hitrosti Seriala
  unsigned long now = millis();
  if (now - lastSmoothUpdate >= smoothInterval) {
    lastSmoothUpdate = now;

    // Postopno premikanje proti cilju (Smoothing)
    currentPan = (targetPan * smoothing) + (currentPan * (1.0 - smoothing));
    currentTilt = (targetTilt * smoothing) + (currentTilt * (1.0 - smoothing));



    // Premik servomotorjev z varnostno omejitvijo
    panServo.write((int)constrain(currentPan, 10, 170));
    tiltServo.write((int)constrain(currentTilt, 10, 170));
  }
}
