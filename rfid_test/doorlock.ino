/*
* Arduino Door Lock Access Control Project
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: MFRC522, https://github.com/miguelbalboa/rfid
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9
#define SS_PIN    10

byte readCard[4];
char* myTags[100] = {};
int tagsCount = 0;
String tagID = "";
boolean successRead = false;
boolean correctTag = false;
int proximitySensor;

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // Initiating
  SPI.begin();        // SPI bus
  mfrc522.PCD_Init(); //  MFRC522

  Serial.print("-No Master Tag!-");
  Serial.print("SCAN NOW");
  // Waits until a master card is scanned
  while (!successRead) {
    successRead = getID();
    if ( successRead == true) {
      myTags[tagsCount] = strdup(tagID.c_str()); // Sets the master tag into position 0 in the array
      Serial.print("Master Tag Set!");
      tagsCount++;
    }
  }
  successRead = false;
  printNormalModeMessage();
}

void loop() {
  int proximitySensor = analogRead(A0);
  if (proximitySensor > 200) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      return;
    }
    tagID = "";
    // The MIFARE PICCs that we use have 4 byte UID
    for ( uint8_t i = 0; i < 4; i++) {  //
      readCard[i] = mfrc522.uid.uidByte[i];
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading

    correctTag = false;
    // Checks whether the scanned tag is the master tag
    if (tagID == myTags[0]) {
      Serial.print("Program mode:");
      Serial.print("Add/Remove Tag");
      while (!successRead) {
        successRead = getID();
        if ( successRead == true) {
          for (int i = 0; i < 100; i++) {
            if (tagID == myTags[i]) {
              myTags[i] = "";
              Serial.print("Tag Removed!");
              printNormalModeMessage();
              return;
            }
          }
          myTags[tagsCount] = strdup(tagID.c_str());
          Serial.print("Tag Added!");
          printNormalModeMessage();
          tagsCount++;
          return;
        }
      }
    }
    successRead = false;
    // Checks whether the scanned tag is authorized
    for (int i = 0; i < 100; i++) {
      if (tagID == myTags[i]) {
        Serial.print("Access Granted!");
        printNormalModeMessage();
        correctTag = true;
      }
    }
    if (correctTag == false) {
      Serial.print("Access Denied!");
      printNormalModeMessage();
    }
  }
  else {
    while (!doorOpened) {
      proximitySensor = analogRead(A0);
      if (proximitySensor > 200) {
        doorOpened = true;
      }
    }
    doorOpened = false;
    delay(500);
    printNormalModeMessage();
  }
}

uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

void printNormalModeMessage() {
  delay(1500);
  Serial.print("-Access Control-");
  Serial.print(" Scan Your Tag!");
}