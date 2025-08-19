/*
* El driver tiene un buffer en el que se pueden leer
* y escribir n bytes y se va avanzando el cursor.
*
* Se aprende ´loffs_t *offs´
*
* Hay que crear manualmente una entrada en /dev que tenga de nº mayor
* el nº de nuestro driver:
*	'sudo mknod /dev/mydevice c 99 0'
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

/* Datos para registrar el dispositivo*/
#define DEVICE_NAME "mi_dispositivo"
#define MAYOR 99

/* Nuestro buffer en el driver*/
#define SIZE_BUFFER 64
static char buffer[SIZE_BUFFER];

/*
 Funcion para leer bytes del buffer, al leer avanza el curor de lectura/escritura
 @param user_buffer Puntero a zona de usuario donde el driver escribira los bytes leidos del buffer
 @param count Numero de bytes que se desean leer
 @param offs Cursor de posicion (offset)
*/
static ssize_t driver_read(struct file *File, char __user *user_buffer, size_t count, loff_t *offs){

	int to_copy_max; //nº bytes que el usuario quiere leer del buffer limitado al maximo tamanio del buffer.
	int not_copied; // la funcion ´copy_to_user()´ devuelve el nº de bytes !no copiados!

	// si en el buffer hay menos nº de bytes que el que se pide leer, se trunca hasta el maximo del buffer
	to_copy_max = (count + *offs < sizeof(buffer)) ? count : (sizeof(buffer) - *offs);

	pr_info("Ejecutando lectura, queremos leer %ld bytes, pero realmente copiando solo %d bytes. El offset es %lld\n", count, to_copy_max, *offs);

	// si el cursor ha llegado al final del buffer, no lee nada.
	if(*offs >= sizeof(buffer))
		return 0;

	// con ´copy_to_user()´ copiamos los datos al espacio de usuario
	not_copied = copy_to_user(user_buffer, &buffer[*offs], to_copy_max);

	// hay que incrementar el cursor manualmente
	*offs += (to_copy_max - not_copied);

	// devolvemos el nº de bytes realmente copiados (leidos)
	return (to_copy_max - not_copied);
}
/*
 Funcion para escribir bytes en el buffer
 @param user_buffer Puntero a zona de usuario que contiene los bytes que se quieren escribir en el buffer
 @param cout Numero de bytes que se desean escribir
 @param offs Cursor de posicion (offset)
*/
static ssize_t driver_write(struct file *File, const char __user *user_buffer, size_t count, loff_t *offs){

	int to_copy_max; //nº bytes que el usuario quiere escribir en el buffer
	int not_copied; // la funcion `copy_from_user()´ devuelve el nº de bytes !no copiados!

	// limitar los bytes escritos al tamanio del buffer
	to_copy_max = (count < SIZE_BUFFER) ? count : SIZE_BUFFER;

	pr_info("Ejecutando write(), se quiere escribir %ld bytes, pero realmente copiando %d  bytes. El offset es %lld\n", count, to_copy_max, *offs);

	if(*offs >= sizeof(buffer))
		return 0;

	not_copied = copy_from_user(&buffer[*offs], user_buffer, to_copy_max);

	// hay que incrementar el cursor manualmente
	*offs += (to_copy_max - not_copied);

	// devolvemos el nº de bytes realmente copiados (escritos)
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

