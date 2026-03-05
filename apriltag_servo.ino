#include <Servo.h>

Servo panServo;  // Pan os (X) na pinu 9
Servo tiltServo; // Tilt os (Y) na pinu 10

// Spremenljivke za pozicijo
float currentPan = 90.0;
float currentTilt = 90.0;
float targetPan = 90.0;
float targetTilt = 90.0;

// Shranjevanje zadnjih koordinat za mrtvo območje
int last_x_pixel = 320;
int last_y_pixel = 240;

// Kalibracija - prilagodi glede na tvojo nastavitev
int pan_min = 150;   // Kot ko je X = 0
int pan_max = 33;    // Kot ko je X = 640
int tilt_min = 140;  // Kot ko je Y = 480
int tilt_max = 20;   // Kot ko je Y = 0

// Mrtvo območje (Deadband) - število pikslov, ki jih ignoriramo
int deadband = 4; 

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
  
  panServo.attach(9);
  tiltServo.attach(10);
  
  // Postavi servomotorje na sredino ob zagonu
  panServo.write(90);
  tiltServo.write(90);
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
        targetPan = floatMap(x_pixel, 0, 640, pan_min, pan_max);
        targetTilt = floatMap(y_pixel, 0, 480, tilt_min, tilt_max);
        
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

    float y_offset = 5.0;

    // Premik servomotorjev z varnostno omejitvijo
    panServo.write((int)constrain(currentPan, 10, 170));
    tiltServo.write((int)constrain(currentTilt + y_offset, 10, 170));
  }
}
