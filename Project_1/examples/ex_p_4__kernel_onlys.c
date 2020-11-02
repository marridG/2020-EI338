//
// Created by GuoZL on 2020-11-02.
//

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/hash.h>
#include <linux/gcd.h>


/* [Module Entry Point] This function is called when the module is loaded. */
int simple_init(void) {
    printk(KERN_INFO
    "Loading Kernel Module \"kernel_onlys\"\n");

    // Page "P-4" Exercise - 1
    // unsigned long GOLDEN_RATIO_PRIME
    printk(KERN_INFO
    "\t<unsigned long> GOLDEN_RATIO_PRIME = %lu\n", GOLDEN_RATIO_PRIME);

    return 0;
}

/* [Module Exit Point] This function is called when the module is removed. */
void simple_exit(void) {

    // Page "P-4" Exercise - 2
    // unsigned long gcd(unsigned long a, unsigned b);
    printk(KERN_INFO
    "\t<unsigned long> gcd(3300, 24) = %lu\n", gcd(3300, 24));

    printk(KERN_INFO
    "Removing Kernel Module \"kernel_onlys\"\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module - Kernel Only's");
MODULE_AUTHOR("SGG");