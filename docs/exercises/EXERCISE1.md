# Exercises sheet 1
---
## Assembly/binary/ABI:
	1. "By design, x86-64 calling convention is faster than x86-32 cdecl calling convention". What do you think about this statement?
	2. How do you recognize the access to function's arguments and function's local variable at the assembly level. You can assume that these are always accessed w.r.t. RBP register.  

## Stack buffer overflow:
	1. Let us assume that there is a buffer overflow in a function due to the presence of one string buffer B. There are other local variables present in the function. Some declared (in the source code) before B and some after B. Location wise (w.r.t. the address of these variables, considering the address &B), which of these local variables can be overflown as a result of overflowing B? For example, in the video lecture, I mentioned about the concept of WYSINWYX. Can you think of a scenario where the above point 1 changes your understanding of valriable locations when you look at the binary of the code? Consider lottary.c for example.
	2. Read about off-by-one error which is also a form of buffer overflow, but with a limited scope of overflowing.
## Format String:
	1. read about %n specifier and think about what can be done with it? Give a small scenario which uses this specifier to change the behavior of your code. You can write any insecure code yourself and be creative in using %n to change the program behaviour.
	2. For the example, given on the last slide, think about the problem and exploitation on x86-32 and x86-64 bit systems. Hint: calling convention!

## Concurrency Vulnerabilities:
1. Reproduce in a virtual machine the Dirty COW vulnerabilities.
Please, refer to the [website](https://dirtycow.ninja/).
You may need to run a specific version of the kernel for this to work.
Look at how to setup virtual machines using [vagrant](https://learn.hashicorp.com/collections/vagrant/getting-started).
You will need to setup a vagrant box and install (or compile) a specific version of the kernel.
This can be done in the vagrant provision script.
You are encouraged to use Google liberally!

2. Explain the examplar code provided on the Dirty COW website.

  ---
