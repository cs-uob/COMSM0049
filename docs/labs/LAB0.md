# COMSM0049: Assembly Refresher

Welcome to *COMSM0049: Systems and Software Security*! In this course
we're going to show you some of the techniques needed to hack computer
systems alongside how to defend against them.

Unfortunately, to be able to exploit many systems level bugs you need to
be able to work with *machine code*: that is the program code that is
emmited by your compilers and executed directly on the processor. Most
of you won't have done much (if any) assembly language programming
before; and so to give you a bit of a running start this first lab is
going to be a bit of a refresher in how to read it, compile it and run
it.

**If you've done lots of assembly programming before:** feel free to
blast through this work sheet and go (or better yet stick around and
help your classmates!)

**If you've not done much/any assembly programming before:** then this
is going to be a bit of a shock! Assembly programming (unlike high level
programming) is hard and designed to help the machine *run* your code as
opposed to helping you *write* your code. It isn't portable, there's no
real error handling, instructions are named weird and complex things and
`GOTO` is used liberally!

But don't worry! No one is going to expect you to be a master assembly
language programmer having taken this course[^1], or even be able to
write a *real* program in assembly[^2]. There's a big difference between
being able to **read** assembly language, and being able to **write**
it. In this course we'll expect you to be able to read the code and,
given a bit of time and some head scratching as well as looking things
up in the manual, be able to roughly work out what it's doing. We'll
expect you to be able to write a little bit in the labs, given the same
head scratching, TA assistance, and (if necessary) a little bit of quiet
cursing.

Assembly is hard; but you can figure it out! You've got this! Don't be
afraid to ask for help, **everyone** finds this hard, and we're all in
it together.

# Vagrant

All the labs in this course are designed to run in *virtual machines* on
the lab machines setup via Vagrant. Expect to spend the first 10 minutes
of any lab redownloading the VM images[^3]. If you have an X86-based
Linux machine of your own, you might get away with using your own
machine; if you have a Mac you're going to be stuck with the lab
machines. If you have Windows you *might* get away with it, but if you
ask a TA to help diagnose a problem on your own machine, then you'll get
told to use the lab machines: sorry but there aren't enough hours in the
day to debug and test on other systems.

Each week you'll get a `Vagrantfile` to set up your VM. Here's this
weeks!

``` ruby
Vagrant.configure("2") do |config|
  config.vm.box = "generic/debian10"
  config.vm.synced_folder ".", "/vagrant"
  config.vm.provision "shell", inline: "sudo apt update"
  config.vm.provision "shell", inline: "sudo apt install strace nasm"
end
```

Save it in a plaintext file called `Vagrantfile` in a sensibly named
folder, `cd` to it in a terminal, and run:

`vagrant up`  
to bring the VM online.

`vagrant provision`  
to install any software you'll need.

`vagrant ssh`  
to log in to the machine via SSH.

Files in your folder will be available in `/vagrant` on the VM: this'll
be handy for saving your work.

You should be able to get at the `root` account via the `sudo` command.
See `man 8 sudo` if you need help. There isn't a password for the
default user to use `sudo`. You can install stuff via `apt` (you might
need to install things if I forget to add the provision line).

# Hello World

Lets get writing some code! Create a text file called `hello64.S` and
write the following:

``` asm
; hello64.S: a first step into assembly programming!
global _start

  section .text

  _start:
          mov rax, 1
          mov rdi, 1
          mov rsi, msg
          mov rdx, len
          syscall

          mov rax, 60
          mov rdi, 0
          syscall

  section .rodata
  msg: db "Hello, World!", 10
  len: equ $ - msg
```

Compile, link and run it as follows:

``` shell
nasm -f elf64 -o hello64.o hello64.S
ld -o hello64 hello64.o
./hello64
```

## Question 1

Comment every line of `hello64.S` and describe what each line does.
Focus on the *indented lines* to begin with.

- What is a `syscall`?
- What is `mov`?
- What are all those magic numbers?
- What is `rax`, `rdi`, `rsi` and `rdx` and why are things being moved
  into them?

To help you, you ~~might~~ **will** need to refer to the following bits
of documentation:

- `man 2 intro` and `man 2 write`
- [The Intel 64 and IA-32 Architectures Software Developer's Manual:
  Volume
  2](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-instruction-set-reference-manual-325383.html)
