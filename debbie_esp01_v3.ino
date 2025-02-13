#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//wifi configuration
#define BLYNK_TEMPLATE_ID "TMPL6D_Ndf1NJ"
#define BLYNK_TEMPLATE_NAME "debbie8266"
#define BLYNK_AUTH_TOKEN "h6AI52r9E2B-WdBFWrVzLU39LppFhuRV"
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

SoftwareSerial EspSerial(12, 13);
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);

char ssid[] = "MaryChen";
char pass[] = "mary123890"; 

//pins
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

#define DHT11_PIN A1
DHT dht(DHT11_PIN, DHT11);

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Bpin 2
int lightB = 0;
bool lightChanged = false;
unsigned long lastInterruptTime = 0;
unsigned long debounceDelay = 200;

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("this is debbie:)");
    delay(3000);
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
    pinMode(Bpin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Bpin), lightInterrupt, CHANGE);
}

//ultrasound sensors
int Dist(int EchoPin, int TrigPin) {
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(6);
    digitalWrite(TrigPin, LOW);
    long pulseDuration = pulseIn(EchoPin, HIGH);
    return pulseDuration / 58;
}

//motor
void forward() { motorControl(0, 100, 100, 0, "Forward"); }
void backward() { motorControl(100, 0, 0, 100, "Backward"); }
void left() { motorControl(0, 120, 80, 0, "Right"); }
void right() { motorControl(0, 80, 100, 0, "Left"); }
void brake() { motorControl(255, 255, 255, 255, "Brake"); }

void motorControl(int in1, int in2, int in3, int in4, const char* direction) {
    analogWrite(motorIN1, in1);
    analogWrite(motorIN2, in2);
    analogWrite(motorIN3, in3);
    analogWrite(motorIN4, in4);
    Serial.println(direction);
}

  //blynk control functions
BLYNK_WRITE(V2) { if (param.asInt() == 1) forward(); else brake(); }
BLYNK_WRITE(V3) { if (param.asInt() == 1) backward(); else brake(); }
BLYNK_WRITE(V4) { if (param.asInt() == 1) left(); else brake(); }
BLYNK_WRITE(V5) { if (param.asInt() == 1) right(); else brake(); }

//buzzer
void playTone(int frequency, int duration) {
  int period = 1000000 / frequency;
  int halfPeriod = period / 2;
  long cycles = duration * 1000L / period;
  for (long i = 0; i < cycles; i++) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(halfPeriod);
  }
}

//target board
void lightInterrupt() {
    unsigned long currentTime = millis();
    if (currentTime - lastInterruptTime > debounceDelay) {
        lightB = digitalRead(Bpin) == LOW;  // Detect: resitance HIGH dark, LOW = bright
        lightChanged = true;
        lastInterruptTime = currentTime;
    }
}

void loop() {

  //face
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    >  __  <    ");
    
  
    Blynk.run();

    //buzzer
    int D1 = Dist(EchoPin1, TrigPin1);
    int D2 = Dist(EchoPin2, TrigPin2);
    if (D1 < 12 || D2 < 12) {
        brake();
        playTone(NOTE_C4, 500);     
    }

    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;    
      
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
    
    //send data to blynk
      Blynk.virtualWrite(V1, temperature);
      Blynk.virtualWrite(V6, humidity);
      Blynk.virtualWrite(V14, D1);
      Blynk.virtualWrite(V15, D2);

    //target board sensor
      if (lightChanged) {
        Serial.println(lightB ? "Lights on!" : "Lights off!");
        Blynk.virtualWrite(V16,lightB);
        lightChanged = false;
        delay(1000);
    }
    }
}
