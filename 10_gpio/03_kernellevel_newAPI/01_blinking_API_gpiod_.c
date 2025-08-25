/*
 Programa que parpadea el pin 36 (GPIO16) en la raspberrypi 4. (conectar led externo con resistencia)
 A nivel de driver como modulo kernel. Utilizando API antigua <linux/gpio.h> y funciones gpio_<>

 No es 100% moderno porque usa `gpio_to_desc()` para obtener el descriptor a partir de la numeracion GPIO BCM.
 Esto solo funciona porque realmente hay un driver oficial que ya ha registrado el GPIO. 

 Lo correcto seria utilizar `gpiod_get()` y crear en el Device Tree un nodo que represente el led.

 Al montar el modulo se enciende el led, y se apaga al desmontar.
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio/consumer.h> // API neva,(antigua: <linux/gpio>)
#include <linux/init.h>

#define GPIO_LED 16 // para GPIO16
#define OFFSET 512  //offset del GPIO del "gpiochip512"

static struct gpio_desc* led_gpio; // descriptor

static int __init led_init(void){

	int ret;

	// Obtener descriptor del GPIO
	led_gpio = gpio_to_desc(GPIO_LED + OFFSET); // esta funcion es un atajo antiguo
	if (!led_gpio){
		pr_err("Error obteniendo GPIO\n");
		return -ENODEV;
	}

	// Configurar como output, valor inicial 0
	ret = gpiod_direction_output(led_gpio, 0);
	if (ret){
		pr_err("Error al cofigurar GPIO como ouput\n");
		return ret;
	}

	pr_info("Encendiendo GPIO\n");

	// Encender led
	gpiod_set_value(led_gpio, 1);

	return 0;
}

static void __exit led_exit(void){
	pr_info ("Modulo descargado, apagando led\n");

	// Apagar led
	gpiod_set_value(led_gpio, 0); //Apagar el led
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Encender led GPIO16 con API moderna");