- `/usr/src/linux-headers-4.19.0-21-amd64/arch/x86/include/generated/uapi/asm/unistd_64.h`

## Question 2

Disassemble the binary you just made with:

``` shell
objdump -d hello64
```

Look at the output of the command. How does it relate to the code you
just wrote? What about if you run:

``` shell
objdump -D hello64
```

### NOTE

As you're hopefully discovering there are multiple syntaxes for assembly
programming. I prefer the *Intel* syntax but other people (academics and
compiler writers mostly) prefer the AT&T syntax. The GNU tools use by
default the AT&T syntax; NASM uses Intel. You need to be able to read
both. If it bothers you that much, try running:

``` shell
objdump -Mintel -d hello64
```

Similar options are available for GDB and all the other tools you'll
ever meet. If in doubt, use the `man` command.

# Hello World 32bit

Part of what makes assembly *such fun* is that assembly programs are not
portable between different operating systems, different computer
architectures[^4], and even different versions of the same OS on the
same computer! The `hello64` program we wrote was for 64-bit Linux, but
we can also run 32-bit Linux programs on a 64-bit system! Let rewrite
the same *hello world* program for 32-bit Linux! Save the following into
`hello32.S`:

``` asm
; hello64.S: a first step into assembly programming!
global _start

  section .text

  _start:
          mov eax, 4
          mov ebx, 1
          mov ecx, msg
          mov edx, len
          int 0x80

          mov eax, 1
          mov ebx, 0
          int 0x80

  section .rodata
  msg: db "Hello, World!", 10
  len: equ $ - msg
```

Then compile, link, run and disassemble with:

``` shell
nasm -f elf32 -o hello32.o hello32.S
ld -melf_i386 -o hello32 hello32.o
./hello32
objdump -d hello32
```

## Question 3

What has changed and why? What is the calling convention for a 32bit
Linux system call compared to a 64bit Linux System call?

Again, use the manual pages and your favorite search engine to help.
Make sure you're clear on what the differences are! If in doubt stick
your hand up and get the TAs/Lecturers to confirm your suspicions!

# Hello World C

Okay we can compile and decompile a program now and we can see that the
system call convention changes between systems. What does this look like
in C? That should compile to much the same code right? Let try, save the
following as `hello.c`:

``` c
#include <stdio.h>

int main(void) {
  printf("Hello World!\n");
  return 0;
}
```

Compile the program and lets look at the disassembly:

``` shell
cc -o hello-c hello.c
objdump -d hello-c
```

Oh dear. That looks a bit more complicated, but hopefully it isn't too
bad. If you look at the disassembly of the `main` function you should
see that it is calling `puts`. If you look at `man 3` puts or recall
your C programming days, you'll see thats the function used to print
strings… so that makes sense… sort of.

Lets see what systemcalls the program makes with the `strace` command

``` shell
strace ./hello-c
```

Check that the `write` system call still happens as you expect.

## Question 4

Lets look at the library call to `puts` in the `main` function. You'll
notice that C functions use a completely different calling convention!
Go look up what it is for 64bit Linux and make a note of it.

Recompile the C hello world for 32bit Linux: what's the calling
convention now?

``` shell
cc -m32 -o hello-c-32.o hello-c
objdump -d hello-c-32.o
```

Windows has 4 different calling conventions for C functions in x86!
Google what they are and what the ones commonly used for 64bit systems
are (they simplified it a bit). Make a note of them! This is going to
turn up again and again…

# Translating between C and Assembly

So far we've been just looking at Hello World. Lets try looking at
something a little more complicated! Here's a simple C program:

``` c
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool check_password(char *password) {
  char buffer[15];
  char solution[15] = "DPNTN115:JTGVO"; 
  int result;

  strncpy(buffer, password, 14);
  for (size_t i = 0; i < 14; i++)
    buffer[i] += 1;
  result = strncmp(buffer, solution, 14);

  return result? false : true;
}

int main(int argc, char *argv[]) {
  if (argc > 1)
    printf("You %s!\n", check_password(argv[1])? "win" : "lose");
  return 0;
}
```

Compile it and disassemble as normal; but you might want to pipe the
output to `less` this time so you can scroll.

