#!/bin/bash

# Programa que parpadea el pin 36 (GPIO16) en la rasperryp pi 4. (conectar led externo con resistencia)
# A nivel de aplicacion, utilizando libreria Wiring Pi (descargada de github) (obsoleta)

# `gpio` es una utilizad que viene con la libreria Wiring Pi
# `-g`para utilizar la numeracion de BCM

# Configurar el pin como salida
gpio -g mode 16 out

echo "Parpadeando GPIO16..."

while true; do
	gpio -g write 16 1 # encender
	sleep 1
	gpio -g write 16 0 # apagar
	sleep 1
done
