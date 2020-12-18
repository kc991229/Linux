#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/io.h> /* writel */
#include"led.h"
#define GPMCON  0x7F008820 //LED��Ӧ��GPIO���ƼĴ�����ַ
#define GPMDATA 0x7F008824
unsigned int *led_config;//LED���ƼĴ���
unsigned int *led_data;//LED���ݼĴ��� 0��ʾ��LED 1��ʾ�ر�LED
 struct cdev cdev_led; //���ﶨ��һ��cdev�ṹ�����
dev_t devno; //�����������豸�ŵı���
int led_open(struct inode *node, struct file *filp)
{
	//unsigned int reg;
	led_config = ioremap(GPMCON, 4);//�������ַת��Ϊ�����ַ��4���ֽ�
	
	//reg = readl(led_config);
	//reg |= 0x1111;
	
	writel(0x1111, led_config);//��ֵд�����üĴ��� ����Ϊ���
	
	led_data = ioremap(GPMDATA, 4);//�������ַת��Ϊ�����ַ��4���ֽ�
	
	return 0;
 
}
long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case 0:
			writel(0x00,led_data);//��32λ�Ĵ�����д��һ��ֵ
			return 0;
		case 1:
			writel(0xFF,led_data);//��32λ�Ĵ�����д��һ��ֵ
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
	cdev_init(&cdev_led, &led_fops);//��ʼ��cdev_led�ṹ����������
       alloc_chrdev_region(&devno, 0, 1, "myled");//�������豸�� ���ö�̬���䷽ʽ 
      cdev_add(&cdev_led, devno, 1);//ע�����豸��
		return 0;
}
 
static void led_exit()
{
	cdev_del(&cdev_led);//��һ�� ע���豸
	
	unregister_chrdev_region(devno, 1);//�ڶ��� �����뵽�����豸��Ҳע����
}
 
 
module_init(led_init);
module_exit(led_exit);
