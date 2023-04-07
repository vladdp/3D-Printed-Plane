/* 
 * Receiver code that will run on the plane
 */
#include <Servo.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

Servo R_aileron, L_aileron, elevator, rudder;
Servo R_ESC, L_ESC;

RF24 radio(7, 8);
const byte address[6] = "00001";

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

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
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  rudder.attach(2);
  elevator.attach(3);
  R_aileron.attach(4);
  L_aileron.attach(5); 

  R_ESC.attach(9, 1000, 2000);
  L_ESC.attach(10, 1000, 2000);

  resetData();
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));
    lastReceiveTime = millis();
    Serial.print("RS_V: ");
    Serial.print(data.RS_V);
    Serial.print(" RS_H: ");
    Serial.print(data.RS_H);
    Serial.print(" LS_V: ");
    Serial.print(data.LS_V);
    // Serial.print(" j2PotY: ");
    // Serial.print(data.j2PotY-90);
    Serial.println();

    R_ESC.write(data.LS_V - 90);
    L_ESC.write(data.LS_V - 90);

    elevator.write(data.RS_V);
    if (data.LBB == 0 && data.RBB == 0) rudder.write(90);
    else if (data.LBB == 0) rudder.write(60);
    else if (data.RBB == 0) rudder.write(120);
    else rudder.write(90);
    R_aileron.write(data.RS_H);
    L_aileron.write(data.RS_H);   
  }
  currentTime = millis();

  if (currentTime - lastReceiveTime > 1000) {
    Serial.println("No Radio Signal.");
    Serial.print("RS_V: ");
    Serial.print(data.RS_V);
    Serial.print(" RS_H: ");
    Serial.print(data.RS_H);
    Serial.print(" LS_V: ");
    Serial.print(data.LS_V);
    // Serial.print(" j2PotY: ");
    // Serial.print(data.j2PotY-90);
    Serial.println();
    resetData();
    lastReceiveTime = currentTime;
  }
}

void resetData() {
  data.RS_V = 90;
  data.RS_H = 90;
  data.LS_V = 0;
  // data.j2PotY = 0;

  R_ESC.write(data.LS_V);
  L_ESC.write(data.LS_V);

  R_aileron.write(data.RS_H);
  L_aileron.write(data.RS_H);

  elevator.write(data.RS_V);

  rudder.write(90);
}
