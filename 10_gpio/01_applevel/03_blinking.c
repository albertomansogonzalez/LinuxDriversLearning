/*
 Programa que parpadea el pin 36 (GPIO16) en la raspberry pi 4. (conectar led externo con resistencia)
 A nivel de aplicacion, utilizando libreria Wiring Pi(descargada de github)(obsoleta)
*/

#include <wiringPi.h> // header de la biblioteca
#include <stdio.h>
#include <unistd.h>

int main(){
	/* wiringPiSetup() -> usa la numeracion propia de wiringPi
	   wiringPiSetupGpio() -> la numeracion BCM
	   wiringPiSetupPhys() -> la numeracion fisica de los pines */
	if(wiringPiSetupGpio() == -1){
		printf("Error al inicializar wiringpi\n");
		return 1;
	}

	int pin = 16; //GPIO16 (pin fisico 36)
	pinMode(pin, OUTPUT); // configurar el pin

	while(1){
		digitalWrite(pin, HIGH); //encender
		sleep(1);
		digitalWrite(pin, LOW); //apagar
		sleep(1);
	}

	return 0;
}
