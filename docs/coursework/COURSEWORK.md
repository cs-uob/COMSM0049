# COMSM0051: Systems & Software Security Coursework

## Introduction

This project validates the whole unit, there is no other assessment. It represents a significant investment of time and effort that should mostly take place during Week 8, 9 and 10. Unlike previously unassessed labs, you will work on an open-ended project that you will choose from a list. We encourage you to form groups of 3 students. You will need as part of the project to submit: a proposal (not assessed), your code + video and a final write up. Some projects have been designed to leverage material you may have learned in other units, this is by design. Security is not a silo and encompasses a large number of computer science sub-disciplines.

We expect every member of a group to participate fully in the project. You are free to organise as you wish, but your personal contribution will be evaluated and need to be demonstrated (see below). We are expecting you to work together and to collaborate effectively. If you have any concern about your group dynamic, do contact us via e-mail.

## Deliverable

### Project proposal - group (formative)

By the end of week 8, you need to have a proposal of your project. You will describe the problem you aim to tackle. The main objective of this deliverable is to ensure that everyone is on track and that you have thought through what you need to do. Your proposal should be at most 1 A4 pages and contain the following information:

- The group members;
- The problem you aim to address;
- How you plan to address it;
- A short summary of one or two relevant academic papers;
- What you are proposing to implement exactly.
- How you plan to distribute this work (this does not need to be final, but you need to plan for equal contributions between all members of the group).

You will be contacted to receive feedback on your proposal. The earlier you submit, the earlier feedback will come. We are encouraging you to not wait for the deadline to get this done.

**Submission Instructions:** see on blackboard.

### Project demonstration - group (graded 30%)

You will demonstrate that your solution works and demonstrate your project. Your project should be coming with a README. You will follow the readme instructions and demonstrate that you obtain the results presented in your report and that you can reproduce the evaluation. The video should be no longer than 10 minutes.

The easiest way to record your “screen” is to use OBS or ZOOM.

**Submission Instructions:** see on blackboard.


| Max-grade | Category             | Comment                                                                                                                                                                                                                                                                                                   |
|-----------|----------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 10%       | Technical Clarity    | You will explain clearly how to run your project. Technical terminology should be used appropriately. You should assume an educated audience of your peers and explain terminologies and concepts specific to your project. It should be clear how that relates to the evaluation section of your report. |
| 20%       | Instructions Clarity | The instructions contained in your README should be simple to follow and lead to the results presented in your report. You need to demonstrate this, by following the instructions step by step in your video. We invite you to start from a “clean” environment.                                         |

### Final write up - group (graded 70%)

