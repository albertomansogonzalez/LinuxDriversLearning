/*
* Comunicacion entre llamadas al sistema y operaciones del driver
* a traves del fichero de dispotivio /dev/
*
* Hay que crear manualmente una entrada en /dev que tenga de nº mayor
* el nº de nuestro driver:
*	'sudo mknod /dev/mydevice c 99 0'
*
* Cuando se ejecuta una llamada al sistema de open() o close() sobre
* este fichero, se ejecutan las funciones del driver implementadas.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define DEVICE_NAME "mi_dispositivo"
#define MAYOR 99

// funcion casera que implementa open()
static int my_device_open(struct inode *inode, struct file *file){
	printk("El dispositivo se ha abierto\n"); // implementacion dummy
	return 0;
}

// funcion casera que implementa close()
static int my_device_release(struct inode *inode, struct file *file){
	printk("El dispotisitivo se ha cerrado\n"); // implementacion dummy
	return 0;
}

/* Mediante file_operations, vinculamos las llamadas al sistema
     con las funciones de nuestro driver */
static struct file_operations fops = {
	.open = my_device_open, // llamada al sistema open()
	.release = my_device_release // llamada al sistema close()
};

static int __init my_init (void)
{
	if (register_chrdev(MAYOR, DEVICE_NAME, &fops) < 0){
		printk("Hola, error registrando el dispositivo\n");
		return -1;
	}else{
		printk("Hola, exito en registro nMayor = %d\n", MAYOR);
		return 0;
	}
}

static void __exit my_exit (void)
{
	unregister_chrdev(MAYOR, DEVICE_NAME);
	printk("Modulo descargado y dispotivio desregistrado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que implementa operacion open y release");

