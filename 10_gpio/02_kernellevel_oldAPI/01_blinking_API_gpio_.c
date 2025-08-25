/*
 Programa que parpadea el pin 36 (GPIO16) en la raspberrypi 4. (conectar led externo con resistencia)
 A nivel de driver como modulo kernel. Utilizando API antigua <linux/gpio.h> y funciones gpio_

 Al montar el modulo se enciende el led, y se apaga al desmontar.
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h> // API decretated (nueva: <linux/gpio/consumer.h)
#include <linux/init.h>

// en la API antigua, la linea GPIO se identifica por un numero magico.
#define GPIO_LINE 528	// GPIO16, ver "cat /sys/kernel/debug/gpio"

static int __init led_init(void){

	int ret;

	pr_info("Modulo cargado\n");

	// Solicitar  control del GPIO16
	ret = gpio_request(GPIO_LINE, "LED_GPIO16");
	if (ret){
		pr_err ("Error solicitando el GPIO\n");
		return ret;
	}

	// Configurarlo como salida y valor a 1
	ret = gpio_direction_output(GPIO_LINE, 1); //encender el led
	if(ret){
		pr_err("Error configurando GPIO como salida\n");
		gpio_free(GPIO_LINE);
		return ret;
	}

	pr_info("Led encendido\n");
	return 0;
}

static void __exit led_exit(void){
	pr_info ("Modulo descargado\n");

	gpio_set_value(GPIO_LINE, 0); //Apagar el led
	gpio_free(GPIO_LINE);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Encender led GPIO16 con API antigua");

