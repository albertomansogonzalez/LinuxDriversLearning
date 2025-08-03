/*
* Registrarmos un dispositivo de Caracter en el kernel con Numero Mayor Fijo manual (uno disponible)
* Ademas de la manera moderna, usando "register_chrdev_region() junto con `cdev`"
*
* (No se crea automaticamente el fichero /dev/)
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>	//para register_chrdev() y file_operations
#include <linux/cdev.h> // para estructura cdev

#define DEVICE_NAME "mi_dispositivo"	//nombre interno registrado para el dispositivo
					// independiente del fichero /dev/<nombre>
static dev_t dev_num; //identifica a un dispotivio (compuesto por: numero mayor y numero menor)
#define MAYOR 99  //numero mayor fijo (debe estar disponible)
#define DEVICE_COUNT 1 //numero de dispositivos  que vamos a registrar (cantidad de numeros menores)

static struct cdev *my_cdev; // estructura que conecta el numero de dispotivo dev_t con las file_operations
static struct file_operations fops = {
	.owner = THIS_MODULE,
}; //se rellenara mas adelante

static int __init my_init (void)
{
	int menor_inicial = 0;
	dev_num = MKDEV(MAYOR, menor_inicial); //macro para setear la estructura dev_t
	// 1. Registrar con numero mayor `MAYOR` y una cantidad de `DEVICE_COUNT` dispositivos (numeros menores)
	// register_chrdev_region(): si sabemos el device mayor number
	// alloc_chrdev_region(): si queremos allocate a dynamically-allocated major number
	if(register_chrdev_region(dev_num, DEVICE_COUNT, DEVICE_NAME) < 0){
		printk("Hola, error registrando el dispositivo\n");
		return -1;
	}else{
		printk("Hola, exito en registro nMayor = %d\n", MAYOR);
	}

	// 2. Asignar cdev dinamicamente
	my_cdev = cdev_alloc();
	if (!my_cdev){
		unregister_chrdev_region(dev_num, DEVICE_COUNT);
		printk("Fallo al asignar cdev\n");
		return -ENOMEM;
	}
	my_cdev->ops = &fops; // enlazamos la estrutura de file_operations
	//tambien se puede hacer con `cdev_init()` si tenemos el cdev embebido en una estructura nuestra
	my_cdev->owner = THIS_MODULE;

	// 3. Registrar el cdev
	if (cdev_add(my_cdev, dev_num, DEVICE_COUNT) < 0){
		printk("Error registrando cdev\n");
		unregister_chrdev_region(dev_num, DEVICE_COUNT);
		return -1;
	}

	printk("modulo cargado y registrado correctamente\n");
	return 0;
}

static void __exit my_exit (void)
{
	if(my_cdev){ //comprobacion segura por si no existe
		cdev_del(my_cdev); //borrar la cdev
	}else{
		printk("no existe cdev\n");
	}

	if(dev_num){ // chequeo de seguridad
		unregister_chrdev_region(dev_num, DEVICE_COUNT); // des-registrar el dispostivo
		printk("Modulo descargado y dispotivio desregistrado\n");
	}
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que registra dispositivo de Caracter, numero manual pero manera moderna con cdev");