``` shell
make translate-me
objdump -d translate-me | less
```

This time our program has variables, but in assembly languages we don't
(typically) have variables, instead we have space on the stack and CPU
registers for holding values we're immediately working on.

Instead of struggling on with `objdump` lets try using something a bit
easier! The [Godbolt Compiler Explorer](https://godbolt.org)[^5] lets
you see what C code compiles into and will show you which lines of C
produced which bits of assembly. Try compiling with x86-64 GCC and with
no extra flags. Coloured blocks in the C code on the left, correspond to
the same coloured lines of assembly on the right (there are colorblind
settings under *More* then *Settings* if that is helpful).

The first line of the `check_password` function starts by creating
enough room on the stack for all the variables in the function, and the
last line of the function returns the result.

When a register (i.e. `rbp`) contains a *pointer* then `[rbp]`
dereferences it (i.e. `*ptr` in C), and `[rbp+1]` dereferences it with
an offset (i.e. `ptr[1]`. Stare at the code and try and read it. Check
that it roughly makes sense and that you can follow it!

It's **really** important you don't beat yourself up if this is
confusing for you: this is tricky, but the more you do it the easier it
will get. Try and work line by line and check your understanding. You
*do not* need to understand *everything*, but you need to have a high
level gist of what is going on, even if that is just at the level of
*"it's doing something with whatever is in that pointer then calling
`strcmp` with these arguments"*. Ask questions! I still have to blink a
few times whenever I stare at this stuff.

## Question 5

Okay lets test what you read.

1.  How many bytes are allocated on the stack in `check_password` to
    hold all the variables?
2.  What do `rbp` and `rsp` point to? Which is bigger and which is
    smaller?
3.  Those are some big scary numbers in your disassembly! What are they?
    (Hint go look at an ASCII table).
4.  What is the address of the `char buffer[]` relative to the base
    pointer in `check_password`?
5.  What is the address of the `char solution[]` relative to the base
    pointer in `check_password`?
6.  What is the address of `argc` in `main`?
7.  Where does `argv` point to initially and why does it add 8 to it
    instead of 1 in `main`?
8.  What is the password?

# Assembly Comprehension

Okay, lets move away from reading real programs and try and figure out
what little snippets of code do.

## Question 6

What does this snippet of code do?

``` asm
xor      eax,eax
lea      rbx,[0]
loop     $
mov      rdx,0
and      esi,0
sub      edi,edi
push     0
pop      rbp
```

You might need to look things up in [The Intel 64 and IA-32
Architectures Software Developer's Manual: Volume
2](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-instruction-set-reference-manual-325383.html)
if you aren't familiar with an operation.

## Question 7

What about this snippet?

``` asm
.loop:
            xadd rax, rdx
            loop .loop
```

Hint  
Try writing a program using these instructions (look up inline assembly
in C). Good initial values for `rax` and `rdx` might be to set them to
`1` and to see what happens as `rcx` is 1, 2, 3, 4 or 5!

# Writing code

Enough reading: lets try and write something!

# Question 8

Look up the `execve` system call (`man 2 execve`). Write a program in
assembly language that uses `execve` to run the following shell command:

    wc -w /usr/share/dict/words

You're going to have to write similar programs a few times on this
course, but not really anything more complex. Get it done and then
you're done for the week!

# Further Reading

The assembly snippets in questions 6 and 7 came from a fantastic book
called [xchg
rax,rax](https://www.xorpd.net/pages/xchg_rax/snip_01.html). Go through
it and try and figure out what each of the snippets do. Its a great way
to relax in an evening[^6].

If you enjoy writing assembly there is a series of *fantastic* books by
Oscar Toledo Gutierrez called *Programming Boot Sector Games* and *More
Boot Sector Games* that teach you how to write assembly language games
for the boot sectors of X86 machines all in under 510 bytes. Learn to
write *Flappy Bird* in assembly. Petition the librarians in Queen's
Building to buy some if they don't already have copies!

If you're the sort of person who likes to write ungodly fast code and
count CPU cycles you should **definitely** check out [Michael Abrash's
Graphics Programming Black
Book](https://www.jagregory.com/abrash-black-book/). Abrash is the
programmer behind the ancient Quake game[^7], and is one of the finest
programmers out there. His books show you how to use assembly to get the
best possible performance out of a PC and the sort of devious tricks
that make mere programmers run scared! If this is your sort of thing you
might also like to check out [Fabien Sanglard's Game Engine Black Book
DOOM](https://fabiensaglard.net/gebbdoom/) and explore how John Carmack
wrote the original DOOM.

# Bonus Questions

Some of these questions are **much** harder than the rest of the lab…
make sure you've done everything else first. If you don't get to these
don't sweat it; this stuff is just for masochists and those of you who
find this stuff interesting, and want some bonus work. There is no
particular order here; pick what seems interesting and get cracking!

## Hard Question a

When you wrote your solution to question 8 you weren't fussy about what
your assembly code compiled into. Soon you're going to have to get a
little bit tricksy about it.

You saw in question 5 that in assembly programming there is often more
than one way to do write the same code: can you find a way to write the
functional part of the code (i.e. just the bit that sets up the
arguments for `execve` and the `syscall` itself) that when you
dissassemble it **only** uses bytes that occur in the printable ASCII
byte range (i.e. the letters, numbers, and punctuation)?

There are tools which can do it for you (Metasploit has a plugin) but
try and do it yourself.

Hint  
Joshua Mason, et al. "English shellcode." *Proceedings of the 16th ACM
conference on Computer and Communications Security*. 2009.

## Hard Question A

Linux is all very fun and good, but lets try some other systems. Got a
modern Mac? Try and write an assembly hello world. Got a Windows
machine? Do the same! Theres an ancient UNIX teletype in the foyer… can
anyone get hello world running on that?

If you want something a bit more tricksy though… try and write it in
assembly for OpenBSD[^8]. You'll find a Vagrant box for it under
`generic/openbsd7`. The package manager is `pkg_add` and the `doas`
command replaces `sudo`. OpenBSD has a couple of security hardening
tricks/gotchas that make assembly programming trickier. You'll need to
do some research! `;-)`

## Hard Question 1

When you compiled the C version of Hello World your call to `printf` got
switched out for a call to `puts`. It makes sense *why* that happened
(`puts` does a subset of the `printf` functionality) but *who* actually
did the switch and where *exactly* did it happen and why?

If you assign the result of the `printf()` call to a variable it won't
get switched (i.e. `int x = printf("Hello World\n");`). What are the
conditions under which the swap will be made?

Hint  
Go get the GCC source code and start playing with grep `;-)`

## Hard Question α

In the C version of Hello World we saw that the call to `printf`
eventually led to a `write` system call being made. If we look at the
libraries our binary was linked to we can find the machine code for the
puts that gets called.

``` shell
ldd hello-c
```

The `libc.so` is what you want. Go dig into the binary code and figure
out the chain of functions that happens internally in it to lead to the
`write` system call actually getting made.

There are a couple of ways of doing this. If you value your sanity
you'll go find the source code online and trace it in that, or maybe
even a simpler and easier libc like [Musl libc](https://musl.libc.org)
and trace it through that.

If you really want a challenge though do it through disassembly. This is
entering reverse engineering territory here and I would strongly urge
you to look up a proper reverse engineering framework if you want to do
this. I recommend either [Ghidra](https://ghidra-sre.org) (if you like a
GUI) or [Radare](https://radare.org/) (if you are happy on a command
line). The old `objdump` tool is good for simple jobs, but as things get
more complex proper tooling really helps.

[^1]: If you'd like to learn that I'll refer you to Michael Abrash's
    Graphics Programming Black Book.

[^2]: Heck, even me and Sana can't do that easily, and would need a
    quiet lie down in a darkened room after any attempt!

[^3]: Yes, we know this is stupid, and if you use the same machine week
    on week and no one has restarted it you might get away with it… but
    probably not. We're as frustrated as you are by the whole situation.

[^4]: This is why the M1/2-based Mac user are out of luck on this
    course!

[^5]: Turns out the person who wrote it is a Matthew Godbolt, I assumed
    for years it was so named because it was *divinely* useful!

[^6]: Your milage may vary.

[^7]: Now I feel old.

[^8]: Fun fact: OpenBSD is my favorite operating system! It's what I run
    on my own laptop… if you're interested in operating systems come
    talk to me and I'll show you around!