You should submit a report following [USENIX Latex conference template](https://www.usenix.org/conferences/author-resources/paper-templates) of roughly 5 pages (excluding reference and appendix). Your report should contain a minimum of six academic citations. We suggest the following structure:
- Introduction
- Background
- Design & Implementation
- Evaluation
- Conclusion

Do not hesitate to use figures to illustrate your point, well-drawn figures can communicate more than a thousand words.


**In addition in the appendix you should:**

1. Discuss how well you met your proposal’s objective. If you did not implement everything you described in your proposal this does not mean that you will fail (or get a bad grade). You should discuss why it could not be done (e.g. technical challenges, change of direction, alternative approach taken, sickness of one of the group members etc.).
2. Your individual contribution to the project as a score (e.g., in a group of three if you all worked equally 33% each) + a few paragraphs describing your individual contributions.You need to all agree on this section. **You are all expected to participate in the technical aspects as well as the writing**. We will take into consideration the complexity of your work as well as your individual contributions when deciding your individual grades. Our intent is to ensure that no one is penalized if one (or several) of the students want to work above and beyond expectations. We will only **improve** individual grades, we won’t award any grades below the report grade.

**Submission Instructions:** see on blackboard.

| Max-grade | Category                | Comment                                                                                                                                                                                                                                                                                                                                                                                                                                     |
|-----------|-------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 10%       | Presentation            | You should use the provided latex template properly. Reference should be appropriately formatted. We expect the presentation standard to be on par with the reading material seen during lectures.                                                                                                                                                                                                                                          |
| 20%       | Literature Review       | You will identify the relevant academic literature, show understanding of the papers you have selected and cite them appropriately. It should be clear how they relate to your work. You are expected to explore beyond the paper assigned as reading material.                                                                                                                                                                             |
| 20%       | Design & Implementation | You should describe your implementation at an appropriate level of abstraction (refer to the reading material seen during teaching). You should clearly describe any technical challenges you faced and articulate the design decisions you made and why you believe they were appropriate. This should be understandable by an audience of your peers.                                                                                     |
| 20%       | Evaluation              | You should evaluate how well the outcome of your work addresses your objectives. You should use quantitative (e.g. measuring performance overhead of a security mechanism) or qualitative (e.g. critical discussion of the security guarantees of a mechanism) as appropriate to your project. You are strongly encouraged to draw from evaluations found in the literature to design yours (reference this clearly when this is the case). |

## List of projects

We propose a selection of topics you can choose from for your coursework. As discussed at the beginning of the unit, the coursework will build on notions learned during lectures and labs. We explicitly mention the existing labs (when appropriate) to help you identify a project. We also propose a few more open-ended projects for those interested. You are strongly encouraged to carefully select your project topic. You need to **pick one project** for your group.

### Lab 3

You may want to watch this [video](https://www.youtube.com/watch?v=4rFxZw3USIs&ab_channel=TheLinuxFoundation) before starting on one of those topics.

**Project 1:** Modify Linux to keep track of the last process and user that modified each file. Perhaps log a warning or raise an alarm when a file is modified by an application that hasn't written to that file before.

**Project 2:** Implement more flexible protection mechanisms for Linux (so that any user can create additional protection domains -- sub-users -- to run code with less privileges, without having to be root). See the last “extra” of the third lab for inspiration.

**Project 3:** You can check the library currently loaded for a given process (` sudo lsof -p <pid>`). Can you build a tool that identifies their origin? You may want to take inspiration from the installation graph concept described in [this paper](https://arxiv.org/pdf/2008.11533.pdf).

### Lab 4

**Project 4:** We learned about generating exploits based on ROP. This CW takes the lab to the next step by generating such exploits automatically. In particular:

1. We assume a stack overflow based vulnerability that overwrites the saved return address. You are supposed to automatically  find the input (string) length that is sufficient to overwrite the saved RET (in the lab, you did so by doing manual analysis to find that you needed 44 bytes of junk data before starting to overwrite the saved RET).
2. In the lab, we were generating a ROP chain thatused to setup `execve("/tmp//nc","-lnp","5678","-tte","\bin//sh", NULL)`. In this CW, we need to automatically generate the exploit which takes arbitrary command line for execve and on a successful exploit, you should get that program (argument to execve) launched. (look at the code of the ROPGadget tool)
3. You need to make sure that the exploit works for any chosen .data address (remember, no null bytes!).
4. Rather than forming a ROP of step 2 above (i.e. arbitrary arguments to execve), generate a ROP based exploit for a given arbitrary shellcode (see: [Transforming Malicious Code to ROP Gadgets for Antivirus Evasion](https://ieeexplore.ieee.org/abstract/document/8890330) paper)

### Lab 5

**Project 5:** Find calls to malloc such that the argument to malloc may contain integer overflow bugs. Given a binary:

1. find calls to malloc in a function F
2. find malloc argument, i.e. if malloc(S), find S (calling convention)
3. perform a dataflow analysis to see if the argument to malloc S computed with some arithmetic operation (addition or multiplication, i.e. S= x+y).
4. Perform another dataflow analysis to see if the one of the operands (x or y) of that arithmetic operation is (or related to) an argument to the function F (i.e., if F(a, b), then x or Y is related to a or b.
5. Output such functions.

**Project 6:** Given a binary:

1. iterate over all the functions
2. for each function F, find any loop (if there are) (You may want to use algorithms like Tarjan or Johnson: see [this wikipedia article](https://en.wikipedia.org/wiki/Cycle_(graph_theory).
3. Each loop depends on the concept of back-edge (the edge that forms the loop). Often this is implemented by a compare and jump instruction to the beginning of the loop. By using dataflow analysis, you need to find if this compare instruction depends on a constant or a variable. If it is a variable, perform a dataflow analysis to find if it is related to any of the arguments of the function.
4. output such functions that satisfy the later condition ie. the ones depend on a variable. Often such functions are involved in buffer overflows!

**Project 7:** Implement the static analysis technique used to find similar (vulnerable) functions in a given binary. The technique to implement is [Rendezvous: A Search Engine for Binary Code](https://www.cl.cam.ac.uk/~rja14/Papers/rendezvous.pdf). You are not required to implement all the components of the paper. If you are interested in this, talk to Sanjay to discuss the precise implementation details.

**Project 8:** One of the techniques to find vulnerabilities in binary code is to find clones of known vulnerable functions in a given binary. This project proposes to implement (not in its entirety) the paper: [Detecting code clones in binary executables](https://dl.acm.org/doi/pdf/10.1145/1572272.1572287?casa_token=hxNQP19oipwAAAAA:7lzHocgTF8D7E878TrUqXs7OneuXcSKGkiTvZ3lFykjA4ICy9y6JBS8pHhymeVtpVeuRZT4-OvLE). You will use Ghidra (instead of IDA used in the original paper). As stated, you are not required to implement the whole algorithm, but a part of it. If you are interested in this, talk to Sanjay to discuss the precise implementation details.

**Project 9:** Read [Extracting Compiler Provenance from Program Binaries](http://pages.cs.wisc.edu/%7Ejerryzhu/pub/Rosenblum10prov.pdf). The idea of this project is to identify the compiler that created a given binary. The project involves static analysis using Ghidra to identify instructions of interest and then the application of an ML technique of your choice to generate a classifier. You will be required to collect a good amount of x86 (64) binaries originating from a set of compilers (e.g. gcc, MS visualstudio, intel cc, gcc, clang etc. a maximum of three compilers is good.). You should implement the technique(s) presented in the above mentioned paper.

### Lab 6

**Project 10:** You will use dynamic taintflow analysis to find if an argument to malloc is tainted. To do so, you will use [libdft](https://github.com/AngoraFuzzer/libdft64) to perform the taintflow analysis. This is based on Intel PIN. For more information on taintflow, read: [wikipedia article](https://en.wikipedia.org/wiki/Taint_checking ), [libdft info](https://www.cs.columbia.edu/~vpk/research/libdft/) and Sections I & II of [this paper](https://users.ece.cmu.edu/~aavgerin/papers/Oakland10.pdf).
Once you can identify tainted information at the byte-level that is affecting a malloc call, create a simple fuzzer that changes those bytes (mutation) and feed it to the application to see if any generated input results in a crash.

### Lab 7

**Project 11:** Expand the rootkit you implemented in [Lab 7](https://cs-uob.github.io/COMSM0049/labs/LAB7.html) to further hide your malicious payload (this is quite open-ended, but lab 7 gives a few potential directions, do check the lab).

### Open-ended

**Project 12:** Building on week 2 videos on intrusion detection. Identify a set of papers (2 or 3) proposing intrusion detection algorithms (ideally with source code available, e.g.,  [Kistune](https://gangw.web.illinois.edu/class/cs598/papers/NDSS18-intrusion.pdf)). Identify publicly available datasets. Using those, design a benchmark to evaluate the effectiveness of the different solutions.

**Project 13:** This project is about writing queries to find vulnerable patterns in the source code. This is integrated in Github. The query language is called [COdeQL](https://help.semmle.com/codeql/codeql-overview.html). This provides an built-in support for dataflow (and taintflow) analysis. With this, we can find patterns where dependency to user input can be shown. Your task is to learn CodeQL (for C lang only) and write COdeQL patterns that find some interesting bugs (e.g. heartbleed, insecure malloc, use-after-free etc.). A similar platform is: [DDlog](https://github.com/vmware/differential-datalog).
