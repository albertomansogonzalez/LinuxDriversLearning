/*
 Compilar y ejecutar este codigo con sudo para mostrar ejemplo
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

	fd = open("/dev/mydevice", O_RDWR);
	if (fd < 0){
		perror("No se puede abrir el fichero\n");
		return 1;
	}

	printf("Se ha abierto el fichero\n");


	// Escribir en el driver
	const char *msg = "Hola driver\n"; // puntero en espacio de usuario
	ssize_t bytes_written;

	/*bytes_written = write(fd, msg, 12); // ejecuta llamada al driver write()
	if (bytes_written < 0){
		perror("Error en write()");
	}else{
		printf("Se han escrito correctamente %zd bytes\n", bytes_written);
	}*/


	// Leer del driver
	char buffer[64];
	ssize_t bytes_read;

	bytes_read = read(fd, buffer, sizeof(buffer)); // ejecuta llamada al driver read()
	if (bytes_read < 0){
		perror ("Error en read()");
	}else{
		printf("Se han leido %zd bytes : %.*s\n", bytes_read, (int)bytes_read, buffer);
	}


	close(fd);

	return 0;
}
