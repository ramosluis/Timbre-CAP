/***************************************************
 * Programa para automatizar el timbre que marca   *
 * el inicio y fin de cada periodo de secundaria   *
 * en Colegio Americano del Pacífico               *
 ***************************************************
 * Autor: M en C Luis Enrique Ramos Maldonado      *
 ***************************************************/
/* Circuito LCD:
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

// Importar funciones de fecha y hora para el DS1307 conectado via I2C
#include <Wire.h>
#include "RTClib.h"
#include <Time.h>
#include <LiquidCrystal.h>

// Definir pines de entrada y salida
#define horario 3  // switch para cambiar entre horario de verano e invierno
#define transistor 2  // transistor que hace que conmute el relevador que activa el timbre
#define boton_manual   // boton para activar el timbre manualmente

RTC_DS1307 rtc;  // Objeto de RTC para leer la fecha y hora

// Inicializar la libreria LCD con los pines de interfaz definidos anteriormente
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

int segundo, minuto, hora, dia;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday",
				"Thursday", "Friday", "Saturday"};
void setup () 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(transistor, OUTPUT);
  pinMode(horario, INPUT);

  // Definir el numbero de filas y columnas del LCD:
  lcd.begin(16, 2);

  // Debugging
  Serial.begin(57600);
  if (! rtc.begin()) {
    lcd.setCursor(0,0);
    lcd.print("No RTC found!");
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Mas debugging
  if (! rtc.isrunning()) {
    lcd.clear();
    lcd.print("RTC NOT running");
    Serial.println("RTC is NOT running!");
    // las siguientes lineas ajustan el RTC a la fecha y hora que el codigo
    // es compilado, corre solo en caso de que no este corriendo
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Esta linea ajusta el RTC a una fecha y hora explicitas, util para
    // deubugging
    // rtc.adjust(DateTime(2016, 10, 21, 20, 39, 0));
  }
  // Descomentar y ajustar el siguiente comando para recalibrar rtc
  // no se recomienda a menos que sepan que estan haciendo, mejor usar las
  // funciones en el bloque de arriba
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // rtc.adjust(DateTime(2017, 1, 13, 6, 59, 59));
  lcd.clear();
}

void loop () 
{
  // si el switch esta encendido, se usa el horario de verano
  if (digitalRead(horario) == HIGH) 
  {
    DateTime now = rtc.now();
    lcd.setCursor(0,0);
    // si el dia, mes, hora o segundo estan en un solo digito, ie < 10
    // agregamos un cero antes de ese digito para mantener consistente
    // la salida a la pantalla LCD

    // imprimir fecha en formato dia/mes/anio
    if (now.day() < 10)
      lcd.print("0");
    lcd.print(now.day());
    lcd.print("/");
    if (now.month() < 10)
      lcd.print("0");
    lcd.print(now.month());
    lcd.print("/");
    lcd.print(now.year());
    lcd.setCursor(13,0);

    // imprimir dia de la semana
    dia = now.dayOfTheWeek();
    if (dia == 1)
      lcd.print("LUN");
    if (dia == 2)
      lcd.print("MAR");
    if (dia == 3)
      lcd.print("MIE");
    if (dia == 4)
      lcd.print("JUE");
    if (dia == 5)
      lcd.print("VIE");    
    if (dia == 6)
      lcd.print("SAB");
    if (dia == 0)
      lcd.print("DOM");
    
    
    // imprimir hora 
    lcd.setCursor(0,1);
    if (now.hour() < 10)
      lcd.print("0");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10)
      lcd.print("0");
    lcd.print(now.minute());
    lcd.print(":");
    if (now.second() < 10)
      lcd.print("0");
    lcd.print(now.second());
    lcd.setCursor(13,1);
    // imprimir que se esta usando el horario de verano
    lcd.print("VER");
  
    hora = now.hour();
    minuto = now.minute();
    segundo = now.second();
  
    // encender y apagar alarma dependiendo del horario o si se presiona un boton
    if ( (dia >= 1 && dia <= 5) && (segundo == 0) && ( (hora == 7 && 
		(minuto == 0 || minuto == 50)) || (hora == 8 && minuto == 40) ||
		(hora == 9 && (minuto == 30 || minuto == 45)) || (hora == 10 &&
		minuto == 35) || (hora == 11 && minuto == 20) || (hora == 12 &&
		(minuto == 10 || minuto == 25)) || (hora == 13 && minuto == 15)
		|| (hora == 14 && (minuto == 0 || minuto == 50)) ||
		(hora == 15 && minuto == 10) ) )
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(transistor, HIGH);
    }
    else if (segundo == 5) // la alarma suena por 5 segundos
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(transistor, LOW);
    }
    Serial.println();
  }
  else // si es invierno
  {
    DateTime now = rtc.now();
    lcd.setCursor(0,0);
    if (now.day() < 10)
      lcd.print("0");
    lcd.print(now.day());
    lcd.print("/");
    if (now.month() < 10)
      lcd.print("0");
    lcd.print(now.month());
    lcd.print("/");
    lcd.print(now.year());
    lcd.setCursor(13,0);
    dia = now.dayOfTheWeek();
    if (dia == 1)
      lcd.print("LUN");
    if (dia == 2)
      lcd.print("MAR");
    if (dia == 3)
      lcd.print("MIE");
    if (dia == 4)
      lcd.print("JUE");
    if (dia == 5)
      lcd.print("VIE");    
    if (dia == 6)
      lcd.print("SAB");
    if (dia == 0)
      lcd.print("DOM");
    
    
    
    lcd.setCursor(0,1);
    if (now.hour() < 10)
      lcd.print("0");
    lcd.print(now.hour());
    lcd.print(":");
    if (now.minute() < 10)
      lcd.print("0");
    lcd.print(now.minute());
    lcd.print(":");
    if (now.second() < 10)
      lcd.print("0");
    lcd.print(now.second());
    lcd.setCursor(13,1);
    lcd.print("INV");
  
    hora = now.hour();
    minuto = now.minute();
    segundo = now.second();
  
    // encender y apagar alarma de acuerdo al horario de invierno
    if ( (dia >= 1 && dia <= 5) && (segundo == 0) && ( (hora == 7
		&& minuto == 30) || (hora == 8 && minuto == 15) || (hora == 9 &&
		(minuto == 0 || minuto == 45)) || (hora == 10 && (minuto == 0 ||
		minuto == 40)) || (hora == 11 && minuto == 25) || (hora == 12 &&
		(minuto == 10 || minuto == 25)) || (hora == 13 && minuto == 15) ||
		(hora == 14 && (minuto == 0 || minuto == 50)) || (hora == 15
		&& minuto == 10) ) )
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
