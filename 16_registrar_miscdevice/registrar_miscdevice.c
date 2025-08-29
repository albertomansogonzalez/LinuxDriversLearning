/*
* Registrarmos un dispositivo de Caracter en el kernel, de forma sencilla con "miscdevice"
* Util para pruebas y aprendizaje
*
* Se crea automaticamente fichero /dev/<nombre>. (No hay que usar mknod)
*/

#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>	//para register_chrdev() y file_operations
#include <linux/uaccess.h> // para copy_to_user

#define DEVICE_NAME "mydevice"	//nombre interno registrado para el dispositivo
				// tambien se usara para /dev/<nombre>

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
	.release = my_device_release
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
	misc_deregister(&my_misc_device); // funcion para desregistrar el dispostivo
	printk("Modulo descargado y dispotivio desregistrado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que registra dispositivo de Caracter, con miscdevice");
