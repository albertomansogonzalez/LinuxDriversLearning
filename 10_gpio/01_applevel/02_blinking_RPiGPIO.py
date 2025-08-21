'''
Programa que parpadea el pin 36 (GPIO16) en la raspberry pi 4.
A nivel de aplicacion, utilizando libreria mas grande, RPi.GPIO.
'''

import RPi.GPIO as GPIO
import time

# Configuracion de numeracion de los pines
GPIO.setmode(GPIO.BCM) # BCM (Broadcom SOC Channel)
# Ejemplo con enumeracion de pines fisicos:
#`GPIO.setmode(GPIO.BOARD)`

# Configurar pin 16 como salida
GPIO.setup(16, GPIO.OUT)

try:
	while True:
		GPIO.output(16, GPIO.HIGH) # Encender led
		time.sleep(1)
		GPIO.output(16, GPIO.LOW) # Apagar led
		time.sleep(1)
except KeyboardInterrupt:
	GPIO.cleanup() # Liberar todos los GPIO usados
	print("Fin de blinking")
