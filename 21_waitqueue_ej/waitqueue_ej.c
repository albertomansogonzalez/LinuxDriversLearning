/*
*
* Implementacion de .read() pero bloqueante, es decir, si no hay datos para leer
* el proceso de usuario queda bloqueado (estado dormido).
*
* Se aprende Waiting Queue (colas de espera)
*	DECLARE_WAIT_QUEUE_HEAD(): declarar e inicilizar de forma estatica
*	wait_event_interruptible(): para registrar en la cola
*	wake_up_interruptible(): para despertar de la cola
*
* Hay que crear manualmente una entrada en /dev que tenga de nº mayor
* el nº de nuestro driver:
*	'sudo mknod /dev/mydevice c 99 0'
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/wait.h>	//para la cola de espera


#define DEVICE_NAME "mi_dispositivo"
#define MAYOR 99

#define SIZE_BUFFER 250
static char buffer[SIZE_BUFFER];
static unsigned int usedBytes = 0;

/* Declaracion e inicializacion estatica de la Cola de Espera*/
DECLARE_WAIT_QUEUE_HEAD(my_queue);

/*
 Funcion para leer bytes del buffer
 @param user_buffer Puntero a zona de usuario donde el driver escribira los bytes leidos del buffer
 @param count Numero de bytes que se desean leer
 @param offs Cursor de posicion (No lo usamos)
*/
static ssize_t driver_read(struct file *File, char __user *user_buffer, size_t count, loff_t *offs){
	printk("Ejecutando lectura\n");

	/* Para que sea bloqueante, utilizamos WAITQUEUE (cola de espera)*/
	if(wait_event_interruptible(my_queue, usedBytes != 0)){
		return -ERESTARTSYS;
	}

	int to_copy_max;
	int not_copied;

	to_copy_max = (count < usedBytes) ? count : usedBytes;

	not_copied = copy_to_user(user_buffer, buffer, to_copy_max);

	return (to_copy_max - not_copied);
}
/*
 Funcion para escribir bytes en el buffer
 @param user_buffer Puntero a zona de usuario que contiene los bytes que se quieren escribir en el buffer
 @param cout Numero de bytes que se desean escribir
 @param offs Cursor de posicion (No lo usamos)
*/
static ssize_t driver_write(struct file *File, const char __user *user_buffer, size_t count, loff_t *offs){
	printk("Ejecutando escritura\n");

	/* Desperamos los procesos bloqueados (cola de espera) de read() */
	if (count > 0) wake_up_interruptible(&my_queue);

	int to_copy_max;
	int not_copied;

	to_copy_max = (count < SIZE_BUFFER) ? count : SIZE_BUFFER;

	not_copied = copy_from_user(buffer, user_buffer, to_copy_max);
	usedBytes = (to_copy_max - not_copied);

	return (to_copy_max - not_copied);
}

static struct file_operations fops = {
	.read = driver_read,
	.write = driver_write
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
MODULE_DESCRIPTION("Modulo que implementa operacion read y write");

