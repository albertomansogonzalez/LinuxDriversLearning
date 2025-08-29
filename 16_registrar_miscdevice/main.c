/*
 Compilar y ejecutar este codigo para mostrar ejemplo
 de comunicacion con el driver.

 Este programa sera un proceso ejecutado en espacio de usuario
 que se comunica con el driver (en espacio del kernel) mediante
 llamadas al sistema.

*/
#include <stdio.h>

int main(){

	printf("Presiona enter para abrir /dev/mydevice");
	getchar();

	FILE *f = fopen("/dev/mydevice", "r"); // ejecuta llamada al sistema open()
	if (f == NULL){
		perror("No se puede abrir el fichero\n");
		return 1;
	}

	printf("Se ha abierto el fichero\n");

	printf("Presiona una tecla para cerrar /dev/mydevice");
	getchar();
	fclose(f); // ejecuta llamada al sistema close() (.release)

	return 0;
}
