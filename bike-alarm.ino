/** MPU6050 **/
#include<Wire.h>

const int MPU_addr=0x68;
const float xThreshold = 20;  // Max X angle tilt before blowing the alarm
const float yThreshold = 20;  // Max Y angle tilt before blowing the alarm
const float zThreshold = 150; // Max Z angle tilt before blowing the alarm (currently disabled)
int16_t AcX,AcY,AcZ;  // MPU6050 variables
int minVal=265; // MPU6050 variables
int maxVal=402; // MPU6050 variables

double angles[3]; // Array to store the angles being read live, and compare them to armedAngles.
double armedAngles[3];  // Array to store the angles when alarm is armed
/** //MPU6050 **/



/** RC522 **/
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN  9    //Pin 9 RC522 reset
#define SS_PIN  10   //Pin 10 for RC522 SS (SDA)
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create RC522 object
/** //RC522 **/


int relayPin = 3; // Pin to activate/deactivate the relay that blows the alarm.
boolean armed = false;  // Is the alarm armed or not.
boolean triggered = false;  // Has the alarm been triggered or not.
long maxLapse; // Millis until the alarm is going to be beeping. Setted when alarm blows.
long cooldown; // Millis until alarm blows again once it was muted if conditions are met again. Setted when maxLapse has passed.

void setup()
{
  Serial.begin(9600);
  initializeRC522();
  initializeMPU6050();
  pinMode(relayPin,OUTPUT);
  toggleAlarm();
}
void loop()
{
  if(triggered){  // In case the alarm is triggered    
    if(checkElapsedTime()){ // Check if maxLapse has already passed. Doesn't disarm the alarm, just mutes it.
      digitalWrite(relayPin, LOW);
      triggered = false;
      cooldown = millis() + 10000; // We add 10 seconds of cooldown until alarm blows again if conditions are met.
    }
    else if(readRFID()){ // Check if card is present to disarm the alarm
      digitalWrite(relayPin, LOW);
      delay(200);
      toggleAlarm();
      triggered = false;
    }
    else{
      /**
       * We need to "lose" some time blowing the alarm (~500ms)
       * If the program runs faster, the RC522 freezes and needs to be restarted.
       */

      // TYPE 1 ALARM
      /*
      digitalWrite(relayPin, HIGH);
      delay(400);
      digitalWrite(relayPin, LOW);
      delay(250);
      */

      // TYPE 2 ALARM
      
      digitalWrite(relayPin, HIGH);
      delay(250);
      digitalWrite(relayPin, LOW);
      delay(80);
      digitalWrite(relayPin, HIGH);
      delay(250);
      digitalWrite(relayPin, LOW);
      delay(80);
      

      // TYPE 3 ALARM
      /*
      digitalWrite(relayPin, HIGH);
      delay(100);
      digitalWrite(relayPin, LOW);
      delay(50);
      digitalWrite(relayPin, HIGH);
      delay(100);
      digitalWrite(relayPin, LOW);
      delay(50);
      digitalWrite(relayPin, HIGH);
      delay(100);
      digitalWrite(relayPin, LOW);
      delay(50);
      */
    }
  }
  else{
    if(armed){  // Check if alarm is armed
      if((millis() > cooldown) && compareStoredAngles()){  // Check for any tilt to trigger the alarm
        maxLapse = millis() + 30000;
        triggered = true;
      }
      else
        delay(1000);
    }
    
    if(!triggered){ // Unless the alarm has just been triggered. Very rare/improbable usecase
      if(readRFID())  // Check for card in range to arm/disarm the alarm.
        toggleAlarm();
        
      delay(1000);
    }
  }
} 

/**
 * Arms / disarms the alarm.
 */
void toggleAlarm(){
  armed = !armed;
  if(armed){
    readAngles(armedAngles);

    digitalWrite(relayPin, HIGH);
    delay(250);
    digitalWrite(relayPin, LOW);
  }
  else{
    digitalWrite(relayPin, HIGH);
    delay(50);
    digitalWrite(relayPin, LOW);
    delay(50);
    digitalWrite(relayPin, HIGH);
    delay(50);
    digitalWrite(relayPin, LOW);
  }
}

/**
 * Compares if maxLapse has been reached in order to mute the alarm.
 * @return true if maxLapse has passed, false if hasn't
 */
boolean checkElapsedTime(){
  return millis() > maxLapse;
}
