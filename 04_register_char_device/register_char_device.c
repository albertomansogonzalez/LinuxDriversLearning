/*
* Registrarmos un dispositivo de Caracter en el kernel con Numero Mayor Fijo manualmente
* Ademas de la manera antigua, usando "register_chrdev()"
*
* (No se crea automaticamente el fichero /dev/)
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>	//para register_chrdev() y file_operations

#define DEVICE_NAME "mi_dispositivo"	//nombre interno registrado para el dispositivo
					// independiente del fichero /dev/<nombre>

#define MAYOR 99	//fijamos un numero mayor de forma manual (debe estar disponible)

static struct file_operations fops = {}; //se rellenara mas adelante

static int __init my_init (void)
{
	if (register_chrdev(MAYOR, DEVICE_NAME, &fops) < 0){ // funcion para registrar el dispositivo
		printk("Hola, error registrando el dispositivo\n");
		return -1;
	}else{
		printk("Hola, exito en registro nMayor = %d\n", MAYOR);
		return 0;
	}
}

static void __exit my_exit (void)
{
	unregister_chrdev(MAYOR, DEVICE_NAME); // funcion para desregistrar el dispostivo
	printk("Modulo descargado y dispotivio desregistrado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que registra dispositivo de Caracter, forma manual y antigua");
