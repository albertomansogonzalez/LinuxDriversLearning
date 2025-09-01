/*
 Compilar y ejecutar este codigo con `sudo` para ver la
 comunicacion ioctl con el driver.

 Se envian 3 comandos personalizados "ioctl()"
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define ID_COMANDO_1 1
#define ID_COMANDO_2 1234
#define ID_COMANDO_3 9843
int main(){

	int fd = open("/dev/mydevice", O_RDWR);

	if(fd < 0){
		perror("Error abriendo el /dev/mydevice\n");
		return fd;
	}

	char c = 123; // argumento del comando 2
	int i = 5678; //argmento del comando 3
	int status; // para almacenar el valor de retorno de ioctl()

	/* Envio comando ID_COMANDO_1*/
	status = ioctl(fd, ID_COMANDO_1, NULL); //comando sin argumentos
	printf("ioctl returned: %d\n", status);

	/* Envio comando ID_COMANDO_2 */
	status = ioctl(fd, ID_COMANDO_2, &c); // con argumento de entrada
	printf("ioctl returned: %d\n", status);

	/* Envio comando ID_COMANDO_3 */
	status = ioctl(fd, ID_COMANDO_3, &i); // con argmumento de entrada
	printf("ioctl returned: %d\n", status);

	close(fd);

	return 0;
}
