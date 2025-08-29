/*
 Modulo que captura una interrupcion en GPIO16 de la raspberry pi 4. (poner un boton pulsador)
 Con debounce por software
 Se aprende:
	gpiod_to_irq()
	request_irq()
	free_irq()
	time_before()

*/

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/gpio/consumer.h>
#include <linux/jiffies.h>

#define GPIO_BOTON 16 // para GPIO16
#define OFFSET 512 // ofset del GPIO del "gpiochip512"

static struct gpio_desc* btn_gpio; // descriptor

static int irq_number; //identificador numerico de la IRQ

static unsigned long last_jiffie = 0;

// ISR Interrupt Service Routine
static irqreturn_t mi_isr_boton(int irq, void* dev_id){

	unsigned long now = jiffies;

	if(time_before(now, last_jiffie + msecs_to_jiffies(100))){
		return IRQ_HANDLED;
	}

	last_jiffie = now;

	pr_info("Interrupcion capturada\n");

	return IRQ_HANDLED;
}

static int __init my_init(void){

	printk("Modulo cargado\n");

	int ret;

	btn_gpio = gpio_to_desc(GPIO_BOTON + OFFSET); // esta funcion es un atajo antiguo
	if (!btn_gpio){
		pr_err("Error obteniendo GPIO\n");
		return -ENODEV;
	}

	// Configurar GPIO como entrada (boton)
	gpiod_direction_input(btn_gpio);

	//Obtener numero de IRQ
	irq_number = gpiod_to_irq(btn_gpio);
	if (irq_number < 0){
		pr_err("Error obteniendo IRQ\n");
		gpiod_put(btn_gpio);
		return irq_number;
	}

	// Registar la Interrupcion con la ISR
	ret = request_irq(irq_number, mi_isr_boton, IRQF_TRIGGER_RISING, "mi_boton_irq", NULL);

	return 0;
}


static void __exit my_exit(void){

	free_irq(irq_number, NULL); // Liberar la interrupcion
	gpiod_put(btn_gpio);

	printk("Modulo descargado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("");
