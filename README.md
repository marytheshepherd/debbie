# debbie
This project is built in fulfillment of the requirements for the module ET1024-2420: Introduction to Engineering &amp; Design CA3. 
It aims to develop a desk-stationed companion robot, debbie, designed to combat loneliness in the comfort of our homes.  

## Component Lists and Functions
**WiFi & Blynk IoT** [*Blynk by Volodymyr]  
**LCD screen** : *Displays an introductory message from debbie during setup, followed by display of its face.*    
**Motor** : *Controls the movement of debbie, which is managed remotely through the Blynk software.*    
**Ultrasounic sensor** *2 : *Detect obstacles in debbie’s path; when an obstacle is detected, the buzzer sounds, and debbie stops moving even if commands are sent from the app.*  
**Buzzer** : *Sounds when an obstacle is detected by the ultrasonic sensors*  
**Target Board** *1 : *Detects flashlight (representing darkness). When detected, LED automatically turns on; when the flashlight is removed (representing a well-lit room), the LED turns off.*  
**Temperature & Humidity Sensor** [*DHT sensor library by Adafruit] : *Measures and sends temperature and humidity data to the Blynk app.*   

## Usage
1) Test all the hardware seperately.  
2) Create Blynk account >> template >> Hardware: ESP8266 | Connection type: WiFi >> New Devices  
3) debbie_esp01_v3.ino
4) Blynk template id, name, auth token and personal hotspot  

## Common Error
```
ESP is not responding
```
Restart your robot  
Change laptop(some laptops don't have default file path configured)  
Change hotspot(might be network error)  
Change SoftwareSerial EspSerial(12, 13); to different ports e.g. (2,3)  
```
Compilation error: ESP8266_Lib.h: No such file or directory
```
download https://github.com/vshymanskyy/BlynkESP8266/blob/master/ESP8266_Lib.h  
paste in C:\Users\(yourname)\OneDrive\Documents\Arduino\libraries\Blynk\src  


> [!CAUTION]  
The ESP8266 (ESP-01) baud rate *must* be set to 9600, while the Serial.begin()/Serial Monitor baud rates can be either 9600 or 115200.
The code is currently at its maximum and will overload if added more  
Beware of damaging your components  








