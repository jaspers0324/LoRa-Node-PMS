#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Jasper_hpma115S0.h>
#include <SoftwareSerial.h>

const int csPin = 10;
const int resetPin = 9;       // LoRa radio reset
const int irqPin = 2;         // LoRa radio Interrupt 

// Feather TX, Feather RX
SoftwareSerial hpmaSerial(4, 3); 
//Create an instance of the hpma115S0 library
Jasper_HPMA115S0 hpma115S0(hpmaSerial);

void setup() {
  Serial.begin(9600);

  LoRa.setPins(csPin, resetPin, irqPin);

  hpmaSerial.begin(9600);
  hpma115S0.Init();
  hpma115S0.StartParticleMeasurement();
  
  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {
  unsigned int pm2_5, pm10;
  if (hpma115S0.ReadParticleMeasurement(&pm2_5, &pm10)) {
    Serial.println("PM 2.5: " + String(pm2_5) + " ug/m3" );
    Serial.println("PM 10: " + String(pm10) + " ug/m3" );
  }
  delay(1000);
 
  //Serial.print("Sending packet: ");
  //Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("PM 2.5=");
  LoRa.print(pm2_5);
  LoRa.print(" ");
  LoRa.print("PM 10=");
  LoRa.print(pm10);
  //LoRa.print(counter);
  LoRa.endPacket();

  //counter++;

  delay(1000);
}
