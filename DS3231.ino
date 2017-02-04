/***************************************************
 * Programa para automatizar el timbre que marca   *
 * el inicio y fin de cada periodo de secundaria   *
 * en Colegio Americano del Pacifico               *
 ***************************************************
 * Autor: M en C Luis Enrique Ramos Maldonado      *
 ***************************************************
 * Circuito y codigo pueden ser vistos/descargados en:
 *  https://github.com/ramosluis/Timbre-CAP
 *  Libreria RTC:
 *  http://www.RinkyDinkElectronics.com/
 */

// Importar libreria de RTC y LCD
#include <DS3231.h>
#include <LiquidCrystal.h>

// Inicializar RTC via I2C
DS3231  rtc(SDA, SCL);

// Definir pines de entrada y salida
#define horario 3  // switch para cambiar entre horario de verano e invierno
#define transistor 2  // transistor que hace que conmute el relevador que activa el timbre
#define boton_manual   // boton para activar el timbre manualmente

// Inicializar la libreria LCD con los pines de interfaz definidos anteriormente
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

int currentHour, currentMinute, currentSecond;
String currentTime = "", dayWeek = "";

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(transistor, OUTPUT);
  pinMode(horario, INPUT);

  // Definir el numbero de filas y columnas del LCD:
  lcd.begin(16, 2);

  // Setup Serial connection >>>> DEBUG ONLY
  // Serial.begin(115200);
  
  // Initialize the rtc object
  rtc.begin();
  
  // The following lines can be uncommented to set the date and time
//  rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
//  rtc.setTime(14, 59, 55);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(1, 2, 2017);   // Set the date to January 1st, 2014
}

void loop()
{
  lcd.setCursor(0,0);

  // si el dia, mes, hora o segundo estan en un solo digito, ie < 10
  // agregamos un cero antes de ese digito para mantener consistente
  // la salida a la pantalla LCD

  // imprimir fecha en formato dd/mm/yyyy
  lcd.print(rtc.getDateStr(FORMAT_LONG,FORMAT_LITTLEENDIAN,'/')); //dd.mm.yyyy

  lcd.setCursor(13,0);

  // imprimir dia de la semana
  dayWeek = rtc.getDOWStr(FORMAT_SHORT);
  lcd.print(dayWeek);

  // imprimir hora 
  lcd.setCursor(0,1);
  currentTime = rtc.getTimeStr();
  currentHour = currentTime.substring(0,2).toInt();
  currentMinute = currentTime.substring(3,5).toInt();
  currentSecond = currentTime.substring(6,8).toInt();

  if (currentHour < 10)
    lcd.print(0);
  lcd.print(currentHour);
  lcd.print(":");
  if (currentMinute < 10)
    lcd.print(0);
  lcd.print(currentMinute);
  lcd.print(":");
  if (currentSecond < 10)
    lcd.print(0);
  lcd.print(currentSecond);

  // si el switch esta encendido, se usa el horario de verano
  if (digitalRead(horario) == HIGH) 
  {
    // imprimir que se esta usando el horario de verano
    lcd.setCursor(13,1);
    lcd.print("VER");

    // encender y apagar alarma dependiendo del horario o si se presiona un boton
    if ( (dayWeek == "Mon" ||
    dayWeek == "Tue" ||
    dayWeek == "Wed" ||
    dayWeek == "Thu" ||
    dayWeek == "Fri") && 
    (currentSecond >= 0 && currentSecond < 5) && 
    ( (currentHour == 7 && (currentMinute == 0 || currentMinute == 50)) ||
    (currentHour == 8 && currentMinute == 40) ||
    (currentHour == 9 && (currentMinute == 30 || currentMinute == 45)) ||
    (currentHour == 10 && currentMinute == 35) ||
    (currentHour == 11 && currentMinute == 20) ||
    (currentHour == 12 && (currentMinute == 10 || currentMinute == 25)) ||
    (currentHour == 13 && currentMinute == 15) ||
    (currentHour == 14 && (currentMinute == 0 || currentMinute == 50)) ||
    (currentHour == 15 && currentMinute == 10) ) )
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(transistor, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(transistor, LOW);
    }
  }
  else
  {
    lcd.setCursor(13,1);
    lcd.print("INV");

    // encender y apagar alarma dependiendo del horario o si se presiona un boton
    if ( (dayWeek == "Mon" ||
    dayWeek == "Tue" ||
    dayWeek == "Wed" ||
    dayWeek == "Thu" ||
    dayWeek == "Fri") && 
    (currentSecond >= 0 && currentSecond < 5) && 
    ( (currentHour == 7 && currentMinute == 30) ||
    (currentHour == 8 && currentMinute == 15) ||
    (currentHour == 9 && (currentMinute == 0 || currentMinute == 45)) ||
    (currentHour == 10 && (currentMinute == 0 || currentMinute == 40)) ||
    (currentHour == 11 && currentMinute == 25) ||
    (currentHour == 12 && (currentMinute == 10 || currentMinute == 25)) ||
    (currentHour == 13 && currentMinute == 15) ||
    (currentHour == 14 && (currentMinute == 0 || currentMinute == 50)) ||
    (currentHour == 15 && currentMinute == 10) ) )
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(transistor, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(transistor, LOW);
    }
  }
  // reinicia LCD para que no se congele durante horas de oficina
  if ( (currentHour == 6 || currentHour == 10 || currentHour == 14) && currentMinute == 59 && currentSecond == 59)
  {
    lcd.clear();
    lcd.begin(16, 2);
  }
  // delay (500);
}
