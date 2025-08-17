#include <stdio.h>

int main(){

	printf("Presiona enter para abrir /dev/mydevice");
	getchar();

	FILE *f = fopen("/dev/mydevice", "r");
	if (f == NULL){
		perror("No se puede abrir el fichero\n");
		return 1;
	}

	printf("Se ha abierto el fichero\n");

	printf("Presiona una tecla para cerrar /dev/mydevice");
	getchar();
	fclose(f);

	return 0;
}
