"""
Programa: timbre.py

Autor: Luis Enrique Ramos Maldonado

Descripcion: Programa que usa el GPIO (general purpose input output)
de un Raspberry Pi para controlar digitalmente un timbre escolar. 

Sincroniza el tiempo por medio de internet; por medio de un transistor
y un relevador controla el timbre, el cual maneja corriente alterna.

Adicionalmente, se incluye un boton tipo push button para timbrar
manualmente si es necesario y un switch para cambiar entre horario de
invierno y horario de verano.


PINES USADOS:
16 => GPIO 23, switch para cambiar entre horario de invierno y de verano
cuando manda HIGH, es horario de verano, si manda LOW es horario de invierno

18 => GPIO 24, controlador de timbre
"""

import datetime
import time
import pigpio

# objeto para manipular GPIO del Pi
pi = pigpio.pi()

# GPIO 23 (switch) como input y 24 (transistor) como output
horario = 23
timbre = 24

verano = 1

pi.set_mode(horario, pigpio.INPUT) # pin 16 fisicamente en placa
pi.set_mode(timbre, pigpio.OUTPUT) # pin 18 fisicamente en placa

while True:
	time.sleep(1) # para depuracion mas facil, se puede borrar esta linea en la version final, pero no afecta el funcionamiento
	now = datetime.datetime.now()
	# los siguientes datos se convierten de tiempo a string a int para
	# hacer los if mas sencillos de usar
	dayWeek = int(datetime.datetime.today().weekday())
	hour = int(now.strftime("%H"))
	minutes = int(now.strftime("%M"))
	seconds = int(now.strftime("%S"))
	
	if pi.read(horario) == verano: # horario de verano
		if ( 
		(dayWeek <= 4) and # lunes es 0 y viernes es 4
		(seconds <= 5) and # timbre dura 6 segundos
		( (hour == 7 and (minutes == 0 or minutes == 50) ) or
		(hour == 8 and minutes == 40) or
		(hour == 9 and (minutes == 30 or minutes == 45) ) or
		(hour == 10 and minutes == 35) or
		(hour == 11 and minutes == 20) or
		(hour == 12 and (minutes == 10 or minutes == 25) ) or
		(hour == 13 and minutes == 15) or
		(hour == 14 and (minutes == 0 or minutes == 50) ) or
		(hour == 15 and minutes == 10) )
		):
			pi.write(timbre, 1)
			print(pi.read(timbre))
		else:
			pi.write(timbre, 0)
			print(pi.read(timbre))
	else: # horario de invierno
		if ( 
		(dayWeek <= 4) and 
		(seconds <= 5) and 
		( (hour == 7 and minutes == 30) or
		(hour == 8 and minutes == 15) or
		(hour == 9 and (minutes == 0 or minutes == 45) ) or
		(hour == 10 and (minutes == 0 or minutes == 40) ) or
		(hour == 11 and minutes == 25) or
		(hour == 12 and (minutes == 10 or minutes == 25) ) or
		(hour == 13 and minutes == 15) or
		(hour == 14 and (minutes == 0 or minutes == 50) ) or
		(hour == 15 and minutes == 10) )
		):
			pi.write(timbre, 1)
			print(pi.read(timbre))
		else:
			pi.write(timbre, 0)
			print(pi.read(timbre))
