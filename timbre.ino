/***************************************************
 * Programa para automatizar el timbre que marca   *
 * el inicio y fin de cada periodo de secundaria   *
 * en Colegio Americano del Pacífico               *
 ***************************************************
 * Autor: M en C Luis Enrique Ramos Maldonado      *
 * Fecha: 04 de octubre de 2016                    *
 ***************************************************/
/* LCD Circuit:
  The circuit:
 * LCD Pin 1 VSS - GND
 * LCD Pin 2 VCC - +5V
 * LCD Pin 3 VEE - GND
 * LCD Pin 4 RS - Nano Pin 9
 * LCD Pin 5 R/W - GND
 * LCD Pin 6 E - Nano Pin 8
 * LCD Pin 11 DB4 - Nano Pin 7
 * LCD Pin 12 DB5 - Nano Pin 6
 * LCD Pin 13 DB6 - Nano Pin 5
 * LCD Pin 14 DB7 - Nano Pin 4
 */
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <Time.h>
#include <LiquidCrystal.h>

#define horario 3
#define transistor 2
#define boton_manual 
RTC_DS1307 rtc;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

int segundo, minuto, hora, dia;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
void setup () 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(transistor, OUTPUT);
  pinMode(horario, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2016, 10, 21, 20, 39, 0));
  }
  // descomentar y ajustar el siguiente comando para recalibrar rtc
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(2016, 11, 2, 14, 49, 55));
}

void loop () 
{
  if (digitalRead(horario) == HIGH) // si es horario de verano
  {
    DateTime now = rtc.now();
    // imprimir al puerto serial la fecha y hora actual
    // en realidad no se necesita pero me ayuda en el debugging
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(" (");
//    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
//    Serial.print(") ");
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(now.year());
    lcd.print("/");
    lcd.print(now.month());
    lcd.print("/");
    lcd.print(now.day());
    
    lcd.setCursor(0,1);
    lcd.print(now.hour());
    lcd.print(":");
    lcd.print(now.minute());
    lcd.print(":");
    lcd.print(now.second());
    lcd.setCursor(13,1);
    lcd.print("VER");
  
    // lunes = 0 y domingo = 7
    dia = now.dayOfTheWeek();
    hora = now.hour();
    minuto = now.minute();
    segundo = now.second();
  
    // encender y apagar alarma dependiendo del horario o si se presiona un boton 
    if ( (dia >= 0 && dia <= 5) && (segundo == 0) && ( (hora == 7 && (minuto == 0 || minuto == 50)) || (hora == 8 && minuto == 40) || (hora == 9 && (minuto == 30 || minuto == 45)) || (hora == 10 && minuto == 35) || (hora == 11 && minuto == 20) || (hora == 12 && (minuto == 10 || minuto == 25)) || (hora == 13 && minuto == 15) || (hora == 14 && (minuto == 0 || minuto == 50)) || (hora == 15 && minuto == 10) ) )
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(transistor, HIGH);
    }
    else if (segundo == 5)
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(transistor, LOW);
    }
    Serial.println();
  }
  else // si es invierno
  {
    DateTime now = rtc.now();
    // imprimir al puerto serial la fecha y hora actual
    // en realidad no se necesita pero me ayuda en el debugging
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(" (");
//    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
//    Serial.print(") ");
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(now.year());
    lcd.print("/");
    lcd.print(now.month());
    lcd.print("/");
    lcd.print(now.day());
    
    lcd.setCursor(0,1);
    lcd.print(now.hour());
    lcd.print(":");
    lcd.print(now.minute());
    lcd.print(":");
    lcd.print(now.second());
    lcd.setCursor(13,1);
    lcd.print("INV");
  
    // lunes = 0 y domingo = 7
    dia = now.dayOfTheWeek();
    hora = now.hour();
    minuto = now.minute();
    segundo = now.second();
  
    // encender y apagar alarma dependiendo del horario o si se presiona un boton 
    if ( (dia >= 0 && dia <= 5) && (segundo == 0) && ( (hora == 7 && minuto == 30) || (hora == 8 && minuto == 15) || (hora == 9 && (minuto == 0 || minuto == 45)) || (hora == 10 && (minuto == 0 || minuto == 40)) || (hora == 11 && minuto == 25) || (hora == 12 && (minuto == 10 || minuto == 25)) || (hora == 13 && minuto == 15) || (hora == 14 && (minuto == 0 || minuto == 50)) || (hora == 15 && minuto == 10) ) )
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(transistor, HIGH);
    }
    else if (segundo == 5)
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(transistor, LOW);
    }
    Serial.println();
  }
}
