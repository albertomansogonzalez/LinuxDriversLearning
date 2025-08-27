/*
* Los registros de GPIO esta mapeados en el bus de direcciones (MMIO: Memory Mapped Input/Output), es decir,
* se puede acceder a estos registros como si se trataran de zonas de memoria de la RAM normales.
* Estos registros GPIO tienen un tamanio de 32 bits (aunque la raspberry sea de 64 bits, no importa)
*
* Se aprende:
* 	ioremap(): mapear direcciones fisica, a puntero virtual para poder acceder
*	iounmap(): para desmapear
*	__iomem: anotacion para indicar que no es una direccion de memoria de RAM normal.
*		no se puede desreferenciar asi: `*puntero = 4`.
		Hay que acceder mediante funciones: readl(), writel()...
*	readl(): lee 32 bits en direcciones mapeadas MMIO por ioremap()
*	writel(): escribe 32 bits en direcciones mapeadas MMIO por ioremap()
*
* Activa GPIO16 en la raspberry pi 4 al cargar el modulo, y lo desactiva al descargar.
*
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/io.h>

// direccion donde se mapea GPIO
// aunque en el Device Tree ponga 7e200000, realmente luego se accede mediante el bus del SoC
// que a su vez mapea a la direccion 0xFE200000
#define GPIO_BASE 0xFE200000
#define GPIO_SIZE 0xA0 // mapeamos un total de 160 bytes (registros GPIO)

#define REG_GPFSEL1 0x04 //Offset del registro GPFSEL1 (GPIO Function Select 1)
#define REG_GPSET0 0x1C //OFfset del registro GPSET0 (GPIO Pin Output Set 0)
#define REG_GPCLR0 0x28 //Offset del registro GPCLR0 (GPIO Pin Output Clear 0)

static void __iomem *gpio_base; // puntero que tendremos mapeao con GPIO

static int __init my_init (void)
{
	u32 val; // 32 bits sin signo. Para manipular los registros

	gpio_base = ioremap(GPIO_BASE, GPIO_SIZE);
	if(!gpio_base){
		printk("Error al mapear GPIO\n");
		return -ENOMEM;
	}

	// pone '001' -> modo output
	val = readl(gpio_base + REG_GPFSEL1);
	val &= ~(0x7 << 18); //primero limpia los bits
	val |= (0x1 << 18); // luego setea el ultimo
	writel(val, gpio_base + REG_GPFSEL1);

	writel((1 << 16), gpio_base + REG_GPSET0); // activa el GPIO16

	return 0;
}

static void __exit my_exit (void)
{
	writel((1 << 16), gpio_base + REG_GPCLR0); //desactiva el GPIO16

	if(gpio_base){
		iounmap(gpio_base); //desmapear
	}
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que activa GPIO16 mediante MMIO ioremap");

