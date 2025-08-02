/*
* Registrarmos un dispositivo de Caracter en el kernel con Numero Mayor Fijo automatico (uno disponible)
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

static int n_mayor;

static struct file_operations fops = {}; //se rellenara mas adelante

static int __init my_init (void)
{
	n_mayor = register_chrdev(0, DEVICE_NAME, &fops); // 0: para que el kernel busque el nº automaticamente
	if (n_mayor < 0){
		printk("Hola, error registrando el dispositivo\n");
		return -1;
	}else{
		printk("Hola, exito en registro nMayor = %d\n", n_mayor);
		return 0;
	}
}

static void __exit my_exit (void)
{
	unregister_chrdev(n_mayor, DEVICE_NAME); // funcion para desregistrar el dispostivo
	printk("Modulo descargado y dispotivio desregistrado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que registra dispositivo de Caracter, numero automatico pero manera antigua");
