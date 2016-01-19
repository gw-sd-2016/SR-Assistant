

#include <SoftwareSerial.h>
//// software serial #1: TX = digital pin 10, RX = digital pin 11
//SoftwareSerial portOne(10, 11);

// software serial #2: TX = digital pin 8, RX = digital pin 9
// on the Mega, use other pins instead, since 8 and 9 don't work on the Mega
SoftwareSerial portTwo(8, 9);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


//  // Start each software serial port
//  portOne.begin(9600);
  portTwo.begin(57600);
}

void loop() {
//  // By default, the last intialized port is listening.
//  // when you want to listen on a port, explicitly select it:
//  portOne.listen();
//  Serial.println("Data from port one:");
//  // while there is data coming in, read it
//  // and send to the hardware serial port:
//  while (portOne.available() > 0) {
//    char inByte = portOne.read();
//    Serial.write(inByte);
//  }
//
//  // blank line to separate data from the two ports:
  Serial.println();
  delay(1000);
  // Now listen on the second port
  portTwo.listen();
  // while there is data coming in, read it
  // and send to the hardware serial port:
  Serial.println("Data from port two:");
  while (portTwo.available() > 0) {
    char inByte = portTwo.read();
    Serial.write(inByte);
  }

  // blank line to separate data from the two ports:
  Serial.println();
}
