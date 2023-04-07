/* 
 * Transmitter code that will run on a handheld controller
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // nRF24L01 (CE, CSN)
const byte address[6] = "00001";

struct Data_Package {
  byte LBB; // Left Bumper Button
  byte RBB; // Right Bumper Burron
  byte LY; // Left Yellow Button
  byte RY; // Right Yellow Button

  byte RS_V; // Right Stick Vertical
  byte RS_H; // Right Stick Horizontal
  byte LS_V; // Left Stick Vertical
  // byte j2PotY;
};
Data_Package data;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);

  pinMode(2, INPUT_PULLUP); // Left Bumper Button (LBB)
  pinMode(3, INPUT_PULLUP); // Right Bumper Button (RBB)
  pinMode(4, INPUT_PULLUP); // Left Yellow Button (LY)
  pinMode(5, INPUT_PULLUP); // Right Yellow Button (RY)

  data.RS_V = 90;
  data.RS_H = 90;
  data.LS_V = 90;
  // data.j2PotY = 90;
}

void loop() {
  data.RS_V = map(analogRead(A0), 0, 1023, 130, 50);
  data.RS_H = map(analogRead(A1), 0, 1023, 130, 50);
  data.LS_V = map(analogRead(A2), 0, 1023, 0, 180);
  // data.j2PotY = map(analogRead(A3), 0, 1023, 0, 180);

  data.LBB = digitalRead(2);
  data.RBB = digitalRead(3);
  data.LY = digitalRead(4);
  data.RY = digitalRead(5);

  printData();

  radio.write(&data, sizeof(Data_Package));
}

void printData() {
  Serial.print("RS_V: ");
  Serial.print(data.RS_V);
  Serial.print(", RS_H: ");
  Serial.print(data.RS_H);
  Serial.print(", LS_V: ");
  Serial.print(data.LS_V);
  // Serial.print(", LS Horizontal: ");
  // Serial.print(data.j2PotY);
  Serial.print(", LBB: ");
  Serial.print(digitalRead(2));
  Serial.print(", RBB: ");
  Serial.print(digitalRead(3));
  Serial.print(", LY: ");
  Serial.print(digitalRead(4));
  Serial.print(", RY: ");
  Serial.print(digitalRead(5));
  Serial.println();
}
