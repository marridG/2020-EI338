//
// Created by GuoZL on 2020-11-02.
//

// Exercises on Page P-4
// 1. Print out the value of GOLDEN_RATIO_PRIME in the simple init() function.
// 2. Print out the greatest common divisor of 3,300 and 24 in the simple_exit() function.
// 3. Print out the values of jiffies and HZ in the simple init() function.
// 4. Print out the value of jiffies in the simple exit() function.

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/hash.h>
#include <linux/gcd.h>


/* [Module Entry Point] This function is called when the module is loaded. */
int simple_init(void) {
    printk(KERN_INFO
    "Loading Kernel Module \"Exercises on Page P-4\"\n");

    // Page "P-4" Exercise - 1
    // unsigned long GOLDEN_RATIO_PRIME
    printk(KERN_INFO
    "\t[EX 1] <unsigned long> GOLDEN_RATIO_PRIME = %lu\n", GOLDEN_RATIO_PRIME);

    // Page "P-4" Exercise - 3
    // HZ: timer tick rate (defined in <asm/param.h>)
    printk(KERN_INFO
    "\t[EX 3] HZ = %d\n", HZ);

    // Page "P-4" Exercise - 4
    // jiffies: # of timer interrupts occurred since the system was booted (defined in <linux/jiffies.h>)
    printk(KERN_INFO
    "\t[EX 4] jiffies = %lu\n", jiffies);


    return 0;
}

/* [Module Exit Point] This function is called when the module is removed. */
void simple_exit(void) {

    // Page "P-4" Exercise - 2
    // unsigned long gcd(unsigned long a, unsigned b);
    printk(KERN_INFO
    "\t[EX 2] <unsigned long> gcd(3300, 24) = %lu\n", gcd(3300, 24));

    printk(KERN_INFO
    "Removing Kernel Module \"Exercises on Page P-4\"\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module - Kernel Only's");
MODULE_AUTHOR("SGG");