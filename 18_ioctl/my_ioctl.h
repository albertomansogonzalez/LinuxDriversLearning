/*
 Este fichero de cabeera contiene la definicion de los identificadores de los comandos
 para ser enviados con "ioctl()" entre una aplicacion de usuario y un driver del kernel.

 Incluir este fichero en ambos sitios; en el programa de usuario, y en el modulo kernel.
*/
#ifndef MY_IOCTL_H_
#define MY_IOCTL_H_


// Tipo de dato para usar como argumento en uno de los comandos
struct mystruct{
	int repeat;
	char name[64];
};


// Se utilizan las macros _IOW, _IOR, _IOWR, _IO, para generar los identificadores
#define WR_VAL _IOW('a', 'b', int *)
#define RD_VAL _IOR('a', 'c', int *)
#define GREET _IOW('a', 'd', struct mystruct *)

#endif
