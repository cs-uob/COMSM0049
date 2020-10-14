# Exercises sheet 3  

## Heap Overflow:
0. During the video lecture, we mentioned about "Where and what" primitive for an vulnerability/exploit. What do you understand by it from an attacker's perspective. In what way, does heap overflow vulnerability provide this primitive (You can consider the example used in the video to explain it)?
1. During the lecture video, I referred to integer overflow as a cause for incorrect heap size calculation, leading to heap overflow. What is integer overflow and how does it help (leads to) heap overflow ([relevant reading](http://phrack.org/issues/60/10.html#article))? For example, what wrong could happen with the malloc allocation w.r.t. size in the following code:  
	
		int main(int argc, char * argv[])
		{
			unsigned short i,n, size;
			char *p, *q;
			if (argc <2)
			{
				printf("1 num argument is required");
				return -1;
			}
		if (atoi(argv[1]) <= 0 ) return -1;// no negative or zero.
		i=atoi(argv[1]);
		size=i*2;
		printf("size: %d, i=%d\n ",size,i);
		p=(char *)malloc(size);
		...
		}
	
2. In the context of malloc *size* (as its parameter), how will you check if the size calculation may contain integer overflow? \[Hint: (re)read about C *short, int, long* type w.r.t. integer overflow.  (see the above article again.). For example: there will be overflow in the expression A + B if for int A and int B, you have A > INT_MAX - B \]
3. Consider the following code snippet: 
		
		int i;
		i = INT_MAX;  // 2,147,483,647(value of INT_MAX)
		i++;
		printf("i = %d\n", i); //@P1
  
	Can you tell (without executing the code), what value of i will be printed at P1? Later you can compile it (after completing the code) and see if your answer matched with the actual executed code.


## Return-Oriented Programming:
1. Read the original article on ROP (up to section 2 is sufficient) By Shacham "The Geometry of Innocent Flesh on the Bone: Return-into-libc without Function Calls" [here](https://hovav.net/ucsd/dist/geometry.pdf). In *section 1.2.5  Wait, What about Zero Bytes*, what problem do you see when the address of a particular gadget contains \00 (e.g. 0x80f400)?
2. Suppose you are acting as a security analyst and oversee the operation of a big company. Can you think of a method that may detect a ROP based attack?
3. In the example that we discussed in the video, why do we need to have a shadow-stack?
