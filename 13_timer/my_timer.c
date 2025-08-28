/*
 Creacion de un Timer en el kernel de Linux que ejecuta un Callback cada 2 segundos.
 Se aprende:
	struct timer_list: estructura que representa un Timer
	timer_setup(): inicializar el timer y enlazar el callback
	mod_timer(): programar tiempo de disparo
	del_timer(): eliminar el timer

*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

// Timer
static struct timer_list mi_timer;

static unsigned long start; // para guardar los jiffies iniciales

// Funcion Callback que disparara el Timer
void mi_callback(struct timer_list *t){
	printk("Timer activado a los %u ms\n", jiffies_to_msecs(jiffies-start));

	// Rearme del timer, para Termporazor Periodico
	mod_timer(&mi_timer, jiffies + msecs_to_jiffies(2000));
}


static int __init my_init(void){

	printk("Modulo cargado\n");
	start = jiffies;

	// Inicializar el Timer y enlazar el Callback
	timer_setup(&mi_timer, mi_callback, 0);

	//Programar el Timer a los 2000 ms (2 segundos)
	mod_timer(&mi_timer, jiffies + msecs_to_jiffies(2000));

	return 0;
}


static void __exit my_exit(void){

	del_timer(&mi_timer); //eliminar el timer
	printk("Modulo descargado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo con Temporizador periodico cada 2 segundos");
