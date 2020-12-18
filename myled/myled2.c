#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/io.h> /* writel */
#include"led.h"
#define GPMCON  0x7F008820 //LED对应的GPIO控制寄存器地址
#define GPMDATA 0x7F008824
unsigned int *led_config;//LED控制寄存器
unsigned int *led_data;//LED数据寄存器 0表示打开LED 1表示关闭LED
 struct cdev cdev_led; //这里定义一个cdev结构体变量
dev_t devno; //用来申请主设备号的变量
int led_open(struct inode *node, struct file *filp)
{
	//unsigned int reg;
	led_config = ioremap(GPMCON, 4);//将物理地址转化为虚拟地址，4个字节
	
	//reg = readl(led_config);
	//reg |= 0x1111;
	
	writel(0x1111, led_config);//将值写入配置寄存器 设置为输出
	
	led_data = ioremap(GPMDATA, 4);//将物理地址转化为虚拟地址，4个字节
	
	return 0;
 
}
long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case 0:
			writel(0x00,led_data);//向32位寄存器中写入一个值
			return 0;
		case 1:
			writel(0xFF,led_data);//向32位寄存器中写入一个值
			return 0;
		default:
			return -EINVAL;
	}
}
 static const struct file_operations led_fops = 
{
	.open = led_open,
	.unlocked_ioctl = led_ioctl,
};
static int led_init()
{
	cdev_init(&cdev_led, &led_fops);//初始化cdev_led结构；两个参数
       alloc_chrdev_region(&devno, 0, 1, "myled");//分配主设备号 采用动态分配方式 
      cdev_add(&cdev_led, devno, 1);//注册主设备号
		return 0;
}
 
static void led_exit()
{
	cdev_del(&cdev_led);//第一步 注销设备
	
	unregister_chrdev_region(devno, 1);//第二步 将申请到的主设备号也注销掉
}
 
 
module_init(led_init);
module_exit(led_exit);
