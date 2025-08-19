/*
 Compilar y ejecutar este codigo para mostrar ejemplo
 de lectura y escritura en el driver.

 Este programa sera un proceso ejecutado en espacio de usuario
 que se comunica con el driver (en espacio del kernel) mediante
 llamadas al sistema.

*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main(){
	int fd; //descritor de fichero
	char c;

	fd = open("/dev/mydevice", O_RDWR);
	if (fd < 0){
		perror("No se puede abrir el fichero\n");
		return 1;
	}

	while(read(fd, &c, 1))
		putchar(c);

	close(fd);

	return 0;
}
