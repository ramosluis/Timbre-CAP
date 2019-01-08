"""
Programa: timbre.py

Autor: Ing. Luis Enrique Ramos Maldonado, M. C.

Descripcion: Programa que usa el GPIO (general purpose input output)
de un Raspberry Pi para controlar digitalmente un timbre escolar. 

Sincroniza el tiempo por medio de internet y por medio de un transistor
y un relevador controla el timbre, el cual maneja corriente alterna.

Adicionalmente, se incluye un boton tipo push button para timbrar
manualmente si es necesario y un switch para cambiar entre horario de
invierno y horario de verano.


PINES USADOS:
TODO

"""

import datetime
import time
import pigpio

"""
DATETIME REFERENCE AND EXAMPLES

now = datetime.datetime.now()

print
print("Current date and time using str method of datetime object:")
print(str(now))

print
print("Current date and time using instance attributes:")
print("Current year: %d" % (now.year))
print("Current month: %d" % (now.month))
print("Current day: %d" % (now.day))
print("Current hour: %d" % (now.hour))
print("Current minute: %d" % (now.minute))
print("Current second: %d" % (now.second))
print("Current microsecond: %d" % (now.microsecond))

print
print("Current date and time using strftime:")
print(now.strftime("%Y-%m-%d %H:%M"))


print
print("Current date and time using isoformat:")
print(now.isoformat())

print
print("Current day of the week:") # Monday is 0 and Sunday is 6
print(datetime.datetime.today().weekday())
"""

# objeto para manipular GPIO del Pi
pi = pigpio.pi()

# GPIO 23 (switch) como input y 24 (transistor) como output
horario = 23
timbre = 24

verano = 1

pi.set_mode(horario, pigpio.INPUT) # pin 16 fisicamente en placa
pi.set_mode(timbre, pigpio.OUTPUT) #pin 18 fisicamente en placa

while True:
	time.sleep(1)
	now = datetime.datetime.now()
	dayWeek = int(datetime.datetime.today().weekday())
	hour = int(now.strftime("%H"))
	minutes = int(now.strftime("%M"))
	seconds = int(now.strftime("%S"))
	
	if pi.read(horario) is verano: # horario de verano
		if ( 
		(dayWeek >= 0 or dayWeek <= 4) and 
		(seconds >= 0 and seconds <= 5) and 
		(hour is 7 and (minutes is 0 or minutes is 50) ) or
		(hour is 8 and minutes is 40) or
		(hour is 9 and (minutes is 30 or minutes is 45) ) or
		(hour is 10 and minutes is 35) or
		(hour is 11 and minutes is 20) or
		(hour is 12 and (minutes is 10 or minutes is 25) ) or
		(hour is 13 and minutes is 15) or
		(hour is 14 and (minutes is 00 or minutes is 50) ) or
		(hour is 15 and minutes is 10)
		):
			pi.write(timbre, 1)
			print(pi.read(timbre))
		else:
			pi.write(timbre, 0)
			print(pi.read(timbre))
	else: # horario de invierno
		if ( 
		(dayWeek >= 0 or dayWeek <= 4) and 
		(seconds >= 0 and seconds <= 5) and 
		(hour is 7 and minutes is 30) or
		(hour is 8 and minutes is 15) or
		(hour is 9 and (minutes is 0 or minutes is 45) ) or
		(hour is 10 and (minutes is 0 or minutes is 40) ) or
		(hour is 11 and minutes is 25) or
		(hour is 12 and (minutes is 10 or minutes is 25) ) or
		(hour is 13 and minutes is 15) or
		(hour is 14 and (minutes is 0 or minutes is 50) ) or
		(hour is 15 and minutes is 10)
		):
			pi.write(timbre, 1)
			print(pi.read(timbre))
		else:
			pi.write(timbre, 0)
			print(pi.read(timbre))
	if (
	seconds is 0 and 
	(minutes is 0 or minutes is 15 or minutes is 30 or minutes is 45) and
	(hour is 18 or hour is 20 or hour is 22 or hour is 0 or hour is 2 or hour is 4)
	):
		print("Test")
	
