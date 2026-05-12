#include <Arduino.h>
#include <SoftwareSerial.h>

// Pin 2 is RX -> Connects to Pi TXD0 (GPIO 14 / Pin 8)
// Pin 3 is TX -> Connects to your resistor shield / Pi RXD0 (GPIO 15 / Pin 10)
SoftwareSerial piSerial(2, 3); 

void setup() {
  Serial.begin(115200);   // Fast USB connection to your PC
  piSerial.begin(9600);   // Stable speed for the jumper wires
  Serial.println("--- Arduino Passthrough Online (9600 Baud) ---"); 
}

void loop() {
  // Read from PC Serial Monitor/Picocom, forward out Pin 3 to Pi
  while (Serial.available() > 0) {
    char outByte = Serial.read();
    piSerial.write(outByte);
  }
  
  // Read from Pin 2 from Pi, forward to your PC screen
  while (piSerial.available() > 0) {
    char inByte = piSerial.read();
    Serial.write(inByte);
  }
}
