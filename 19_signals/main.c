/*
 Compilar y ejecutar este codigo con `sudo` para ver la
 recepcion de seniales enviadas por el driver.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

#define SIGTX 44

#define REGISTER_UAPP _IO('R', 'g')


// funcion handler de la senial
void signalhandler(int sig){
	printf("Senial recivida del driver!\n");
}


int main(){

	int fd = open("/dev/mydevice", O_RDWR);

	// enlazamos la senial con el handler
	signal(SIGTX, signalhandler);

	printf("PID = %d\n", getpid());

	if(fd < 0){
		perror("Error abriendo el /dev/mydevice\n");
		return fd;
	}


	/* Envio comando para registrar la APP */
	ioctl(fd, REGISTER_UAPP, NULL); // asi el driver sabra nuestro PID



	// Esperar a la senial
	while(1) sleep(1);

	close(fd);

	return 0;
}
