# Week 3 Homework

## Reading 

There was quite a lot of reading last week..., so lets lighten it up a
bit this week.

- [Heres the disclosure for the return-to-libc exploit](https://seclists.org/bugtraq/1997/Aug/63)

Curl yesterday had a heap-based buffer overflow released! Curl (if you haven't
heard of it) is one of those fundamental technologies holding up the internet;
so it's always interesting to see these severe bugs appearing… 

- [Disclosure notice](https://curl.se/docs/CVE-2023-38545.html)
- [Redhat leaked the patch a little early…](https://cohost.org/lifning/post/3137540-lib-curl-cve-2023-38)

## Alternative explanations and videos

- [LiveOverlow 0x29: Introducing Weird Machines: ROP Differently
  Explaining part 1](https://youtu.be/8Dcj19KGKWM) (quite a nice
  high level introduction to ROP... the lab will give you a more
  practical intro)
  
  
### A Bonus (for those interested)

Last week we talked about heap overflows and use after free bugs.
Last summer, a *brilliant* exploit was demonstrated in the TAS Block
of the *Summer Games Done Quick* speedrunning conference: Legend of Zelda Ocarina of Time Triforce
Percent.

If you haven't seen it, have an hour to spare and have a fondness for
old Nintendo games (or are old enough to have been a kid in the 90s
and spent many hours on the playground discussing if it was actually
possible to get the Triforce in OoT...) I'd strongly
recommend it.

- [OoT Triforce Percent](https://youtu.be/2x_pqyrf9lA)

It ends with a lovely note by the developers that hacking is often
portrayed as being *destructive*... breaking systems.  Yet their
exploit is *creative*: by using hacking techniques they can create new
games, new content, and make childish dreams come true. 

As well as the run itself, theres also an explanation video of how it
works.

- [Finally Obtaining the Triforce in Ocarina of Time: Triforce Percent Explained](https://www.youtube.com/qBK1sq1BQ2Q)

These are all techniques we've been covering, and which *you've* been
doing in the labs.  *Cool or what?*
  
## Exercises

1. Describe at a high level how to gain arbitrary code execution with W^X memory by
  using shellcode without using ROP.  Hint: you'll need to use
  `mprotect()`. (5 marks)
  
  
2. Write a ROP chain to set =rax= to 1 and =rbx= to 2.
  Assume you have access to the following gadgets:
  
  ```assembly
  xor_ab: xor rax, rbx ; rax = rax ^ rbx;
          ret

  xor_ba: xor rbx, rax ; rbx = rbx ^ rax
          ret
          
  pop_a:  pop rax;
          ret
  ```
  
  (5 marks)
  


3. To further reduce the amount of entropy spent randomising libraries,
  some systems randomise the offset a library will be loaded at on
  boot and then reuse the same offset for all loaded libraries in that
  session; rather than randomising on every program load. Discuss the
  tradeoff and suggest whether this would make a ROP based attack
  harder or easier? 
  
  (15 marks) 


<!--
## Answers (do not check this before you try to answer the exercices alone)

![View the source, Luke](https://raw.githubusercontent.com/cs-uob/COMSM0049/master/docs/extra/view-the-source.jpg)

1. Overflow stack buffer and ensure that shellcode is written to writable memory at a predictible address (1) 

e.g. stack buffer or environment variable (1).

Overflow stack buffer creating two stack frames

Set first frame to call mprotect marking the shellcode as executable (1)

Set second to call shellcode (1)

Yes you could quibble ret2libc is ROP in disguise.  I'd propbably give a point for that very reasonable quibbling...

2.    See the hacker's delight! its an old dirty trick to swap two variables without an intermediary:

pop_a
2
xor_ab ; a = 2 ^ b; b = b
xor_ba ; a = 2 ^ b; b = b ^ 2 ^ b = 2 (remember x ^ x = 0, and 0 ^ y = y)
pop_a
1

3.  So this is what iOS and Android do (or at least used to do).
The advantage of this is that it can dramatically reduce program startup time which is good for phones and other constrained devices where you want a snappy response.  The downside is that once that off set is leaked then until the phone restarts (and when do you ever do that?).  In a mobile OS with an app store where you have pretty good assurance that code has been checked and isnt directly leaking pointers thats probably okay but in a free for all general purpose OS probably not... which is why most of them dont do it.  

It makes ROP slightly easier because you don't need to defeat ASLR in order to find the gadgets.  Leak the pointers to the gadgets from a program you write then on next run the offsets will still be the same and on running a different program they're the same.
-->
