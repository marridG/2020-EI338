#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#include <linux/hash.h>


#define BUFFER_SIZE 128

#define PROC_NAME "seconds"
// #define MESSAGE "Hello World\n"

static unsigned long START_JIFFIES = 0;

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,

        // the name of the function (proc_read() here) that is to be called whenever /proc/hello is read.
        .read = proc_read,
};


/* This function is called when the module is loaded. */
static int proc_init(void)
{

    // creates the /proc/hello entry
    // the following function call is a wrapper for
    // proc_create_data() passing NULL as the last argument
    proc_create(PROC_NAME, 0, NULL, &proc_ops);

    START_JIFFIES = jiffies;
    printk(KERN_INFO "[Assignment 2] /proc/%s created @jiffies=%lu\n",
           PROC_NAME, START_JIFFIES);

    return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void) {

    // removes the /proc/hello entry
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "[Assignment 2] /proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/hello is read.
 *
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];   // "buffer" exists in kernel memory
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    rv = sprintf(buffer, "Elapsed Time in Seconds = %f\n",
                (double)(jiffies-START_JIFFIES) / HZ);

    // copies the contents of buffer to userspace usr_buf
    copy_to_user(usr_buf, buffer, rv);  // "usr_buf" exists in user space.

    return rv;
}


/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module, modified upon SGG's");
MODULE_AUTHOR("G");
