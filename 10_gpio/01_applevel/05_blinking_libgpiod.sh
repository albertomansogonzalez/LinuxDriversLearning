#!/bin/bash

# Programa que parpadea el pin 36 (GPIO16) en la rasperryp pi 4. (conectar led externo con resistencia)
# A nivel de aplicacion, utilizando libreria libgpiod (moderna y oficial)

# La libreria moderna libgpiod invoca al driver GPIO mediante la interfaz "/dev/gpiochipN"

# `gpioset` es una utilizad que viene con la libreria libgpiod

# El GPIO se divide en varios controladores (driver) como un chip numerado: gpiochipN
# Cada chip agrupa varias lineas GPIO (pines).
# En la raspberry pi 4, el gpiochip0 controla todos los pines expuestos en el zocalo

# con `gpiodinfo gpiochio0` se visualizan los pines GPIO

echo "Parpadeando GPIO16..."

while true; do
	gpioset gpiochip0 16=1 # encender GPIO16
	sleep 1
	gpioset gpiochip0 16=0 # apagar GPIO16
	sleep 1
done
