/*
*
* 
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h> // para kmalloc y kfree
#include <linux/string.h>


struct driver_data{
	u8 version;
	char text[64];
};

static u32 *ptr1; // puntero que apuntara a direccion de memoria dinamica

static int __init my_init(void){

	ptr1 = kmalloc(sizeof(u32), GFP_KERNEL); // reservar memoria

	if(ptr1 == NULL){
		printk("fallo en kmalloc\n");
		return -1;
	}

	printk("valor *ptr1: 0x%x\n", *ptr1);
	*ptr1 = 0xc001c0de;
	printk("valor *ptr1: 0x%x\n", *ptr1);

	kfree(ptr1); // liberar memoria

	return 0;
}

static void __exit my_exit(void){}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que usa memoria dinamica con kmalloc");

