/*
*
* con printk podemos definir la priodiad del mensaje
* mas moderno: pr_info(), pr_debug() ...
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init my_init (void)
{
	/* Escribir mensajes estableciendo su prioridad */
	printk(KERN_EMERG "EMERG\n");		//prioridad 0
	printk(KERN_ALERT "ALERT\n");		//prioridad 1
	printk(KERN_CRIT "CRIT\n");		//prioridad 2
	printk(KERN_ERR "ERR\n");		//prioridad 3
	printk(KERN_WARNING "WARNING\n");	//prioridad 4, por defecto
	printk(KERN_NOTICE "NOTICE\n");		//prioridad 5
	printk(KERN_INFO "INFO\n");		//prioridad 6
	printk(KERN_DEBUG "DEBUG\n");		//priporidad 7
	return 0;
}

static void __exit my_exit (void)
{
	/* Mas moderno se recomienda:*/
	pr_emerg("EMERG\n");
	pr_alert("ALERT\n");
	pr_crit("CRIT\n");
	pr_err("ERR\n");
	pr_warn("WARNING\n");
	pr_notice("NOTICE\n");
	pr_info("INFO\n");
	pr_debug("DEBUG\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo hello world");
