//20170713,Mobile Weather Station,by Hou Wenxuan
//Under CC BY-SA 3.0 CN
//V1.3_BETA
//          :)
/*
  Any Arduino pins labeled:  SDA  SCL
  Uno, Redboard, Pro:        A4   A5
  Mega2560, Due:             20   21
  Leonardo:                   2    3
*/
//#include <Wire.h>

#include <SFE_BMP180.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

#define C_TIME 500 //Cycle Time in Loop

SFE_BMP180 pressure;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int addr = 0;
double T[119];
int addrT = 0;

void setup() {

  //int n; //just a number :)

  /*Serial.begin(9600);
    Serial.println("BOOTing~");
    if (pressure.begin())//Try to boot BMP180
    Serial.println("BMP180 init success");
    else
    {
    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1); // Pause forever.
    }*/

  if (!pressure.begin()) {
    while (1);
  }

  if (EEPROM.read(0) == 0) {  //EEPROM(0)==0 means no data in it,or means the next available ADDR..
    addr = 1;
  }
  else {
    addr = EEPROM.read(0);
  }

  for (int num = 0; num < 119; ++num) {//Clear thr Array
    T[num] = 0;
  }

  lcd.begin(16, 2);
  lcd.print("Boot Succeeded.");
  lcd.setCursor(0, 1);
  lcd.print(" -By Hou Wenxuan");
  delay(1000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Temp:");//6,0

  lcd.setCursor(10, 0);
  lcd.print("|");

  lcd.setCursor(10, 1);
  lcd.print("|");

  //lcd.setCursor(13,0);
  lcd.setCursor(12, 0);
  lcd.print("Alt:");//4,1

  lcd.setCursor(0, 1);
  lcd.print("Pre:");//9,1
}

void loop() {
  char stat;
  double Temp, P, alt;//温度、气压、已知海平面气压计算的高度
  double p0 = 1013.2;//Sea Level Pressure
  double max_Temp = 0.0;
  int addrTT;//For "for"

  stat = pressure.startTemperature();//Getting T,P,Alt
  delay(stat);
  stat = pressure.getTemperature(Temp);
  delay(stat);
  stat = pressure.startPressure(3);
  delay(stat);
  stat = pressure.getPressure(P, Temp);
  alt = pressure.altitude(P, p0);

  /*
    Serial.print(Temp, 2);
    Serial.print("     ");
    Serial.print(P, 2);
    Serial.print("     ");
    Serial.println(alt, 2);*/

  delay(C_TIME);//Or it will be too fast to read

  lcd.setCursor(5, 0);
  lcd.print(Temp, 2);

  lcd.setCursor(11, 1);
  lcd.print(alt, 1);

  lcd.setCursor(4, 1);
  lcd.print(P, 2);

  lcd.setCursor(10, 1);
  lcd.print("|");


  T[addrT] = Temp;

  //DEBUGGING
  //Serial.print (T[addrT]);
  //Serial.print("       ");
  //Serial.println(addrT);

  addrT++;

  if (addrT == 119) {
    for (addrTT = 0; addrTT < 119; ++addrTT) {
      if (max_Temp < T[addrTT]) {
        max_Temp = T[addrTT];
      }
    }
    addrT = 0;
    EEPROM.write(addr, max_Temp);
    addr++;
  }
  EEPROM.write(0, addr);
  if (addr == EEPROM.length()) {
    addr = 1;
  }

  //DEBUGGING
  //Serial.print (EEPROM.read(addr));
  //Serial.print ("                ");
  //Serial.println(addr);

}
