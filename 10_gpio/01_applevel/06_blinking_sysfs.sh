#!/bin/bash

# Programa que parpadea el pin 36 (GPIO16) en la rasperryp pi 4. (conectar led externo con resistencia)
# A nivel de aplicacion, utilizando interfaz con el kernel sysfs (deprecado, ahora usar libgpiod con "dev/gpiochipN")

# para ver la enumeracion de los GPIO en sysfs, consultar fichero "/sys/kernel/debug/gpio"

echo "Parpadeando GPIO16..."

# primero hay que exportar la linea gpio para poder accederla, en caso de no estar exportada
if [ ! -d /sys/class/gpio/gpio528 ]; then
	echo 528 > /sys/class/gpio/export
	sleep 0.1
fi

# configuramos el pin como output
echo out > /sys/class/gpio/gpio528/direction

while true; do
	echo 1 > /sys/class/gpio/gpio528/value # encender GPIO16
	sleep 1
	echo 0 > /sys/class/gpio/gpio528/value # apagar GPIO16
	sleep 1
done
