/*
 Programa que parpadea el pin 36 (GPIO16) en la raspberrypi 5. (conectar led externo con resistencia)
 A nivel de aplicacion, realizando manualmente las llamadas ioctl sobre el driver "/dev/gpiochip0" (moderno y oficial)
 sin utilizar ninguna libreria de ayuda.
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>

#define GPIO_LINE 16	//GPIO16

int main(){

	int fd_chip;
	struct gpiohandle_request req; //estructura para hacer la peticion ioctl
	struct gpiohandle_data data; // estructura para recibir la informacion

	//Abrir el dispositivo
	fd_chip = open("/dev/gpiochip0", O_RDWR);
	if (fd_chip < 0){
		perror("Error al abrir gpiochip0\n");
		return EXIT_FAILURE;
	}

	// Configurar el GPIO como salida
	memset(&req, 0, sizeof(req));
	req.lineoffsets[0] = GPIO_LINE;
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	req.lines = 1; // nº de lineas que se solicitan en la peticion.
	data.values[0] = 0;
	memcpy(&req.default_values, &data.values, sizeof(req.default_values));

	if (ioctl(fd_chip, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0) {
		perror("Error al configurar la linea GPIO como salida");
		return EXIT_FAILURE;
	}

	sleep(1); // para dar tiempo al kernel a configurar

	for(int i = 0; i < 5; i++){
		// Activar el GPIO
		printf("Activando GPIO%d\n", GPIO_LINE);
		data.values[0] = 1; // Encender
		if (ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0){
			perror ("Error al activar GPIO16");
			close(req.fd);
			close(fd_chip);
			return EXIT_FAILURE;
		}

		sleep(1);

		printf("Desactivando GPIO%d\n", GPIO_LINE);
		data.values[0] = 0; // Apagar
		if (ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0){
			perror("Error al desactivar GPIO\n");
			close(req.fd);
			close(fd_chip);
			return EXIT_FAILURE;
		}

		sleep(1);
	}


	//Liberar recursos
	close (req.fd);
	close (fd_chip);
	printf("Control del GPIO finalizado\n");

	return 0;
}
