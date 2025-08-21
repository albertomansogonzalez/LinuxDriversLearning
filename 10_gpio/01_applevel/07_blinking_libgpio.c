/*
 Programa que parpadea el pin 36 (GPIO16) en la raspberrypi 4. (conectar led externo con resistencia)
 A nivel de aplicacion, utilizando la interfaz con el driver "/dev/gpiochip0" (moderno y oficial)

 Se utiliza la libreria de aplicacion "libgpiod" para facilitat la interaccion con el driver.
 (esta libreria por debajo ejecutara operaciones ioctl() de comunicacion con el driver)

 Compile with: `gcc 07_blinking_libgpio.c -l gpiod`
*/

#include <gpiod.h> // header de libreria libgpiod
#include <stdio.h>
#include <unistd.h>

#define CHIP "/dev/gpiochip0" // fichero de dispositivo para accede al driver.
#define LINE 16 	// GPIO16


int main(){

	/* Estructura de datos de libgpiod*/
	struct gpiod_chip *chip; // representa el chip GPIO
	struct gpiod_line *line; // representa una linea GPIO (pin)

	chip = gpiod_chip_open(CHIP);
	if(!chip){
		perror("error en gpiod_chip_open\n");
		return 1;
	}

	line = gpiod_chip_get_line(chip, LINE);
	if(!line){
		perror("error en gpiod_chip_get_line");
		return 1;
	}

	gpiod_line_request_output(line, "led", 0); //"led" es un nombre arbitrario

	printf("Parpadeando led...\n");
	for (int i = 0; i < 3; i++){
		gpiod_line_set_value(line, 1); // Encender GPIO16
		sleep(1);
		gpiod_line_set_value(line, 0); // Apagar GPIO16
		sleep(1);
	}

	printf("Cerrando recursos\n");

	// Liberar recursos
	gpiod_line_release(line);
	gpiod_chip_close(chip);

	return 0;
}
