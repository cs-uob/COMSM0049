# Lab 4 - ROP based Exploitation

**Task 1**: Exploit the program and use ROPgadget to generate a ROP shellcode.

**Task 2**: Write using ROPgadget a custom shellcode to spawn a reverse shell.

Setting up the environment:  

1. Here's a Vagrant file for this weeks lab that we have tested.

```
# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.define "box" do |box|
                box.vm.box = "ubuntu/bionic64"
                box.vm.hostname = "lab4-box"
                box.vm.provider "virtualbox" do |virtualbox|
        virtualbox.name="lab4-box"
    end
 end
end
```

Run the usual `vagrant up` to bring the VM up, and `vagrant ssh` to login.  Your current folder will be mounted in `/vagrant` as before.
You'll need to install some extra dependencies for this lab to get going comfortably.  From a shell:

```
sudo apt update
sudo apt install -y python3-pip gdb gcc-multilib
pip3 install capstone
```

1.  Download ROPGadget and install from: [URL](https://github.com/JonathanSalwan/ROPgadget). Its a github repo, so you can either clone it or (suggested) simple use "Download zip" option. 
2.  Download netcat (the latest release of netcat that comes pre-installed in Ubuntu has removed a particular option (-e) that we need.
Having said that, official netcat release still shipped with that option! So, we are not completely artificial ;). [URL:](https://sourceforge.net/projects/netcat/). However, the same is also avaialble [here](https://github.com/cs-uob/COMSM0049/blob/master/docs/2021/code/nc071.tar.gz).
3.  untar it and build: `./configure` and `make` command (**do not do** *make install*!)
4.  move `src/netcat` to `/tmp/nc`: `cp src/netcat /tmp/nc` (check if the binary is working as expected `/tmp/nc --help`).
5.  Compile [vuln3.c](https://github.com/cs-uob/COMSM0049/blob/master/docs/2021/code/vuln3.c) as `gcc -fno-stack-protector -m32 -static vuln3.c -o vuln3-32`
6.  Find the amount of padding you'll need to overflow the buffer and start overwriting the saved return address.
7.  Use ROPgadget to build a ROP shellcode. 

		ROPgadget --binary vuln3-32 --ropchain > out-rop.txt
		
   Have a look at `out-rop.txt`.  You'll see that at the bottom there is a Python program which you can complete to generate your shellcode exploit (in particular the buffer padding).  Run the program generate the exploit and test it works with the vulnerable program.

8.  The shellcode in the previous step execve'd `/bin/sh`.  This time we'd like you to run the following command:

		/tmp/nc -lnp 5678 -tte /bin/sh

   This command will spawn a reverse-shell server listening on localhost port 5678 that feeds all the input it gets into `/bin/sh`.  Effectively enabling you to run programs remotely!
We're giving helper files to consult which provide a similar exploit for a different machine: [exploit-nc.py](https://github.com/cs-uob/COMSM0049/blob/master/docs/2021/code/exploit-nc.py), but you'll need to get it working on *your* machine.  In particular the gadgets, configuration and Python version may be different!

9.  Once successful, open another terminal and type:  
```
/tmp/nc 127.0.0.1 5678
pwd
```
You've got a reverse shell!

# Hints

1.  To figure out the padding needed to overflow the buffer try using binary search.  If you run `perl -e 'print "A"x124, "DCBA"' >input` you can quickly test different inputs. What would you expect to see when you have correctly got control of the return address?  What would you expect to see if you've gone too far compared to not far enough?
2.  Can't concatenate strings and bytes?  Just use bytes! `bytes("Your string", "ascii")`.
3.  The calling convention for a system call on 32bit X86 Linux is: `eax` syscall number, `ebx,ecx,edx,esi,edi,ebp` arguments as required.
4.  Try and get the exploit chain running first!  Break on the first gadget (In GDB: `b *0x08BLABLA`) and check it is working as expected.  Are things going into the right registers? (In GDB: `si`: step instruction, `i r`: info registers)
5.  Once you're convinced the ROP chain is running the `strace` tool is very helpful to debug what system calls *actually* get made by your program.  The `-v` flag will show all the arguments to system calls, `-e trace=execve` to just show `execve` calls.  Does the call look like what you'd expect?  You may have to add some new code to your exploit to get it working if the memory you're using isn't clean!

# Optional bonus

- Get the `nc` exploit going in 64-bit mode too!  Is it easier or harder?
- Have a play at using [pwntools](https://docs.pwntools.com/en/stable/) to write your exploit instead of ROPgadget.  Is it easier? Harder?
- Have a go at a return to libc exploit as discussed in the lecture.  You'll need to defeat ASLR for this one.
