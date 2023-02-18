
/******************************************Access Granted*******************************************/
void granted(int setDelay, String permission) {
  if (permission == "YES") {
    digitalWrite(BLUELED, LED_OFF);  // Turn off blue LED
    digitalWrite(REDLED, LED_OFF);   // Turn off red LED
    digitalWrite(GREENLED, LED_ON);  // Turn on green LED
  } else {
    analogWrite(BLUELED, (LED_ON - 100));  // Turn off blue LED
    digitalWrite(REDLED, LED_OFF);   // Turn off ßred LED
    digitalWrite(GREENLED, LED_OFF);  // Turn on green LED
  }

  digitalWrite(RELAYDOOR, LOW);   // Unlock door!
  delay(setDelay);                // Hold door lock open for given seconds
  digitalWrite(RELAYDOOR, HIGH);  // Relock door
}
/***************************************************************************************************/

/*****************************************Access Denied*********************************************/
void denied() {
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is off
  digitalWrite(BLUELED, LED_OFF);   // Make sure blue LED is off
  digitalWrite(REDLED, LED_ON);     // Turn on red LED
  delay(1000);
}
/***************************************************************************************************/

/*************************************Cycle Leds (Program Mode)*************************************/
void cycleLeds() {
  digitalWrite(REDLED, LED_OFF);   // Make sure red LED is off
  digitalWrite(GREENLED, LED_ON);  // Make sure green LED is on
  digitalWrite(BLUELED, LED_OFF);  // Make sure blue LED is off
  delay(200);
  digitalWrite(REDLED, LED_OFF);    // Make sure red LED is off
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is off
  digitalWrite(BLUELED, LED_ON);    // Make sure blue LED is on
  delay(200);
  digitalWrite(REDLED, LED_ON);     // Make sure red LED is on
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is off
  digitalWrite(BLUELED, LED_OFF);   // Make sure blue LED is off
  delay(200);
}
/***************************************************************************************************/

/***************************************Normal Mode Led*********************************************/
void normalModeOn() {
  digitalWrite(BLUELED, LED_ON);    // Blue LED ON and ready to read card
  digitalWrite(REDLED, LED_OFF);    // Make sure Red LED is off
  digitalWrite(GREENLED, LED_OFF);  // Make sure Green LED is off
  digitalWrite(RELAYDOOR, HIGH);    // Make sure Door is Locked
}
/***************************************************************************************************/

/***********************************Show Reader Details*********************************************/
void dsplreaderdetails() {
  // Get the MFRC522 software version
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown),probably a chinese clone?"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
    Serial.println(F("SYSTEM HALTED: Check connections."));
    while (true)
      ;  // do not go further
  }
}
/***************************************************************************************************/

/**************************************Success Write to EEPROM***************************************/
// Flashes the green LED 3 times to indicate a successful write to EEPROM
void successWrite() {
  digitalWrite(BLUELED, LED_OFF);   // Make sure blue LED is off
  digitalWrite(REDLED, LED_OFF);    // Make sure red LED is off
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is on
  delay(200);
  digitalWrite(GREENLED, LED_ON);  // Make sure green LED is on
  delay(200);
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is off
  delay(200);
  digitalWrite(GREENLED, LED_ON);  // Make sure green LED is on
  delay(200);
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is off
  delay(200);
  digitalWrite(GREENLED, LED_ON);  // Make sure green LED is on
  delay(200);
}
/***************************************************************************************************/

/*************************************Failed Write to EEPROM****************************************/
// Flashes the red LED 3 times to indicate a failed write to EEPROM
void failedWrite() {
  digitalWrite(BLUELED, LED_OFF);   // Make sure blue LED is off
  digitalWrite(REDLED, LED_OFF);    // Make sure red LED is off
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is off
  delay(200);
  for (int x = 0; x < 3; x++)
  {
    digitalWrite(REDLED, LED_ON);  // Make sure blue LED is on
    delay(200);
    digitalWrite(REDLED, LED_OFF);  // Make sure red LED is off
    delay(200);
  }
  digitalWrite(REDLED, LED_OFF);  // just making sure the Red LED is OFF 
  digitalWrite(BLUELED, LED_ON);  // Make sure blue LED is on
}
/***************************************************************************************************/

/*********************************Success Remove UID From EEPROM************************************/
// Flashes the blue LED 3 times to indicate a success delete to EEPROM
void successRemove() {
  digitalWrite(BLUELED, LED_OFF);   // Make sure blue LED is off
  digitalWrite(REDLED, LED_OFF);    // Make sure red LED is off
  digitalWrite(GREENLED, LED_OFF);  // Make sure green LED is off
  delay(200);
  digitalWrite(REDLED, LED_ON);  // Make sure red LED is on
  delay(200);
  digitalWrite(REDLED, LED_OFF);  // Make sure red LED is off
  delay(200);
  digitalWrite(REDLED, LED_ON);  // Make sure red LED is on
  delay(200);
  digitalWrite(REDLED, LED_OFF);  // Make sure red LED is off
  delay(200);
  digitalWrite(REDLED, LED_ON);  // Make sure red LED is on
  delay(200);
}
/***************************************************************************************************/

/******************************************Print 2 LCD**********************************************/
void print2lcd(String str1, int c1, String str2, int c2) {
  lcd.clear();
  lcd.setCursor(c1, 0);
  lcd.print(str1);
  if (str2.length() > 0) {
    lcd.setCursor(c2, 1);
    lcd.print(str2);
  }
}
/***************************************************************************************************/
