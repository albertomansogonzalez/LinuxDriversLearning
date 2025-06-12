/*
*
* modulo que acepta un parametro al ser cargado:
*	"sudo insmod parametros.ko mi_param=4"
* no son obligatorios: "sudo insmod parametros.ko"
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

/* parametro*/
static int mi_param = 0; //valor por defecto por si no se especifica

module_param(mi_param, int, 0444); //se esta creando un fichero en /sys/module/<modulo>/parameters/mi_param
				   // con 0444: solo permisos de lectura
MODULE_PARM_DESC(mi_param, "Un parametro de ejemplo"); //descripcion visible con modinfo



static int __init my_init (void)
{
	printk("Modulo cargado con parametro = %d\n", mi_param);
	return 0;
}

static void __exit my_exit (void)
{
	printk("Modulo descargado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que acepta un parametro");
