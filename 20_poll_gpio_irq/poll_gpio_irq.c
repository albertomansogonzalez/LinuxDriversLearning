/*
* Ejemplo de implementacion de ".poll()" mediante GPIO y IRQ en GPIO16
*
* Se aprende:
*	Funciones:
*	"init_waitqueue_head()": para iniciar la cola de espera
*	"poll_wait(file, &waitqueue, wait()" : para registrar en la cola de espera
*	"wake_up()": para levantar de la cola de espera
*
*	Estructuras:
*	wait_queue_head_t: tipo de dato, para la cola de espera
*/

#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/poll.h> //incluimos libreria
#include <linux/wait.h> // header para las Colas de Espera


#define DEVICE_NAME "mydevice"	//nombre interno registrado para el dispositivo
				// tambien se usara para /dev/<nombre>

#define GPIO_LINE 528 // GPIO16, ver "cat /sys/kernel/debug/gpio"

unsigned int irq_number;
static int irq_ready = 0; // booleano para asegurarse que se ha detectado la interrupcion
static wait_queue_head_t waitqueue; // Cola de espera, para bloquear el proceso

// handler de la IRQ
static irqreturn_t gpio_irq_poll_handler(int irq, void *dev_id){
	printk("gpio_irq_poll: Interrupt was triggered and ISR was called\n");
	irq_ready = 1;
	wake_up(&waitqueue);
	return IRQ_HANDLED;
}

static unsigned int my_poll(struct file *file, poll_table *wait){
	poll_wait(file, &waitqueue, wait); // no bloquea!! solo registra en la cola
	if(irq_ready == 1){	// si ha habido la interrupcio, .poll() devolvera true
		irq_ready = 0;
		return POLLIN;
	}
	return 0;
}

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
	.release = my_device_release,
	.poll = my_poll			// linkamos .poll()
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &my_fops
};

static int __init my_init (void)
{
	printk("Modulo cargado y dispositivo registrado\n");

	init_waitqueue_head(&waitqueue);

	gpio_request(GPIO_LINE, "rpi-gpio-17");

	gpio_direction_input(GPIO_LINE);

	// gpio_set_debounce(GPIO_LINE, 300);

	irq_number = gpio_to_irq(GPIO_LINE);

	request_irq(irq_number, gpio_irq_poll_handler, IRQF_TRIGGER_RISING, "my_gpio_irq_poll", NULL);
	return misc_register(&my_misc_device);
}

static void __exit my_exit (void)
{
	free_irq(irq_number, NULL);
	gpio_free(GPIO_LINE);

	misc_deregister(&my_misc_device); // funcion para desregistrar el dispostivo
	printk("Modulo descargado y dispotivio desregistrado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que implementa poll() con IRQ de GPIO");
