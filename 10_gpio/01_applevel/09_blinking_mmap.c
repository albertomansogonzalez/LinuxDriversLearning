/*
 Programa que parpadea el pin 36 (GPIO16) en la raspberry pi 4. (conectar led externo con resistencia)
 A nivel de aplicacion, utilizando fichero de dispositivo /dev/gpiomem que proporciona driver para acceso directo a memoria.

 Con la llamada al sistema `mmap()` se mapean las direcciones fisicas de los registros GPIO a zonas de memoria de nuestro proceso.
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h> // para `mmap()`
#include <unistd.h>
#include <stdint.h>

#define BLOCK_SIZE 4096

int main(){
	int fd;
	void *gpio_map; // direccion en usuario, del area mapeada
	volatile unsigned *gpio; // volatile porque el registro puede cambiar de forma externa

	if((fd = open("/dev/gpiomem", O_RDWR|O_SYNC)) < 0){
		perror("Error en open()\n");
		return -1;
	}

	gpio_map = mmap(NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);

	if(gpio_map == MAP_FAILED){
		perror("Error en mmap()");
		return -1;
	}

	gpio = (volatile unsigned *)gpio_map;

	/*
	 gpio[1]: registro GPFSEL1 (GPIO Function Select 1): configurar pines input/ouput
		  bit 20:18 para GPIO16

	 gpio[7]: registro GPSET0 (GPIO Pin Output Set 0): para setear el pin GPIO

	 gpio[10]: registro GPCLR0 (GPIO Pin Output Clear 0): para desactivar el pin GPIO
	*/

	// pone a '001' -> modo output
	gpio[1] &= ~(0x7 << 18); //primero limpia los bits
	gpio[1] |= (0x1 << 18); // luego setea el ultimo

	gpio[7] = (1 << 16); // activa el GPIO16

	sleep(1);

	gpio[10] = (1 << 16); // desactiva el GPIO16

	return 0;

}
