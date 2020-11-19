# Exercises sheet 5
## Defenses:

1. Explain how does a canary 0xff0d000a works as stack canary to protect against overflowing return address where the buffer overflow happens via strcpy()?
2. Read the [article](https://www.usenix.org/system/files/login/articles/105516-Schwartz.pdf) and answer the following questios:

	1. how does W xor X protects against typical shell injection attack as a part of payload (e.g. in the case of a stack buffer overflow vulnerability)?
	2. What are the shortcomings of W xor X (DEP) w.r.t. to ite inability to prevent attacks?
	3. How can mprotect function be used to perform an attack in the presence of a small unrandomized code region with a shellcode to execute? \[see the section "Unrandomized Code"\] 
	
3. Why does C++ code imposes more challenges for a *fine-grained* CFI?
4. From your Lab 2, you exploited a stack buffer overflow bug in a C program. Which of the CFI-- forward or backward edge-- could have been used to prevent that and why? 
5. Why do virtual calls lead to more overhead for CFI solution?
	
## Fuzzing:

1. For an unknown input file format, which of the fuzzing types is more appropriate-- mutational or generational (grammar) and why?
2. What are shortcomings of blackbox fuzzing \[hint: what is the proxy metric for a good fuzzer?\]
3. If you were asked to use Pintool to measure code-coverage for a fuzzer, how you will go about it? (you don't have to provide any pintool code).
4. Read the article "Fuzzing: Hack, Art, and Science" [pdf link](https://patricegodefroid.github.io/public_psfiles/Fuzzing-101-CACM2020.pdf) and answer the following questions:
	1. Why fuzzing is more challenging for netwroked/server type applications?
	2. On page 74, 3rd column, it is written "Note that full program statement coverage is a "necessary but not sufficient" condition to find all the bugs in a program.". Can you elaborate this statement why it is necessary but not sufficient?
	3. In general, which fuzzing type is suitable for fuzzing XML and C code parsers/compiler? 