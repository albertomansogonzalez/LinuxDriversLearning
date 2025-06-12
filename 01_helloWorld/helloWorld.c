/*
*
* Modulo kernel mas sencillo posible.
* Imprime un mensaje en dmesg al ser cargado
* y al ser descargado (insmod, rmmod)
*/

#include <linux/kernel.h>	//funciones del kernel como printk()
#include <linux/module.h>	//para macros como MODULE_LICENSE()
#include <linux/init.h>		//para module_init() o module_exit()

static int my_init (void)
{
	printk("Welcome to my driver!\n"); //escribir en el log del kernel
	return 0;
}

static void  my_exit (void)
{
	printk("Leaving my driver!\n");
}

module_init(my_init); //elegimos nuestra funcion cuando se carga
module_exit(my_exit); //elegimos nuestra funcion cuando se descarga

MODULE_LICENSE("GPL"); //Software libre
