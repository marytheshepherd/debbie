//libraries
#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//wifi configuration
char ssid[] = "";  // Wi-Fi SSID
char pass[] = "";  // Wi-Fi Password

#define BLYNK_TEMPLATE_ID "TMPL6D_Ndf1NJ"
#define BLYNK_TEMPLATE_NAME "debbie8266"
#define BLYNK_AUTH_TOKEN "h6AI52r9E2B-WdBFWrVzLU39LppFhuRV"

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

//pin assignments
#define TrigPin1 11   
#define EchoPin1 10   
#define TrigPin2 8   
#define EchoPin2 7
#define motorIN1 9
#define motorIN2 6
#define motorIN3 5
#define motorIN4 3
#define buzzerPin A0
#define DHT11_PIN A1
#define Apin 4
#define Bpin 2

//notes
#define NOTE_C4  262
// #define NOTE_D4  294
// #define NOTE_E4  330
// #define NOTE_F4  349
// #define NOTE_G4  392
// #define NOTE_A4  440
// #define NOTE_B4  494



SoftwareSerial EspSerial(12, 13);  // Software Serial for ESP8266
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial);


//initialization
// int melody[] = { NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4};
// int noteDurations[] = { 500 };
DHT dht(DHT11_PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned long previousMillis = 0;
const long interval = 4000;
int lightB =0;
bool lightChanged = false;
void lightInterrupt(void);
unsigned long lastInterruptTime = 0;
unsigned long debounceDelay = 200;
unsigned long startMillis;   

//set up function
void setup() {
    Serial.begin(115200);

    lcd.init();
    lcd.backlight();
    delay(2000);

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
    pinMode(Apin, INPUT_PULLUP);
    pinMode(Bpin, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(Bpin), lightInterrupt, CHANGE);
}

void loop() {
    Blynk.run();
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        updateSensors();
    }
    
    if (lightChanged) {
        Serial.println(lightB ? "Lights on!" : "Lights off!");
        lcd.setCursor(0, 0);
        lcd.print(lightB ? "Lights on! " : "Lights off!");
        lightChanged = false;
    }
}

void updateSensors() {
    int D1 = Dist(EchoPin1, TrigPin1);
    int D2 = Dist(EchoPin2, TrigPin2);
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    lcd.clear();

    // Display ultrasound information on the first line
    lcd.setCursor(0, 0); //can remove this if blynk event work
    lcd.print("F:"); lcd.print(D1); lcd.print("cm B:"); lcd.print(D2); lcd.print ("cm  ");
    lcd.setCursor(0, 1);
    lcd.print("T:"); lcd.print(temperature); lcd.print("C H:"); lcd.print(humidity); lcd.print("%");

    Blynk.virtualWrite(V1, temperature); //send data to the blynk 
    Blynk.virtualWrite(V6, humidity);
    Blynk.virtualWrite(V14, D1);
    Blynk.virtualWrite(V15, D2);

    //add blynk event here
    
    if (D1 < 6 || D2 < 6) {
        playTone(NOTE_C4, 500);
        brake();
    }
}

//ultrasound sensor
int Dist(int EchoPin, int TrigPin) {
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(TrigPin, LOW);
    return pulseIn(EchoPin, HIGH) / 58;
}

//motor
void forward() { motorControl(100, 0, 100, 0, "Forward"); }
void backward() { motorControl(0, 100, 0, 100, "Backward"); }
void left() { motorControl(100, 0, 80, 0, "Left"); }
void right() { motorControl(80, 0, 100, 0, "Right"); }
void brake() { motorControl(0, 0, 0, 0, "Brake"); }

void motorControl(int in1, int in2, int in3, int in4, const char* direction) {
    analogWrite(motorIN1, in1);
    digitalWrite(motorIN2, in2);
    analogWrite(motorIN3, in3);
    digitalWrite(motorIN4, in4);
    Serial.println(direction);
}

//buzzer
void playTone(int frequency, int duration) {
    unsigned long start = millis();
    int period = 1000000 / frequency;
    int halfPeriod = period / 2;
    while (millis() - start < duration) {
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(halfPeriod);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(halfPeriod);
    }
}

//target board interrupt
void lightInterrupt() {
    unsigned long currentTime = millis();
    if (currentTime - lastInterruptTime > debounceDelay) {
        lightB = digitalRead(Bpin) == LOW;  // Detect: resitance HIGH dark, LOW = bright
        lightChanged = true;
        lastInterruptTime = currentTime;
    }
}

//blynk functions
BLYNK_WRITE(V2) { if (param.asInt() == 1) forward(); else brake(); }
BLYNK_WRITE(V3) { if (param.asInt() == 1) backward(); else brake(); }
BLYNK_WRITE(V4) { if (param.asInt() == 1) left(); else brake(); }
BLYNK_WRITE(V5) { if (param.asInt() == 1) right(); else brake(); }





