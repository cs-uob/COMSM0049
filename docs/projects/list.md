## Introduction Videos

<iframe width="640" height="360" src="https://web.microsoftstream.com/embed/video/b510a996-08fd-4169-9292-9dded1f99d5f?autoplay=false&amp;showinfo=true" allowfullscreen style="border:none;"></iframe>

<iframe width="640" height="360" src="https://web.microsoftstream.com/embed/video/d93eff54-42cb-4289-a3dc-119d63f6c9a2?autoplay=false&amp;showinfo=true" allowfullscreen style="border:none;"></iframe>

## Open projects (Sanjay and/or Thomas)

You may come to us with projects idea relating to the course.

## From container to unikernel (Thomas)

Containers lack strong isolation and prove problematic in a number of application domains. Unikernel is a potential answer but lacks the practicality of container solutions such as Docker. Using something like Linux Lupine [1,2] can you automate the transformation of containers in unikernels? You will particularly focus on containers as used to deploy microservices.

[1] [https://github.com/hckuo/Lupine-Linux](https://github.com/hckuo/Lupine-Linux)

[2] [https://dl.acm.org/doi/abs/10.1145/3342195.3387526](https://dl.acm.org/doi/abs/10.1145/3342195.3387526)

**Level of challenge:** very high

## Making Linux Audit provenance-ready (Thomas)

Systems such as SPADE [1] or XX [2] are used to build provenance graphs. However, they have not been designed to natively support the creation of directed provenance graphs. You will study the literature and propose modifications of the Linux audit subsystem to support provenance capture. You will have the opportunity to interact with the developers of SPADE.

[1] [https://hal.inria.fr/hal-01555544/document](https://hal.inria.fr/hal-01555544/document)

**Level of challenge:** high

## Deep Graph Learning to detect malicious Linux Package installation (Thomas)

SIGL [1] has been designed to detect malicious installers on Windows. It has been deployed at NEC Labs America to detect malicious behaviour during the installation of python packages. You will test the approach in the context of Linux package manager (e.g. dnf). You will re-implement the paper algorithm and build the necessary dataset to run your evaluation. You will have the opportunity to interact with the developer of SIGL.

[1] [https://arxiv.org/pdf/2008.11533.pdf](https://arxiv.org/pdf/2008.11533.pdf)

**Level of challenge:** moderate

## Self-tuning SPARK (Thomas)

**Not a security project.** We have recently been building [1, 2] an extension to SPARK
to allow the platform to self-optimise overtime (i.e. automatically associate to a workload
a good configuration). Your task will be to deploy a SPARK cluster with this extension
and to perform further benchmarking to understand how it improves performance.

[1] [https://tfjmp.org/files/publications/2020-kdd.pdf](https://tfjmp.org/files/publications/2020-kdd.pdf)

[2] [https://arxiv.org/pdf/2001.08002.pdf](https://arxiv.org/pdf/2001.08002.pdf)

[3] [https://tfjmp.org/files/publications/2019-icdcs.pdf](https://tfjmp.org/files/publications/2019-icdcs.pdf)

**Level of challenge:** high

## Capturing whole-system provenance with eBPF (Thomas)

Project already taken.

**Level of challenge:** high

## Provenance consumption over Kafka (Thomas)

This is mostly a software development project. The idea is to extend an existing software to support publication and consumption of provenance data (represented as directed acyclic graph)  over the Apache Kafka middleware.  

**Level of challenge:** moderate

## Implementing security namespaces with eBPF? (Thomas)

The concept of security namespaces was introduced by Sun et al. [1].
Recently eBPF has been extended to support the attachment of program on LSM hooks.
Would it be possible to leverage this feature to implement the scheme described by Sun et al.?

[1] [https://www.usenix.org/system/files/conference/usenixsecurity18/sec18-sun.pdf](https://www.usenix.org/system/files/conference/usenixsecurity18/sec18-sun.pdf)

[2] [https://www.kernel.org/doc/html/latest/bpf/bpf_lsm.html](https://www.kernel.org/doc/html/latest/bpf/bpf_lsm.html)

**Level of challenge:** high

## Fuzz my kernel (Sanjay and Thomas)

Fuzzing is an automated testing technique that consist of providing random value to an interface to detect unexpected behaviors leading to vulnerabilities. The project will explore the use of fuzzing tools (e.g. Syzkaller) applied to the Linux kernel. In particular looking at detecting vulnerabilities in the Linux Security Module framework and its module implementations (e.g. SELinux, AppArmor etc...). The interested student should be ready to play with C and kernel space (lots of fun!!)

**Level of challenge:** very high

## Fuzz my BPF (Sanjay and Thomas)

eBPF programs are used to instrument the kernel. They are provided from user space and executed on a virtual machine within the kernel. They go through a verification stage to verify that they cannot compromise the kernel. However, it has been shown to not be quite the case [1]. In this project, you will explore fuzzing techniques to attempt to identify means to compromise the kernel integrity and/or availability through malicious eBPF programs.

[1] [https://www.thezdi.com/blog/2020/4/8/cve-2020-8835-linux-kernel-privilege-escalation-via-improper-ebpf-program-verification](https://www.thezdi.com/blog/2020/4/8/cve-2020-8835-linux-kernel-privilege-escalation-via-improper-ebpf-program-verification)

**Level of challenge:** very high

## Loop-aware Fuzzing (Source code) for hunting buffer overflows (Sanjay)
Buffer overflows remain annoying bugs in unmanaged languages like C/C++. While the code-coverage based fuzzers, like AFL have been successful in finding many such bugs, we keep getting reports on discovery of these nugs and 0-day exploits in the wild. In this project, we aim to specifically target buffer overflow bugs with the assumption (i.e. our scope) that internally such buffers are accessed via loops and such loops are controlled by the induction variable. We will be using LLVM framework [1] to analyse the source code the application to find loops and the induction variable. We will use LLVM taint pass (DSan) [2] to check the taint information of this induction variable. Our fuzzing will involve mutating this variable as much as possible.
\[1\]: [https://llvm.org/](https://llvm.org/)
\[2\]: [https://clang.llvm.org/docs/DataFlowSanitizer.html](https://clang.llvm.org/docs/DataFlowSanitizer.html)

**Level of challenge: high**

##Loop-aware Fuzzing (binary only) for hunting buffer overflows (Sanjay)
Buffer overflows remain annoying bugs in unmanaged languages like C/C++. While the code-coverage based fuzzers, like AFL have been successful in finding many such bugs, we keep getting reports on discovery of these nugs and 0-day exploits in the wild. In this project, we aim to specifically target buffer overflow bugs with the assumption (i.e. our scope) that internally such buffers are accessed via loops and such loops are controlled by the induction variable. However, detecting loops in the absence of course code remains a hard problem. Moseley et al. [1] proposed LoopProf to identify loops entirely based on the execution trace of the application. IN this project, we use this technique to identify loops and instructions/memory locations accessed within such loops. We, then, use dynamic taintflow analysis [2] to tag each such memory locations with the taint information. Equipped with this information, we aim to develop a vuzzer (on top of an existing fuzzer VUzzer [3]) to fuzz such tainted memory locations with a higher possibility to hit the used induction variable and thereby triggering the buffer overflow attacks.

\[1\]: [https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.85.7096&rep=rep1&type=pdf](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.85.7096&rep=rep1&type=pdf)
\[2\]: [https://www.cs.columbia.edu/~vpk/research/libdft/](https://www.cs.columbia.edu/~vpk/research/libdft/)
\[3\]: [https://www.cs.vu.nl/~giuffrida/papers/vuzzer-ndss-2017.pdf](https://www.cs.vu.nl/~giuffrida/papers/vuzzer-ndss-2017.pdf)
  
**Level of challenge: high to very high**

## Compilation Based Symbolic Execution and Taintflow for better code-coverage with application to fuzzing (Sanjay)

Code-coverage remains the main strategy for modern fuzzers. Symbolic execution provides a systematic approach to enhance code-coverage (e.g. KLEE [1)]. Accordingly, we have seen several hybrid fuzzers using symbolic execution to get better coverage, e.g QSYM[2]. HOwever, symbolic execution suffers from scalability issues, making it very slow, specially when used in fuzzing. Recently Poeplau et al. [3] proposed a technique (Symbolic execution with SYMCC:Don’t interpret, compile!) which makes symbolic execution much faster. It is implemented over LLVM [4]. In this project, we explore to integrate this technique with LLVM taintflow sanitizer (DSan) [5] to further optimized the constraints to be solved, thereby making it even faster for fuzzing.
\[1\]: [https://klee.github.io/](https://klee.github.io/)
\[2\]: [https://www.usenix.org/system/files/conference/usenixsecurity18/sec18-yun.pdf](https://www.usenix.org/system/files/conference/usenixsecurity18/sec18-yun.pdf)
\[3\]: [http://www.s3.eurecom.fr/docs/usenixsec20_symcc.pdf](http://www.s3.eurecom.fr/docs/usenixsec20_symcc.pdf)
\[4\]: [https://llvm.org/](https://llvm.org/)
\[5\]: [https://clang.llvm.org/docs/DataFlowSanitizer.html](https://clang.llvm.org/docs/DataFlowSanitizer.html)

**Level of challenge: very high**


## Dynamic Analysis Assisted Binary level static Controlflow graph (CFG) and Callgraph precision improvement (Sanjay) 

Binary code analysis in challenging, yet unavoidable in several situations. In order to analyse a COTS application (e.g. 3rd party library or malware), we do need to analyse the application’s binary. Any usable program analysis technique involves the generation of CFG and callgraph and the quality of the analysis depends on the precision of the generated CFG. The main source of imprecision arises from indirect jumps (and calls via function pointers), like jmp rax or call rax In such cases, we need to do a backward analysis to find the value of rax, which is also a hard problem as many a times, it may depends on the dynamic behavior of the program or we may have to across boundaries of several functions. In this project, we propose to augment the static CFG by using a dynamic analysis to find the targets of such indirect jumps. We will investigate the use of dynamic binary instrumentation tool (e.g. Intel Pin [1]) and binary disassembler tool Ghidra [2] to implement this analysis. This is a tool development oriented project.
\[1\]: [https://software.intel.com/content/www/us/en/develop/articles/pin-a-dynamic-binary-instrumentation-tool.html](https://software.intel.com/content/www/us/en/develop/articles/pin-a-dynamic-binary-instrumentation-tool.html)
\[2\]: [https://ghidra-sre.org/](https://ghidra-sre.org/)

**Level of challenge: medium to high**

## Reducing overhead for dynamic binary instrumentation based code-coverage (Sanjay)

For binary only solution for code-coverage information (a basis for fuzzing), dynamic binary instrumentation (e.g. Intel Pin [1]) are instrumental. However, this comes with a cost as each basic block BB (or even instruction) is instrumented at runtime. This incurs a heavy cost for runtime performance. Agrawal [1] proposed the notion of super BB which is a set of BBs that are executed together in a sequence. If any BB in a super block is exercised by an input then all BBs in that super block must be exercised by the same input. By leveraging this construct, the project investigate the use of super block for code-coverage information. The idea is to compute super Block of the program statically and at runtime only instrument this super block’s entry BB. Once the execution is over, combined with the static information about the other BBs in the super block, we get the whole code-coverage for the execution. We aim to implement the above technique in a open source fuzzer.

\[1\]: [https://software.intel.com/content/www/us/en/develop/articles/pin-a-dynamic-binary-instrumentation-tool.html](https://software.intel.com/content/www/us/en/develop/articles/pin-a-dynamic-binary-instrumentation-tool.html)

\[2\]: [https://personal.utdallas.edu/~ewong/SE6367/03-Lecture/10-Hira-01.pdf](https://personal.utdallas.edu/~ewong/SE6367/03-Lecture/10-Hira-01.pdf)

 **Level of challenge: medium to high**
