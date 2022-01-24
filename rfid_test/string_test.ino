#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>

#include "SPI.h" // SPI library
#include "MFRC522.h" // RFID library (https://github.com/miguelbalboa/rfid)

const int pinRST = 9;
const int pinSDA = 10;

MFRC522 mfrc522(pinSDA, pinRST); // Set up mfrc522 on the Arduino

void setup() {
  SPI.begin(); // open SPI connection
  mfrc522.PCD_Init(); // Initialize Proximity Coupling Device (PCD)
  char hex_str[];// String to hold hex value ID
  Serial.begin(9600); // open serial connection
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) { // (true, if RFID tag/card is present ) PICC = Proximity Integrated Circuit Card
    if(mfrc522.PICC_ReadCardSerial()) { // true, if RFID tag/card was read
      Serial.print("RFID TAG ID: ");
      for (byte i = 0; i < mfrc522.uid.size; ++i) { // read id (in parts)
        hex_str[] = str(mfrc522.uid.uidByte[i], HEX); // Save ID to string
        Serial.print(hex_str); // Print string containing ID
      }
      Serial.println(); // Print out of id is complete.
    }
  }
}