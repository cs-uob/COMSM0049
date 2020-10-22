# Lab 7: kernel rootkit

This lab continue our introduction to linux kernel programming and OS security following lab 3.
In this lab you will build a rootkit (please, watch the [corresponding video](https://web.microsoftstream.com/video/e753a384-ae8c-4261-9579-911fbbc9b184) from week 6).

## Recommended Video

[Week 6 - Video 1](https://web.microsoftstream.com/video/e753a384-ae8c-4261-9579-911fbbc9b184)

## Warning

Kernel development should not be done on your working OS!
You may lose data (e.g. crash may corrupt the file system) or you may have difficulty to boot the machine.
A few suggestions:
- Use the provided VM!
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

The goal o this lab is to build such rootkit!
