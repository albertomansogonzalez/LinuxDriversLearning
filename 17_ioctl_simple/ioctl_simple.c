/*
* Driver que implementa comandos "ioctl()"
*
* Enlazar en file_operations con ".unlocked_ioctl".
* Como implementacion, solamente imprime el nº del comando y
* el valor del argumento (direccion de memoria en espacio de usuario)
*/

#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydevice"	//nombre interno registrado para el dispositivo
				// tambien se usara para /dev/<nombre>

/* Implementacion dummy de ioctl 
	cmd: valor id del comando
	arg: direccion de memoria en espacio de usuario (es como un puntero realmente)
*/
static long int my_ioctl(struct file *f, unsigned int cmd, unsigned long arg){

	pr_info("ioctl called with cmd: %d and args: %p\n", cmd, (void*)arg);

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
MODULE_DESCRIPTION("Modulo que implementa tontamente ioctl");
