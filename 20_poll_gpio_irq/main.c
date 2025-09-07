/*
 Compilar y ejecutar este codigo con sudo para mostrar ejemplo
 de comunicacion "polling" con el driver.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>	// libreria para llamada al sistema poll()

int main(){

	int fd;
	int test;

	fd = open("/dev/mydevice", O_RDONLY);
	if(fd < 0){
		perror("Error abriendo el dispositivo /dev\n");
		return -1;
	}

	struct pollfd my_poll;
	memset (&my_poll, 0, sizeof(my_poll));
	my_poll.fd = fd;
	my_poll.events = POLLIN;

	printf("haciendo polling, esperando al boton\n");
	poll(&my_poll, 1, -1);
	printf("Se ha pulsado el boton\n");

	close(fd); // ejecuta llamada al sistema close() (.release)

	return 0;
}
