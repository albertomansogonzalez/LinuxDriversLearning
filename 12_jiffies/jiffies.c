/*
 "jiffies" es un contador (como un reloj interno del kernel) util para trabajar con tiempos.
 Ver valor de 1 jiffie en "grep CONFIG_HZ /boot/config-$(uname -r)". En raspberry pi 4, 1 jiffie=4ms
 Hay:
	Esperas bloqueantes: msleep(), usleep_range()
	Esperas no bloqueantes: udelay(), mdelay()

 Funciones de conversion: jiffies_to_msecs(), msecs_to_jiffies()

*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>	// udelay()
#include <linux/jiffies.h>	//jiffies, jiffies_to_msecs()...

static int __init my_init(void){

	unsigned long start, end;

	start = jiffies; //Guardamos el contador actual
	printk("Tiempo inicio: jiffies=%lu, ms=%u\n", start, jiffies_to_msecs(start));

	// Espera activa (busy-wait) de 4000 microsengundos (4 ms)
	udelay(4000); //(1 jiffie)

	// Espera bloqueante (no consume CPU)
	msleep(12); //(4 jiffies)

	end = jiffies; //Guardamos el contador actual
	printk("Tiempo fin:    jiffies=%lu, ms=%u\n", end, jiffies_to_msecs(end));

	//En total se ha esperado 5 jiffies, que son 20 milisegundos
	printk("Tiempo transcurrido: jiffies=%lu, ms=%u\n", end-start, jiffies_to_msecs(end-start));

	return 0;
}


static void __exit my_exit(void){
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que realiza una espera");
