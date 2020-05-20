# Bike Alarm

Simple project to secure your (motor)bike with an alarm.
The alarm is (dis)armed with an RFID device.

---------------------------------------------------------
## Hardware

The hardware required for this project includes:
- Arduino UNO.
- MPU6050 Gyroscope and Accelerometer.
- RFID-RC522
- Active buzzer
- Relay
- Diode x1

Optional hardware:
- Breadboard
- 220 Ohm Resistor x1

### Wiring

![schematics](https://github.com/Bol-S/bike-alarm/blob/develop/img/schematics.jpg)

| MPU6050 | Arduino |
|-|-|
|VCC|+5V |
|GND| GND|
|SCL|A5|
|SDA|A4|


| RC522 | Arduino |
|-|-|
|+3.3|+5v (By the tests done, at 3.3 the NFC module ends up freezing)|
|RST| D9|
|GND|GND|
|IRQ|N/C|
|MISO|D12|
|MOSI|D11|
|SCK|D13|
|SDA|D10|

|                      RELAY                            |
|-------------------------------------------------------|
|	  Connect +VCC from Power Supply to Relay input. 	|
|	  Connect Relay's output to +VCC of Buzzer.   		|
|	  Connect PS' ground to buzzer's ground.     		|
|	  Connect Relay's control pin to D3 pin of Arduino	|
|	  Connect Relay's control pin GND to Arduino GND	|
|	  Optionally you can connect a resistor between		|
|	  		   relay's output and buzzer.			 	|
|	  !!!!! Connect a diode betwen relay's control + and - !!!!!!	|

**Although the alarm can be connected to +5v of Arduino, it is recommended to connect it to a high voltage power supply to achieve a loud beep.**

---------------------------------------------------------


## Software

### Prerequisites

This project makes use of the following libraries:
- I2Cdev by Jeff Rowberg - https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/I2Cdev
- MPU6050 by Jeff Rowberg - https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050 
- RC522 by Miguel Balboa - https://github.com/miguelbalboa/rfid


### Installing

After adding the required libraries, clone or download the repository into your local machine.

```
git clone https://github.com/Bol-S/bike-alarm.git
```

### Set Up
Once downloaded, rename *cards_sample.h* to *cards.h* and edit CARD_1 and CARD_2 with your cards' hashes. (Currently only 2 authorized cards allowed).
Upload code to arduino.

---------------------------------------------------------
## Usage
Bring one of your cards near the RC522 to arm the alarm, buzzer will beep once.
Bring one of your cards again near the RC522 to disarm the alarm, buzzer will beep twice.

When the alarm is armed, tilt the gyroscope; the alarm will trigger and blow. Beeping will continue for 30 seconds unless the alarm is disarmed with an authorized card. If the 30 seconds pass, there's a cooldown of 10 seconds until the alarm blows again (if the MPU6050 is still tilted).

##### Easily configurable variables
- Beep pattern when alarm is armed/disarmed.
- Beep pattern when alarm is triggered.
- Degrees tilted until alarm blows. (xyz Thresholds)
- Time that the alarm is beeping since it is triggered. (maxLapse)
- Cooldown until alarm blows again. (cooldown)
