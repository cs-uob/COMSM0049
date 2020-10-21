# Exercises sheet 4
## Static Analysis: 
1. What do you understand by a complete and sound program analysis technique?
2. What are the code patterns (constructs) that, in general, makes static analysis hard or inaccurate?
3. What do you understand by fix-point iteration from dataflow analysis point of view?
4. What is a transfer function for a dataflow analysis
5. What is a interprocedural control flow graph?

## Dynamic Analysis: 

1. Why does a whole program analysis (all possible executions) is an issue for a dynamic analysis?
2. What is program instrumentation?
3. "Often static code instrumentation is faster than dynamic instrumentation"-- What do you think about it and why? 
4. Intel Pintool Specific questions.
	1. What are instrumentation and analysis routines?
	2. Download pintool as described in the lecture. Go to the directory source/tools/ManualExamples. Copy [inscount-dif.cpp](../code/inscount-dif.cpp) in that directory. What does this new pintool does?
	3. Edit makefile.rules by appending 'inscount-dif' at the line starting with TEST_TOOL_ROOTS. Download [loop2.c](../code/loop2.c) and compile it as loop. Compile this new pintool as follows: 
		
			$ export PIN_ROOT=<path-to-pin-root-directory>
			$ make obj-intel64/inscount-dif.so TARGET=intel64
			$ ../../../pin -t obj-intel64/inscount-dif.so -- loop
	
	Open inscount.out. You will see two numbers. icount and icount2, with *icount2 < icount*. Can you explain the reason? [Hint: Something to so with instrumentation and analysis routines!!]