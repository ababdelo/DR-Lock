
/******************************************Get PICC's UID*******************************************/

int getID() {
  // Getting ready for Reading PICCs
  if (!mfrc522.PICC_IsNewCardPresent()) {  //If a new PICC placed to RFID reader continue
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {  //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  Serial.println(F("Scanned PICC's UID:"));
  for (int x = 0; x < 4; x++) {  //
    readCard[x] = mfrc522.uid.uidByte[x];
    Serial.print(readCard[x], HEX);
  }
  readdatafromtag(block, uname);
  Serial.println();
  mfrc522.PICC_HaltA();       // Stop reading
  mfrc522.PCD_StopCrypto1();  //Used to exit the PCD from its authenticated state.
  // Remember to call this function after communicating with an authenticated PICC - otherwise no new communications can start.
  return 1;
}

/***************************************************************************************************/

/******************************** Check readCard IF is masterCard***********************************/

// Check to see if the ID passed is the master programing card
boolean isMaster(byte test[]) {
  if (checkTwo(test, masterCard))
    return true;
  else
    return false;
}

/***************************************************************************************************/

/*****************************************Find ID From EEPROM***************************************/

boolean findID(byte find[]) {
  int count = EEPROM.read(0);          // Read the first Byte of EEPROM that
  for (int i = 1; i <= count; i++) {   // Loop once for each EEPROM entry
    readID(i);                         // Read an ID from EEPROM, it is stored in storedCard[4]
    if (checkTwo(find, storedCard)) {  // Check to see if the storedCard read from EEPROM
      return true;
      //break;  // Stop looking we found it
    } else {}// If not, return false
  }
  return false;
}

/***************************************************************************************************/

/*****************************************Remove ID from EEPROM*************************************/

void deleteID(byte a[]) {
  if (!findID(a)) {  // Before we delete from the EEPROM, check to see if we have this card!
    failedWrite();   // If not
    Serial.println(F("Failed! There is something wrong with ID or bad EEPROM"));
  } else {
    int num = EEPROM.read(0);  // Get the numer of used spaces, position 0 stores the number of ID cards
    int slot;                  // Figure out the slot number of the card
    int start;                 // = ( num * 4 ) + 6; // Figure out where the next slot starts
    int looping;               // The number of times the loop repeats
    int j;
    int count = EEPROM.read(0);  // Read the first Byte of EEPROM that stores number of cards
    slot = findIDSLOT(a);        // Figure out the slot number of the card to delete
    start = (slot * 4) + 2;
    looping = ((num - slot) * 4);
    num--;                                                  // Decrement the counter by one
    EEPROM.write(0, num);                                   // Write the new count to the counter
    for (j = 0; j < looping; j++) {                         // Loop the card shift times
      EEPROM.write(start + j, EEPROM.read(start + 4 + j));  // Shift the array values to 4 places earlier in the EEPROM
    }
    for (int k = 0; k < 4; k++) {  // Shifting loop
      EEPROM.write(start + j + k, 0);
    }
    successRemove();
    Serial.println(F("Succesfully removed ID record from EEPROM"));
  }
}

/***************************************************************************************************/

/**************************************** Read an ID from EEPROM************************************/

void readID(int number) {
  int start = (number * 4) + 2;              // Figure out starting position
  for (int i = 0; i < 4; i++) {              // Loop 4 times to get the 4 Bytes
    storedCard[i] = EEPROM.read(start + i);  // Assign values read from EEPROM to array
  }
}

/***************************************************************************************************/

/****************************************Add ID to EEPROM*******************************************/

void writeID(byte a[]) {
  if (!findID(a)) {                   // Before we write to the EEPROM, check to see if we have seen this card before!
    int num = EEPROM.read(0);         // Get the numer of used spaces, position 0 stores the number of ID cards
    int start = (num * 4) + 6;        // Figure out where the next slot starts
    num++;                            // Increment the counter by one
    EEPROM.write(0, num);             // Write the new count to the counter
    for (int j = 0; j < 4; j++) {     // Loop 4 times
      EEPROM.write(start + j, a[j]);  // Write the array values to EEPROM in the right position
    }
    successWrite();
    Serial.println(F("Succesfully added ID record to EEPROM"));
  } else {
    failedWrite();
    Serial.println(F("Failed! There is something wrong with ID or bad EEPROM"));
  }
}

/***************************************************************************************************/

/*****************************************Check Bytes***********************************************/

boolean checkTwo(byte a[], byte b[]) {
  if (a[0] != 0)              // Make sure there is something in the array first
    match = true;                // Assume they match at first
  for (int k = 0; k < 4; k++) {  // Loop 4 times
    if (a[k] != b[k])            // IF a != b then set match = false, one fails, all fail
      match = false;
  }
  if (match) {    // Check to see if if match is still true
    return true;  // Return true
  } else {
    return false;  // Return false
  }
}

/***************************************************************************************************/

/*****************************************Find Slot*************************************************/

int findIDSLOT(byte find[]) {
  int count = EEPROM.read(0);          // Read the first Byte of EEPROM that
  for (int i = 1; i <= count; i++) {   // Loop once for each EEPROM entry
    readID(i);                         // Read an ID from EEPROM, it is stored in storedCard[4]
    if (checkTwo(find, storedCard)) {  // Check to see if the storedCard read from EEPROM is the same as the find[] ID card passed
      return i;                        // The slot number of the card
      //break;                           // Stop looking we found it
    }
  }
}

/***************************************************************************************************/

/*************************************Write Data 2 Tag**********************************************/

/*
  void WriteData2Tag(byte block, byte name[]) {
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else Serial.println(F("PCD_Authenticate() success: "));
  // Write block
  status = mfrc522.MIFARE_Write(block, name, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  } else Serial.println(F("MIFARE_Write() success: "));
  }
*/

/***************************************************************************************************/

/************************************Read Data from Tag*********************************************/
void readdatafromtag(byte block2readfrom, String name) {
  uname = "";
  // Prepare the factory security key for reading from the specific block
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  byte len = 18;
  byte buffer[18];
  // before any attempt to read or write on a block , that's block need to be authenticated by given the right factory key
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block2readfrom, &key, &(mfrc522.uid));  //"PCD_AUTHENTICATION" enable a secure communication
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(block2readfrom, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  for (int i = 0; i < 16; i++) {
    uname += (char)buffer[i];
  }
  uname.trim();  //removing all whitespace
  Serial.println();
  Serial.println(uname);
}
/***************************************************************************************************/
