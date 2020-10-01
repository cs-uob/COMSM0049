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

## Capturing whole-system provenance with eBPF (Thomas)

Project already taken.

**Level of challenge:** high

## Provenance consumption over Kafka (Thomas)

This is mostly a software development project. The idea is to extend an existing software to support publication and consumption of provenance data (represented as directed acyclic graph)  over the Apache Kafka middleware.  

**Level of challenge:** moderate

## Implementing security namespaces with eBPF? (Thomas)

The concept of security namespaces was introduced by Sun et al. [1].
Recently eBPF have been extended to support the attachment on program on LSM hooks.
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
