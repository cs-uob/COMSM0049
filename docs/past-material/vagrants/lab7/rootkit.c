#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas");
MODULE_DESCRIPTION("Hello Module");
MODULE_VERSION("0.0.1");

#define START_ADDRESS 0xffffffff81000000
#define END_ADDRESS 0xffffffffa2000000
#define SYS_CALL_NUM 300

#define DISABLE_W_PROTECTED_MEMORY \
    do { \
        preempt_disable(); \
        write_cr0(read_cr0() & (~ 0x10000)); \
    } while (0);
#define ENABLE_W_PROTECTED_MEMORY \
    do { \
        preempt_enable(); \
        write_cr0(read_cr0() | 0x10000); \
    } while (0);

void **sys_call_table;

void **find_syscall_table(void)
{
    void **sctable;
    void *i = (void*) START_ADDRESS;

    while (i < (void*)END_ADDRESS) {
        sctable = (void **) i;
        // sadly only sys_close seems to be exported -- we can't check against more system calls
        if (sctable[__NR_close] == (void *) sys_close) {
        size_t j;
        // sanity check: no function pointer in the system call table should be NULL
        for (j = 0; j < SYS_CALL_NUM; j ++) {
            if (sctable[j] == NULL) {
                goto skip;
            }
        }
        return sctable;
        }
        skip:
        i += sizeof(void *);
    }

    return NULL;
}

unsigned long read_count = 0;

asmlinkage long (*original_read)(unsigned int, char __user *, size_t);

asmlinkage long hacked_read(unsigned int fd, char __user *buf, size_t count)
{
    read_count ++;

    pr_info("%d reads so far!\n", read_count);
    return original_read(fd, buf, count);
}

static int __init lkm_example_init(void) {
    printk(KERN_INFO "Hello, World!\n");

    sys_call_table = find_syscall_table();
    pr_info("Found sys_call_table at %p\n", sys_call_table);

    void **modified_at_address = &sys_call_table[__NR_read];
    void *modified_function = hacked_read;

    DISABLE_W_PROTECTED_MEMORY
    original_read = xchg(modified_at_address, modified_function);
    ENABLE_W_PROTECTED_MEMORY

    return 0;
}

static void __exit lkm_example_exit(void) {
 printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
