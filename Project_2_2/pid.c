/**
 * Kernel module that communicates with /proc file system.
 *
 * This provides the base logic for Project 2 - displaying task information
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

static int PID;                      // the current pid

/**
 * Function prototypes
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
        .write = proc_write,
};

/* This function is called when the module is loaded. */
static int proc_init(void) {
    // creates the /proc/procfs entry
    proc_create(PROC_NAME, 0666, NULL, &proc_ops);

    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

    return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void) {
    // removes the /proc/procfs entry
    remove_proc_entry(PROC_NAME, NULL);

    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;
    struct task_struct *tsk = NULL;

    if (completed) {
        completed = 0;
        return 0;
    }

    tsk = pid_task(find_vpid(PID), PIDTYPE_PID);
    completed = 1;

    // obtain the <struct pid>, of the current PID
    pid strct__pid__crt = find_vpid(PID);
    if (NULL == pid){
        printk(KERN_INFO "[Error] PID %d NOT Found at find_vpid()\n", PID);
        return 0;
    }
    else {
        // obtain the <struct task_struct.>, of the current PID
        task_struct strct__tsk_strct__crt = pid_task(strct__pid__crt, PIDTYPE_PID);
        if (NULL == strct__tsk_strct__crt){
            printk(KERN_INFO "[Error] PID %d NOT Found at pid_task()\n", PID);
            return 0;
        }

        rv = sprintf(buffer, BUFFER_SIZE,
                     "command = [%s] pid = [%d] state = [%ld]\n",
                     (strct__tsk_strct__crt->comm, PID, strct__tsk_strct__crt->state);
    }

    // copies the contents of kernel buffer to userspace usr_buf
    if (copy_to_user(usr_buf, buffer, rv)) {
        rv = -1;
    }

    return rv;
}

/**
 * This function is called each time we write to the /proc file system.
 */
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos) {
    char *k_mem;

    // allocate kernel memory - routine kernel memory allocation
    k_mem = kmalloc(count, GFP_KERNEL);

    // copies user space "usr_buf" to kernel buffer
    if (copy_from_user(k_mem, usr_buf, count)) {
        printk(KERN_INFO "Error copying from user\n");
        return -1;
    }

    /**
     * kstrol() will not work because the strings are not guaranteed
     * to be null-terminated.
     *
     * sscanf() must be used instead.
     */
    sscanf(k_mem, "%d", &PID);
    printk(KERN_INFO "Current PID is now %l\n", PID);

    // release memory
    kfree(k_mem);

    return count;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module Modified upon SGG");
MODULE_AUTHOR("G");

