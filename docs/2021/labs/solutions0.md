# COMSM0049: Assembly Refresher Solution Guide

# Question 1

``` asm
; hello64.S: a first step into assembly programming!
global _start            ; Make the _start label accessible from the ELF symbol table

  section .text          ; Here be the code!

  _start:                ; Here is the entry point to the program, or where the kernel will jump to once the program has been loaded
          mov rax, 1     ; Were going to start making a system call.  The systemcall number (its identifier) goes in RAX... 1 is the write system call
          mov rdi, 1     ; Argument 1 goes in RDI.  Its the file descriptor of where to write to: in this case stdout (1)
          mov rsi, msg   ; Argument 2 is a char pointer to a string to print
          mov rdx, len   ; Argument 3 is its length
          syscall        ; Make the system call!

          mov rax, 60    ; Now we want to make an exit system call to stop the program cleanly.  60 is the exit system call
          mov rdi, 0     ; What code to return to the OS: 0 is EXIT_SUCCESS (usually)
          syscall        ; Make the system call!

  section .rodata        ; Here is the read only data section of the code.  
  msg: db "Hello, World!", 10 ; Our message is a string of bytes. It is ended by 10 which is the ASCII code for a newline.
  len: equ $ - msg       ; The length of our string is the distance from the msg label to here ($).
```

- A `syscall` is where you ask the operating system to do something and the interface for the Linux kernel is documented in section 2 of the manual.
- A `mov` instruction moves a value (absolute or in register) into a register. 
- See the code above.
- See the Linux syscall ABI for 64bit X86; but essentially is the syscall you want to call and the rest are the arguments (with further ones going on the stack).

# Question 2

Its the compiled code!  Remember assembly has a much closer relationship to the compiled form than a high level language.  You will also see some extra sections in the binary the assembler has added.  Ask if you're unsure what any of them are.

# Question 3

Syscall numbers have changed (write is now 4, exit is now 1), and the argument order has changed.  Now its syscall number in EAX, and args in EBX, ECX, EDX.

The syscall instruction is gone too in favour of calling an interupt (with argument 0x80).

# Question 4

[The internet science site knows all.](https://en.wikipedia.org/wiki/X86_calling_conventions)  In this case it will be the System V AMD64 ABI.

# Question 5

1. 64 (see the `sub` on line 4).
2. `rbp` points to the bottom of the current stack frame, `rsp` points to the top of the stack.  Since the stack grows down, and `rbp` will be pointing lower in the stack than the top (`rsp`), `rbp` will be a higher memory address.  Yes this is confusing, and its just a historical pecularity at this point.
3. Its the "Hello, World!" string.  Because the variable is being declared and assigned in the function it is getting recreated on every function call.  To do so it converts the string into an `int` by reading the bytes in order and blatting it into a register before loading it into memory.  A good optimizing compiler will probably remove this code sharpish given half a chance and just hard code the string in memory.  *Bonus:* why not load the value directly instead of going through RAX?
7. Because a `char *` is 8 bytes wide.
8. COMSM0049ISCOOL if I remember correctly.  Which it is.

# Question 6

All different ways of setting each of the registers to 0. 

# Question 7

Calculates Fibonacci numbers!  `xadd` is exchange and add.  

# Question 8


