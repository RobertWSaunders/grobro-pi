// ==============-------------------- GroBro ---------------------===================

#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeLib.h>

#define TEMP A3
#define LIGHTIN A1
#define LIGHTOUT 4
#define HEAT 5
#define HUMIDIFIER 6
#define TEMPA A0

int currenttemp = 0;
int currenttime;
boolean heat;
boolean humidifier;

enum cycle{
  veg,
  flower
};

const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int DHpin = 3;
byte dat [5];
byte read_data () {
  byte data;
  for (int i = 0; i < 8; i ++) {
    if (digitalRead (DHpin) == LOW) {
      while (digitalRead (DHpin) == LOW); // wait for 50us
      delayMicroseconds (30); // determine the duration of the high level to determine the data is '0 'or '1'
      if (digitalRead (DHpin) == HIGH)
        data |= (1 << (7-i)); // high front and low in the post
      while (digitalRead (DHpin) == HIGH); // data '1 ', wait for the next one receiver
     }
  }
return data;
}
 
void start_test () {
  digitalWrite (DHpin, LOW); // bus down, send start signal
  delay (30); // delay greater than 18ms, so DHT11 start signal can be detected
 
  digitalWrite (DHpin, HIGH);
  delayMicroseconds (40); // Wait for DHT11 response
 
  pinMode (DHpin, INPUT);
  while (digitalRead (DHpin) == HIGH);
  delayMicroseconds (80); // DHT11 response, pulled the bus 80us
  if (digitalRead (DHpin) == LOW);
  delayMicroseconds (80); // DHT11 80us after the bus pulled to start sending data
 
  for (int i = 0; i < 4; i ++) // receive temperature and humidity data, the parity bit is not considered
    dat[i] = read_data ();
 
  pinMode (DHpin, OUTPUT);
  digitalWrite (DHpin, HIGH); // send data once after releasing the bus, wait for the host to open the next Start signal
}


void setup() {
  Serial.begin(9600);
  pinMode(DHpin, OUTPUT);
  pinMode(LIGHTOUT, OUTPUT);
  pinMode(LIGHTIN, INPUT);
  pinMode(TEMP, INPUT);
  pinMode(TEMPA, INPUT);
  pinMode(HEAT, OUTPUT);
  pinMode(2, INPUT);

  lcd.begin(16, 2);
}


void loop() { //-------------------------------------------LOOOP-------------------------------------------------------
  int lightlvl = analogRead(LIGHTIN);
  start_test();
  int temperature = dat [2];
  currenttemp = temperature;
  int humidity = dat [0];
  cycle cyc = digitalRead(2);
  boolean lightstate;

  currenttime = hour();
  
  if(cyc == veg && humidity < 60){
    digitalWrite(6, 1);
    humidifier=1;
 }
 else if( cyc == flower && humidity < 30){
    digitalWrite(6, 1);
    humidifier=1;
 }
  else {
    digitalWrite(6, 0);
    humidifier=0;
  }

  if( cyc == veg) {
    lcd.print("  veg    ");
  }
  else{
    lcd.print("  bloom");
  }

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.print(" ");
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("% ");
  lcd.print("L:");
  if( lightlvl >= 400){
    lcd.print("off");
    lightstate=0;
  }
  else{
   lcd.print("on ");
   lightstate=1;
  }

  Serial.println("");
  Serial.print(temperature);
  Serial.print(" ");
  Serial.print(humidity);
  Serial.print(" ");
  Serial.print(lightstate);
  Serial.print(" ");
  Serial.print(heat);
  Serial.print(" ");
  Serial.print(humidifier);
  //println(""");

  lcd.setCursor(0, 0);
  lcd.print(hour());
  lcd.print(":");
  lcd.print(minute());
  lcd.print(":");
  lcd.print(second());
  lcd.print(":");
  lcd.print(currenttime);
  
  lightcycle(cyc);

  delay (500);
}

void lightcycle(cycle cyc){

  // ---------------- READ TIME FROM THE RASBERRY PI --------
  int temp = dat [2];
   if (cyc == veg){
    if (currenttime < 18){
      digitalWrite(LIGHTOUT, 1);
      if (temp < 25){
        digitalWrite(HEAT, 1);
        heat=1;
      }
      else{
        digitalWrite(HEAT, 0);
        heat=0;
      }
    }
    else{
      digitalWrite(LIGHTOUT, 0);
      if(temp < 16){
        digitalWrite(HEAT, 1);
        heat=1;
      }
      else{
        digitalWrite(HEAT, 0);
        heat =0;
      }
    }
  }

  else if (cyc == flower){
      if (currenttime < 12){
        digitalWrite(LIGHTOUT, 1);
           if (temp < 25){
          digitalWrite(HEAT, 1);
          heat=1;
        }
        else{
          digitalWrite(HEAT, 0);
          heat=0;
        }
      }
      else{
        digitalWrite(LIGHTOUT, 0);
          if(temp < 16){
       digitalWrite(HEAT, 1);
       heat=1;
     }
     else{
       digitalWrite(HEAT, 0);
       heat=0;
     }
    }
  }

  else{
    digitalWrite(LIGHTOUT, 1);
  }
}


