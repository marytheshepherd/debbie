# debbie
This project is built in fulfillment of the requirements for the module ET1024-2420: Introduction to Engineering &amp; Design CA3. 
It aims to develop a desk-stationed companion robot, Debbie, designed to combat loneliness in the comfort of our homes.  
  
__How can we help individuals in Singapore combat loneliness through the use of technology? (UNSDG 3)__

## Component Lists
WiFi & Blynk IoT [*Blynk by Volodymyr]  
Motor  
Ultrasound *2  
Target Board *1  
LCD screen  
Temperature & Humidity Sensor [*DHT sensor library by Adafruit]  
Buzzer  

## Usage
1) Test all the hardware seperately.  
2) Create Blynk account >> template >> Hardware: ESP8266 | Connection type: WiFi >> New Devices  
3) debbie_esp01.ino
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








