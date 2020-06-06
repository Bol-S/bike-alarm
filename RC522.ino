#include "cards.h"; // Cards ids
byte readUID[4]; // Card close to RC522

/*
 * Setup of RC522
 */
void initializeRC522() {
  SPI.begin();
  mfrc522.PCD_Init();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max); // Set antenna to max gain (~5cm)
}

/*
 * Performs a lecture of card in range (if any) and compares it to the authorized ones.
 */
boolean readRFID() {   
  long t = millis(); 
  if (mfrc522.PICC_IsNewCardPresent()) {  // Check if new card is in range
    if (mfrc522.PICC_ReadCardSerial()){ // Read the card
          for (byte i = 0; i < mfrc522.uid.size; i++) {
              readUID[i]=mfrc522.uid.uidByte[i];          
          } 
        
        //Compare card in range to authorized cards
        if(compareArray(readUID, sizeof(readUID),CARD_1, sizeof(CARD_1)) ||
           compareArray(readUID, sizeof(readUID), CARD_2, sizeof(CARD_2))){
            mfrc522.PICC_HaltA();
            mfrc522.PCD_StopCrypto1();
            return true;
        }
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
    }
  }


  // If reading time is not between 20-30ms, RC522 has bricked. Needs to be re-initialized.
  if(millis() - t > 50 || millis() - t < 10){
    initializeRC522();
  }
return false;
}

/*
 * Compares length and content of two given byte arrays
 */
 boolean compareArray(byte array1[], int sizeArray1,byte array2[], int sizeArray2)
{
  if(sizeArray1 != sizeArray2)
    return false;
    
  for(int i = 0; i < sizeArray1; i++){
    if(array1[i] != array2[i])
      return false;
  }
  return true;
}
