#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/*
 * printk():
 *  1. the kernel equivalent of printf()
 *  2. output sent to a kernel log buffer, accessible by the "dmesg" command
 *  3. printk() allows priority flags, whose values are given in  <linux/printk.h>
 *
 */


/* [Module Entry Point] This function is called when the module is loaded. */
int simple_init(void) {
    // KERN_INFO: informational message
    printk(KERN_INFO
    "Loading Kernel Module\n");

    // The module entry point function must return an integer value, with 0
    // representing success and any other value representing failure.
    return 0;
}

/* [Module Exit Point] This function is called when the module is removed. */
void simple_exit(void) {
    printk(KERN_INFO
    "Removing Kernel Module\n");

    // The module exit point function returns void
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");