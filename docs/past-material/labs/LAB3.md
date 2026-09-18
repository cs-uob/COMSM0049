# Lab 3: Building an LSM

## Material

[Vagrantfile](../vagrants/lab3/Vagrantfile)

The `./guest` folder on your host machine is dynamically synced to the
`/vagrant/` in your guest machine. You can use this to easily transfer files in
both directions.
Do note that you should avoid doing compilation (or complex file accesses
generally) in this folder tree.

As usual feel free to customize your vagrant provision script to your heart
desire.

## Recommended Videos

1. [Week 2 - Video 1](https://www.youtube.com/watch?v=c8ucr4UAKyQELC_Inside_LSM)


## Warning

Kernel development should not be done on your working OS!
You may lose data (e.g. crash may corrupt the file system) or you may have difficulty to boot the machine.
A few suggestions:
- Use the provided VM!
- Push your code to github/another machine.
- Save your VM state (right click on the VM in the virtualbox UI).

## Lab objectives

The objectives of this lab are as follow:
* Getting insight in Linux Kernel programing;
* Understanding how the Linux Security Module framework function;
* Getting a feel for how you do kernel development happen in practice.

This last point means looking at existing code and the documentation that is around.
If you were to do this on your own it may mean a lot of googling.
You will be given pointer during this lab on what to do and where to look for the information you need, but you will need to design your own solution!

## What should our LSM do

We are going to do something very basic, and probably not very useful.
You can expand towards more useful features at the end of this lab.

**Given a list of PIDs, we want to prevent the processes associated with those PIDs to create new sockets.**

We will call our LSM module uob.

## Building the linux kernel

First thing first, we need to learn how to compile the Linux kernel!
Some of you may have done this in the past, but don't skip this step as we are going to make this as fast as possible (be warned compiling the kernel may take several dozen minutes!)

**Do this in a VM!**
Use the vagrant file provided to set it up.

The first thing we need to do is download the kernel source code:
``` bash
git clone -b v5.8.7 --single-branch git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
cd linux-stable
```

**Note:** You may want to push this to your own repository on github.

We now need to configure the kernel before we can compilte it.
We want to reasonably minimise the configuration in order to reduce compilation time:
``` bash
make localmodconfig
```

You can learn more about what localmodconfig does and other configuration options by looking at the online documentation [here](https://www.kernel.org/doc/makehelp.txt).

You can further customise your configuration as follow:
``` bash
make menuconfig
```

For now, just save and exit.

Now let's compile our kernel:
``` bash
make -j 16
```

The `-j 16` option set the number of core dedicated to building the kernel.
You should set this value to match the number of core available on your machine.
If you don't know, you can use the [lscpu](https://linux.die.net/man/1/lscpu) command to figure this out.

Compiling the kernel will take a while so be patient!

Once your kernel has compiled you need to install everything:
``` bash
sudo make headers_install INSTALL_HDR_PATH=/usr
sudo make modules_install
sudo make install
```

You can check what those do [here](https://www.kernel.org/doc/makehelp.txt).

Now all you need to do is to reboot the machine:
``` bash
sudo reboot now
```
Make sure to select the kernel you just built in the [GRUB](https://en.wikipedia.org/wiki/GNU_GRUB) menu after reboot.

You can check what kernel is running on your machine with the following command:
``` bash
uname -r
```

## Building Block for our LSM

Our LSM will have two subcomponent:
- A security pseudo filesystem to enter configuration;
- A list of hooks to enforce our policy.

We need to implement those.
The easiest way to get started when doing kernel development is to look how other have done this.
We are going to have a look at [SELinux](https://github.com/torvalds/linux/tree/master/security/selinux) to understand what we need to add.
The two files we are particularly interested by are [selinux/selinuxfs.c](https://github.com/torvalds/linux/blob/master/security/selinux/selinuxfs.c) and [selinux/hooks.c](https://github.com/torvalds/linux/blob/master/security/selinux/hooks.c).

First let's create a directory to put our LSM:
``` bash
cd security
mkdir uob
cd uob
touch fs.c
touch hooks.c
```

### Pseudofile

For our pseudofile, we will need to create an interface to allow adding process PID to the list.
Looking back at the selinux code [here](https://github.com/torvalds/linux/blob/master/security/selinux/selinuxfs.c#L274) and [there](https://github.com/torvalds/linux/blob/master/security/selinux/selinuxfs.c#L328).
Those are a `write` function associated to a pseudo file, and the `ops` associated to that same pseudofile.

Our own skeleton in `fs.c` should look something like this:
``` C
// SPDX-License-Identifier: GPL-2.0-only
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/bug.h>
#include <linux/socket.h>
#include <linux/lsm_hooks.h>
#include <linux/msg.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/xattr.h>
#include <linux/security.h>

static ssize_t uob_write_pid(struct file *file, const char __user *buf,
				 size_t count, loff_t *ppos) {
    // TODO retrieve PID for buf and populate a list
    pr_info("UoB: PID added");
    return 0;
 }

 static const struct file_operations uob_pid_ops = {
	.write		= uob_write_pid,
  .llseek = generic_file_llseek,
};
```

We are going to use [securityfs](https://lwn.net/Articles/153366/) to expose our pseudofile.
We also need to start our pseudo file system module and register the file above:
``` C
static __init int init_uob_fs(void)
{
  struct dentry *uob_dir;

  pr_info("UoB fs: Initializing");
  /* create uob directory in /sys/kernel/security/ */
	uob_dir = securityfs_create_dir("uob", NULL);
  /* create pid file in /sys/kernel/security/uob/ with permission 0666
    and the operations we previously defined */
  securityfs_create_file("pid", 0666, uob_dir, NULL, &uob_pid_ops);

  return 0;
}

__initcall(init_uob_fs);
```

### Hooks

First, we need to identify the hook needed to prevent a process to create a new socket.
To identify all the available lsm hooks, let's look at [include/linux/lsm_hooks.h](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h#L32).
It seems the hook we want to implement is the following [socket_create](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h#L806).
We can see it being used in SELinux [here](https://github.com/torvalds/linux/blob/master/security/selinux/hooks.c#L4490).

We will need something like this:
``` C
// SPDX-License-Identifier: GPL-2.0-only
/* TODO identify needed include */

static int uob_socket_create(int family, int type,
				 int protocol, int kern)
{
  /* TODO:
    retrieve current process PID
    check if it is on the list or not
    return -EPERM if it is
  */
  return 0;
}
```

Similarly to before we need to register our hook and initialize our module:
``` C
/* data structure containing all our hooks */
static struct security_hook_list uob_hooks[] __lsm_ro_after_init = {
  LSM_HOOK_INIT(socket_create, uob_socket_create),
};

static __init int uob_init(void)
{
  pr_info("UoB hooks:  Initializing.\n");
  /* register our hooks */
  security_add_hooks(uob_hooks, ARRAY_SIZE(uob_hooks), "uob");
  return 0;
}

/* define our LSM */
DEFINE_LSM(uob) = {
	.name = "uob",
	.init = uob_init,
};
```

We have now the basic structure for our LSM.
We need to get it to build!

### Makefile and KConfig

You remember menuconfig from earlier?
Well [KConfig file](https://github.com/torvalds/linux/blob/master/security/selinux/Kconfig) populate this.
We need to create such file in `/security/uob/`:

```
config SECURITY_UOB
         bool "UoB - LSM"
         depends on SECURITY
         depends on NET
         depends on INET
         select SECURITY_NETWORK
         select SECURITYFS
         default y
         help
          This our lab LSM module.
```

We also needs to create a [Makefile](https://github.com/torvalds/linux/blob/master/security/selinux/Makefile):
```
#
# Makefile for UoB LSM
#
obj-$(CONFIG_SECURITY_UOB) := uob.o

uob-y := hooks.o fs.o
```

Ok, we are almost done!
We need to tell the kernel to compile it.
We go up in our directory tree and we need to edit `/security/Kconfig` by adding `uob` [here](https://github.com/torvalds/linux/blob/master/security/Kconfig#L240) and [there](https://github.com/torvalds/linux/blob/master/security/Kconfig#L279).
It does not really matter where uob is added on this list, so let's put it at the end!

Now it is the turn of `/security/Makefile`.
Similarly, we need to add `uob` [here](https://github.com/torvalds/linux/blob/master/security/Makefile#L15) and [there](https://github.com/torvalds/linux/blob/master/security/Makefile#L34).

We want to check that everything is working.
Let's run the following in our terminal:
```
make menuconfig
make security W=1
```

In the configuration menu, make sure to select your module.
Alternatively in `security/Kconfig` set `default` to `y` for your module.
You should update the list of security modules via the configuration menu or by running `sed -i -e "s/CONFIG_LSM=\"yama,loadpin,safesetid,integrity,selinux,smack,tomoyo,apparmor\"/CONFIG_LSM=\"yama,loadpin,safesetid,integrity,selinux,smack,tomoyo,apparmor,uob\"/g" .config`.

This will only compile what is contained in the `security` folder and be much faster than compiling the entire kernel.
Fix any error and warning associated with the `uob` LSM.
Once you are satisfied compile the entire kernel, reboot and using `dmesg | grep UoB` you should be able to see the message printed by your LSM!


## Building our LSM

We have learned:
1. to compile the Linux kernel;
2. to build a basic structure for our LSM.

Now we need to do the actual implementation as at the moment our code does absolutely nothing outside of wasting a few CPU cycles every time a new socket is created.
It is now time for you to be creative.

### Pseudofile

You need inside the function `uob_write_pid` to:
1. using [copy_from_user](https://www.fsl.cs.sunysb.edu/kernel-api/re257.html) retrieve the pid written to the pseudo file;
2. if you are passing the pid  as a string (e.g. `echo 1234 > /sys/kernel/security/uob/pid`) convert it to an integer OR write/find a user space program that let you directly write an `int` to the file;
3. add the pid to a global list. Use the kernel [linked list](https://kernelnewbies.org/FAQ/LinkedLists) data structure.
4. in `security/uob/hooks.c` declare your list as [extern](https://www.geeksforgeeks.org/understanding-extern-keyword-in-c/).

### Hook

We need to introduce a nice tool at this point [https://elixir.bootlin.com/](https://elixir.bootlin.com/).
You can use it to find where functions/variables are declared and anywhere they are used in the kernel code.
See for example [llseek](https://elixir.bootlin.com/linux/latest/C/ident/generic_file_llseek).

You need inside the function `uob_socket_create` to:
1. retrieve the pid of the `current` task. You may want to use the function `task_pid_vnr`. Use elixir (linked above) to identify how this function is used with `current`.
2. check if the pid is in the list you built earlier. Check again the [linked list](https://kernelnewbies.org/FAQ/LinkedLists) documentation.

## A little help

You can watch this video to help you get started with the core element of the lab
(try to do it yourself first as it contains the solution).

<iframe width="640" height="360" src="https://web.microsoftstream.com/embed/video/b38f100b-920b-41a0-8de7-796ebe25ba19?autoplay=false&amp;showinfo=true" allowfullscreen style="border:none;"></iframe>

Pointer to material referenced in the video:
- [linux write man](https://www.man7.org/linux/man-pages/man2/write.2.html)
- [copy_from_user](https://lwn.net/Articles/695991/)
- [memdup_user](https://www.kernel.org/doc/htmldocs/kernel-api/API-memdup-user.html)
- [kstrtoint](https://www.kernel.org/doc/htmldocs/kernel-api/API-kstrtoint.html)
- [kfree](https://www.kernel.org/doc/htmldocs/kernel-api/API-kfree.html)
- [socket_create hook](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h#L819)
- [struct task_struct](https://elixir.bootlin.com/linux/v5.8.7/source/include/linux/sched.h#L629)
- [list of unix systems errors](https://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html)

Double check that what I type is correct. In the video, I program from "memory"
and I may not remember the exact name of all functions/macros.

**Clarification:** we are not writing anything to disk here. In our scenario, our pseudofile
simply allows user space to set our pid variable.

**Note:** you can get current pid directly doing `current->pid`.

### Code from the video

hooks.c
```C
// SPDX-License-Identifier: GPL-2.0-only
/* TODO identify needed include */
​
extern int pid;
​
static int uob_socket_create(int family, int type,
				 int protocol, int kern)
{
  /* TODO:
    retrieve current process PID
    check if it is on the list or not
    return -EPERM if it is
  */
	 if (pid == current->pid)
	 	return -EPERM;
​
​
  return 0;
}
​
/* data structure containing all our hooks */
static struct security_hook_list uob_hooks[] __lsm_ro_after_init = {
  LSM_HOOK_INIT(socket_create, uob_socket_create),
};
​
static __init int uob_init(void)
{
  pr_info("UoB hooks:  Initializing.\n");
  /* register our hooks */
  security_add_hooks(uob_hooks, ARRAY_SIZE(uob_hooks), "uob");
  return 0;
}
​
/* define our LSM */
DEFINE_LSM(uob) = {
	.name = "uob",
	.init = uob_init,
};
```

fs.c
```C
// SPDX-License-Identifier: GPL-2.0-only
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/bug.h>
#include <linux/socket.h>
#include <linux/lsm_hooks.h>
#include <linux/msg.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/xattr.h>
#include <linux/security.h>
​
int pid;
​
// echo 1234 > /sys/kernel/security/uob/pid
​
static ssize_t uob_write_pid(struct file *file, const char __user *buf,
				 size_t count, loff_t *ppos) {
		int rc;
		char *str;
		str = memdup_user(buf, count);
		if (!str)
			return -EMEMORY;
		rc = kstrtoint(str, 10, &pid)
		if (!rc)
			goto out;
		rc = count;
​
    pr_info("UoB: PID added");
out:
		kfree(str);
    return rc;
 }
​
 static const struct file_operations uob_pid_ops = {
	.write		= uob_write_pid,
  .llseek = generic_file_llseek,
};
​
static __init int init_uob_fs(void)
{
  struct dentry *uob_dir;
​
  pr_info("UoB fs: Initializing");
  /* create uob directory in /sys/kernel/security/ */
	uob_dir = securityfs_create_dir("uob", NULL);
  /* create pid file in /sys/kernel/security/uob/ with permission 0666
    and the operations we previously defined */
  securityfs_create_file("pid", 0666, uob_dir, NULL, &uob_pid_ops);
​
  return 0;
}
​
__initcall(init_uob_fs);
```

## Going further (extra)

### A cleaner implementation

PID can be reused and maintaining a global list is a very nasty way to do business.
Could we solve this problem more elegantly?
There is a [cred](https://elixir.bootlin.com/linux/latest/source/include/linux/cred.h#L111) data structure associated with every process, in this structure we can store information in a "security blob".

SELinux manipulate those [here](https://github.com/torvalds/linux/blob/master/security/selinux/hooks.c#L6866), [here](https://github.com/torvalds/linux/blob/master/security/selinux/hooks.c#L7280), [here](https://github.com/torvalds/linux/blob/master/security/selinux/include/objsec.h#L31) and [there](https://github.com/torvalds/linux/blob/master/security/selinux/include/objsec.h#L152).
You can see it being used in [selinux_socket_create](https://github.com/torvalds/linux/blob/master/security/selinux/hooks.c#L4493).

We want to define a structure stating either or not a process is allowed to create a new socket (by default yes), modify it via our pseudofile interface, use this in our hook. You will need to spend time understanding the code and doing some research.

### Building a sandbox

We have learned how to elegantly prevent certain process to create socket.
Can you create a more complex LSM?

When a new program is executed this happen:
```
parent fork
child execve
```

Before the execve, we want to interact with our LSM to add some restrictions on what the program we are about to execute can do (e.g. no socket_create, no write etc...).
This should be possible building on what you have done so far.
You need to implement the code to restrict what a process can do in a way similar to what you have for `socket_create`.
You also need to decide how you are going to setup your policies.
Two solutions here, the child calling your LSM via the pseudofile interface as before (easier) OR associating the policy to the file.
The latter can be achieved through [extended attributes](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h#L399) and [bprm_committing_creds](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h).
You should consider when/why you would use an approach rather than an other.

You may also want to ensure that any policy is inherited when forking new process (it would be a shame to be able to exit the sandbox by simply forking).
To do so look at the following [cred_prepare](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h#L604) and [cred_transfer](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h#L609) hooks.

How does what you implemented compare to [seccomp-bpf](https://www.kernel.org/doc/html/v4.16/userspace-api/seccomp_filter.html)? Why do you think they implemented it this way?
