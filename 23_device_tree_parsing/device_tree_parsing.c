/*
* Modulo kernel que lee un nodo del Device Tree.
* Compilar .dts a .dtbo con: "dtc -@ -I dts -O dtb my_overlay.dtbo myoverlay.dts"
* Incluir el DT Overlay con: "sudo dtoverlay <fichero.dtbo>
*
*
* Lee las propiedades: "label = Test" y "my_value = <12>"
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h> // para leer el DT
#include <linux/property.h> // para leer el DT
#include <linux/platform_device.h> // para leer el DT
#include <linux/of_device.h>	//para leer el DT

static int dt_probe(struct platform_device *pdev);
static void dt_remove(struct platform_device *pdev);


static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "brightlight,mydev", // para enlazar con los nodos del DT que tengan este compatible
	}, { /* sentinel */}
};
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct platform_driver my_driver ={
	.probe = dt_probe,
	.remove = dt_remove,
	.driver = {
		.name = "my_device_driver",
		.of_match_table = my_driver_ids,
		},
};

static int dt_probe(struct platform_device *pdev){
	struct device *dev = &pdev->dev;
	const char *label;
	int my_value, ret;

	// Comprobar si el dispositivo tiene las propiedades
	if(!device_property_present(dev, "label")){
		printk("error dt_probe, propiedad label\n");
		return -1;
	}
	if(!device_property_present(dev, "my_value")){
		printk("error dt_probe, propiedad my_value\n");
		return -1;
	}

	// Leer las propiedades
	ret = device_property_read_string(dev, "label", &label);
	if(ret){
		printk("error dt_probe, leer label\n");
		return -1;
	}
	printk("dt_probe - label: %s\n", label);

	ret = device_property_read_u32(dev, "my_value", &my_value);
	if(ret){
		printk("error dt_probe, leer my_value\n");
		return -1;
	}
	printk("dt_probe - label: %d\n", my_value);

	return 0;

}

static void dt_remove(struct platform_device *pdev){
	printk("dt_probe - remove function ok\n");
}

static int __init my_init(void){

	if(platform_driver_register(&my_driver)){
		printk("error dt_probe, error cargar el driver\n");
		return -1;
	}

	return 0;
}

static void __exit my_exit(void){
	platform_driver_unregister(&my_driver);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alberto Manso");
MODULE_DESCRIPTION("Modulo que lee device tree overlay");

