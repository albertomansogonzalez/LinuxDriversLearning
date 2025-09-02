/*
 Compilar y ejecutar este codigo con `sudo` para ver la
 comunicacion ioctl con el driver.

 Se envian 3 comandos personalizados "ioctl()".
	0 argumentos: RD_VAL: ejecutar programa sin comandos -> devuelve valor leido por el driver
	1 argumento:  WR_VAL: ejecutar con 1 argumento -> setea el valor en el driver
	2 argumentos: GREET: <n> <name> repite n veces por dmesg una cadena de texto <name>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

// Inlcluimos fichero de cabecera comun con el driver
#include "my_ioctl.h"

int main(int argc, char** argv){

	int answer;
	int status; //para almacenar el valor de retorno de ioctl
	struct mystruct s;

	int fd = open("/dev/mydevice", O_RDWR);

	if(fd < 0){
		perror("Error abriendo el /dev/mydevice\n");
		return fd;
	}

	switch(argc){
		case 1:
			status = ioctl(fd, RD_VAL, &answer);
			if(status) perror("error en ioctl\n");
			printf("the answer is %d\n", answer);
			break;
		case 2:
			answer = atoi(argv[1]);
			status = ioctl(fd, WR_VAL, &answer);
			if(status) perror("error en ioctl\n");
			break;
		case 3:
			s.repeat = atoi(argv[1]);
			strcpy(s.name, argv[2]);
			status = ioctl(fd, GREET, &s);
			if (status) perror("error en ioctl\n");
			break;
		default:
			// se envia un comando que no existe, el driver dara error
			status = ioctl(fd, 123, s);
			if(status) perror ("error en ioctl\n");
			break;
	}

	close(fd);

	return 0;
}
