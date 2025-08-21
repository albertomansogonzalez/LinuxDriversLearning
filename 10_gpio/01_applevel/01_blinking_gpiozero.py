'''
Programa que parpadea el pin 36 (GPIO16) en la raspberry pi 4. (conectar led externo con resistencia)
A nivel de aplicacion, utilizando libreria muy sencilla, gpiozero.
'''

from gpiozero import LED
from time import sleep

led = LED(16) # configurar led GPIO16 (pin fisico 36)

try:
	while True:
		led.on() # Encender led
		sleep(1)
		led.off() # Apagar led
		sleep(1)
except KeyboardInterrupt:
	print("Fin de blinking")
