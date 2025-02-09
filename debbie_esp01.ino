#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <DHT.h>

//Blynk credentials
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#define TrigPin1 11   
#define EchoPin1 10   
#define TrigPin2 8   
#define EchoPin2 7
#define motorIN1 9
#define motorIN2 6
#define motorIN3 5
#define motorIN4 3

#define buzzerPin A0

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494



int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4 };
int noteDurations[] = { 500, 500, 500, 500, 500, 500, 500 };
#define DHT11_PIN A1

DHT dht(DHT11_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial EspSerial(12, 13);
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);

//input your mobile hotspot
char ssid[] = "";
char pass[] = ""; 

unsigned long previousMillis = 0;
const long interval = 2000;
#define Apin 4
#define Bpin 2
int lightA =0;
int lightB =0;
bool lightChanged = false;
bool lightChanged2 = false;
void lightInterrupt2(void);
void lightInterrupt(void);
unsigned long lastInterruptTime = 0;
unsigned long lastInterruptTime2 = 0;
unsigned long debounceDelay = 200;
unsigned long debounceDelay2 = 200;
unsigned long startMillis;   
const long waitTime = 5000;  
bool hasPrinted = false;

void setup() {
  lcd.init();
    lcd.backlight();
    lcd.clear();
    

    Serial.begin(115200);
    EspSerial.begin(ESP8266_BAUD);
    delay(10);

    Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
    dht.begin();

    pinMode(TrigPin1, OUTPUT);  
    pinMode(EchoPin1, INPUT); 
    pinMode(TrigPin2, OUTPUT); 
    pinMode(EchoPin2, INPUT);

    pinMode(motorIN1, OUTPUT);
    pinMode(motorIN2, OUTPUT);
    pinMode(motorIN3, OUTPUT);
    pinMode(motorIN4, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(Apin,INPUT_PULLUP);
  pinMode(Bpin,INPUT_PULLUP);
     attachInterrupt(digitalPinToInterrupt(Bpin), lightInterrupt, CHANGE);
     attachInterrupt(digitalPinToInterrupt(Apin), lightInterrupt2,CHANGE);


    
    
    
}

void loop() {
  
  
    
    Blynk.run();
    unsigned long currentMillis = millis();
  

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
     

        int D1 = Dist(EchoPin1, TrigPin1);
        int D2 = Dist(EchoPin2, TrigPin2);
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("F:"); lcd.print(D1); lcd.print("cm B:"); lcd.print(D2); lcd.print("cm");

        lcd.setCursor(0, 1);
        lcd.print("T:"); lcd.print(temperature); lcd.print("C H:"); lcd.print(humidity); lcd.print("%");
        
        if (D1 < 6){
      delay(100);
      playTone(melody[1], noteDurations[1]);
    brake();
 
   

    }
    else if (D2 < 6){
      
     
      delay(1000);
      playTone(melody[2], noteDurations[1]);
      brake();
   
    }
        
    }

if (lightChanged) {
        Serial.println(lightB ? "Lights on!" : "Lights off!");
        lcd.clear();
        lcd.print(lightB ? "Lights on!" : "Lights off!");
        lightChanged = false;
}
    
          } 

int Dist(int EchoPin, int TrigPin) {
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    long pulseDuration = pulseIn(EchoPin, HIGH);
    return pulseDuration / 58;
}

BLYNK_WRITE(V2) {
  int state = param.asInt();
  if (state == 1) {
    forward();
    

    }
    else{
      brake();
    }
  }
  

BLYNK_WRITE(V3) {
  int state = param.asInt();
  if (state == 1) {
    backward();
  }
  else{
    brake();
  }
  
}

BLYNK_WRITE(V4) {
  int state = param.asInt();
  if (state == 1) {
    left();
  }
  else{
    brake();
  }
}


BLYNK_WRITE(V5) {
  int state = param.asInt();
  if (state == 1) {
    right();
  }
  else{
    brake();
  }
}

void forward() {
  analogWrite(motorIN1, 100 );
  digitalWrite(motorIN2, LOW);
  analogWrite(motorIN3, 100);
  digitalWrite(motorIN4,LOW);
  Serial.print("Forward");
  Serial.print("\n");
}

void backward() {
  digitalWrite(motorIN1, LOW);
  analogWrite(motorIN2, 100); 
  digitalWrite(motorIN3,LOW);
  analogWrite(motorIN4, 100);
  Serial.print("Backward");
  Serial.print("\n");
}

void left() {
  analogWrite(motorIN1, 100 );
  digitalWrite(motorIN2, LOW);
  analogWrite(motorIN3, 80);
  digitalWrite(motorIN4,LOW);
  Serial.print("Left");
  Serial.print("\n");
}

void right() {
  analogWrite(motorIN1,80 );
  digitalWrite(motorIN2, LOW);
  analogWrite(motorIN3, 100);
  digitalWrite(motorIN4,LOW);
  Serial.print("Right");
  Serial.print("\n");
}

void brake() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorIN3, LOW);
  digitalWrite(motorIN4, LOW);
  Serial.print("Brake");
  Serial.print("\n");
}

void playTone(int frequency, int duration) {
  int period = 1000000 / frequency; // Calculate the period in microseconds
  int halfPeriod = period / 2;     // Half period for HIGH and LOW
  long cycles = duration * 1000L / period; // Total number of cycles for the duration

  for (long i = 0; i < cycles; i++) {
    digitalWrite(buzzerPin, HIGH); // Generate HIGH signal
    delayMicroseconds(halfPeriod);
    digitalWrite(buzzerPin, LOW); // Generate LOW signal
    delayMicroseconds(halfPeriod);
  }
}
void lightInterrupt() {
  unsigned long currentTime = millis();
  delay(1000);
  if (currentTime - lastInterruptTime > debounceDelay) { // Ignore fast repeats
  delay(1000);
    lightB = !lightB; // Toggle light state
    lightChanged = true;
    lastInterruptTime = currentTime; // Update last interrupt time
  }
}



