/*
* Driver que envia una senial a un proceso que se ha registrado mediante ioctl
* Se envia al activar un temporizador despues de 5 segundos
*
* Se aprende:
*	task_struct: estrucutura que representa un proceso del sistema operativo.
*	struct siginfo: almacena informacion de la senial
*	send_sig_info: enviar la senial
*	get_current(): devuelve la task actual
*/

#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/timer.h>

#define DEVICE_NAME "mydevice"	//nombre interno registrado para el dispositivo
				// tambien se usara para /dev/<nombre>

#define REGISTER_UAPP _IO('R', 'g') // comando de ioctl con la app

static struct task_struct* task = NULL; // estructura que representa un proceso
#define SIGNR 44	//numero de la senial enviadao

static struct timer_list my_timer; //estructura para el timer


// funcion del temporizado que se encarga de enviar la senial al proceso registrado (task)
void timer_callback(struct timer_list * data){
	printk("timer activado, lanzando senial\n");

	struct siginfo info; // estructura de datos de la senail
	memset(&info, 0, sizeof(info));
	info.si_signo = SIGNR;
	info.si_code = SI_QUEUE;

	// Enviar la senial
	send_sig_info(SIGNR, (struct kernel_siginfo *) &info, task);
}


static long int my_ioctl(struct file *f, unsigned int cmd, unsigned long arg){

	if (cmd == REGISTER_UAPP){
		task = get_current(); // guardamos el PID del proceso que se ha registrado
		printk("Aplicacion registrada con PID %d\n", task->pid);

		timer_setup(&my_timer, timer_callback, 0);
		mod_timer(&my_timer, jiffies + msecs_to_jiffies(5000)); //activar el timer a los 5 segundos
	}

	return 0;
}


static int my_device_open(struct inode* inode, struct file *file){
	printk("El dispositivo se ha abierto\n");
	return 0;
}

static int my_device_release(struct inode* inode, struct file *file){
	del_timer(&my_timer);
	printk("El dispositivo se ha cerrado\n");
	return 0;
}


static const struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.open = my_device_open,
	.release = my_device_release,
	.unlocked_ioctl = my_ioctl // enlazamos ioctl()
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &my_fops
};

static int __init my_init (void)
{
	printk("Modulo cargado y dispositivo registrado\n");
	return misc_register(&my_misc_device);
}

static void __exit my_exit (void)
{
	misc_deregister(&my_misc_device);
	printk("Modulo descargado y dispotivio desregistrado\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que envia una senial a un proceso registrado con ioctl");
