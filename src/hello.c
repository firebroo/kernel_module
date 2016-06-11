#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>  
#include <linux/fs.h>  
#include <linux/errno.h>  
#include <linux/types.h>  
#include <linux/fcntl.h>  
#include <linux/cdev.h>  
#include <linux/version.h>  
#include <linux/vmalloc.h>  
#include <linux/ctype.h>  
#include <linux/pagemap.h>
 
#define COMMAND 1
 
static char 	alpha[27];
dev_t 		devnum;
static int 	count = 10;
static char 	*init_mesg = "hello,world\n";
static char 	*exit_mesg = "goodbye\n";
static int 	major = 246;
static int 	minor = 10;

int static hello_dev_open(struct inode *inode, struct file *file)
{
         printk("file open in hello_dev_open......finished!\n");
        
         return 0;
}
 
 
int static hello_dev_release(struct inode *inode, struct file *file)
{
         printk("file release in hello_dev_release......finished!\n");
         return 0;
}
 
 
 
ssize_t hello_dev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	int  cnt;

	if(count > 26)
	        cnt = 26;
	else
	        cnt = count;
	/*使用copy_to_user ()函数从driver读数据到user*/
	if(!copy_to_user((char *)buf, alpha, cnt)) {
		printk("copy to user: [%s]\n", buf);
		return cnt;
	}
	else
		return -1;
}

long hello_ioctl(struct file *flip, unsigned int cmd, unsigned long arg ) {
	if (cmd == COMMAND ) {
		printk("ioctl COMMAND  successful\n");
		return 0;
	}	
	printk("ioctl error\n");
	return -1;
}

void init_alpha(void) {
	int 	i;

	memset(alpha, 0, 27);
	for(i = 0; i < 26; i++)
	        alpha[i] = 'a' + i;
}
 
 
ssize_t hello_dev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	int  cnt;

	if(count > 26)
		cnt = 26;
	else
		cnt = count;
	/*使用copy_from_user()函数从user写数据到driver*/
	if(!copy_from_user((char *)alpha, buf, cnt))
	{  
	        printk("copy from user: [%s]\n", alpha);
	        return cnt;
	}  
	else
	        return -1;
}

loff_t hello_lsseek(struct file *filp, loff_t off, int whence)  
{  
        loff_t newpos = -1;

        switch(whence)
        {
        case 0: /* SEEK_SET */
                newpos = off;
                break;

        case 1: /* SEEK_CUR */
                newpos = filp->f_pos + off;
                break;

        case 2: /* SEEK_END */
        default: /* can't happen */
		return -1;
        }
        if (newpos < 0)
                return -1;
        filp->f_pos = newpos;
	printk("flip f_pos = %ld\n", (long)newpos);
        return newpos;
}  
 
 
static struct cdev hello_dev;
static struct file_operations fops ={
         .owner   	   = THIS_MODULE,
	 .llseek           = hello_lsseek,
         .open    	   = hello_dev_open,
         .release          = hello_dev_release,
         .read             = hello_dev_read,
         .write            = hello_dev_write,
	 .unlocked_ioctl   = hello_ioctl,
};
 
static int __init hello_init(void)
{
	int ret;
	//for(i = 0; i < count; i++)
	printk(init_mesg);
	//ret = register_chrdev_region(MKDEV(major,minor), 1, "hello_dev");
	ret = alloc_chrdev_region(&devnum, 10, 1, "hello_dev");
	if(!ret)
	{
	     major = MAJOR(devnum);
	     minor = MINOR(devnum);
	     printk("major = %d; minor = %d\n", major, minor);
	}

	cdev_init(&hello_dev, &fops);
	ret = cdev_add(&hello_dev, devnum, 1);

	init_alpha();

	return ret;
}
 
void __exit hello_exit(void)
{
	printk(exit_mesg);
	cdev_del(&hello_dev);
	//unregister_chrdev_region(MKDEV(major, minor), 1);
	unregister_chrdev_region(devnum, 1);

	return;
}
 
void hello(void)
{
	printk("good morning\n");
}
 
module_param(count, int, S_IRUGO);
module_param(init_mesg, charp, S_IRUGO);
module_param(exit_mesg, charp, S_IRUGO);
 
EXPORT_SYMBOL_GPL(hello);
module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("firebroo");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("a simple example with kernel module");  

