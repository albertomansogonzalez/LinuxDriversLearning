/*
* Driver que implementa comandos "ioctl()"
*
* Enlazar en file_operations con ".unlocked_ioctl".
*
* Incluimos el fichero comun con la app: "my_ioctl.h"
*
* Se implementan 3 comandos que llamamos: WR_VAL, RD_VAL, GREET
*/

#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

/* Incluimos el fichero de cabecera comun con la aplicacion de usuario*/
#include "my_ioctl.h"	//este fichero define los comandos y los tipos de datos

#define DEVICE_NAME "mydevice"

static int answer = 42; //variable del driver para probar ioctl
/* Implementacion dummy de ioctl
	cmd: valor id del comando
	arg: direccion de memoria en espacio de usuario (es como un puntero realmente)
*/
static long int my_ioctl(struct file *f, unsigned int cmd, unsigned long arg){

	int status;
	struct mystruct s;

	pr_info("ioctl called with cmd: 0x%x and args: %p\n", cmd, (void*)arg);

	switch(cmd){
		case WR_VAL:
			status = copy_from_user(&answer, (int *) arg, sizeof(int));
			if (status){
				pr_err("Error en comando WR_VAL\n");
				return status;
			}
			break;
		case RD_VAL:
			status = copy_to_user((int *) arg, &answer, sizeof(int));
			if(status){
				pr_err("Error en comando RD_VAL\n");
				return status;
			}
			break;
		case GREET:
			status = copy_from_user(&s, (struct mystruct *)arg, sizeof(struct mystruct));
			if(status){
				pr_err("Error en comando GREET\n");
				return status;
			}

			for(int i = 0; i < s.repeat; i++)
				pr_info("Hola, %s\n", s.name);
			break;
		default:
			return -EOPNOTSUPP; // comando no soportado
	}
	return 0;
}


static int my_device_open(struct inode* inode, struct file *file){
	printk("El dispositivo se ha abierto\n");
	return 0;
}

static int my_device_release(struct inode* inode, struct file *file){
	printk("El dispositivo se ha cerrado\n");
	return 0;
}


static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_device_open,
	.release = my_device_release,
	.unlocked_ioctl = my_ioctl // enlazamos ioctl()
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &my_fops
};

static int __init my_init (void)
{
	printk("Modulo cargado y dispositivo registrado\n");
	return misc_register(&my_misc_device);
}

static void __exit my_exit (void)
{
	misc_deregister(&my_misc_device);
	printk("Modulo descargado y dispotivio desregistrado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que implementa ioctl con 3 comandos");
