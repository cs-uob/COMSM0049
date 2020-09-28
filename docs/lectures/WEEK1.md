# Week 1 (Software Vulnerabilities & Attacks I)

The material is subdivided in small videos.

Please, watch the videos and go through the reading material in your own time.

Also remember to work on the accompanying [exercises sheet](../exercises/EXERCISE1.md)




| Video                   | Links                     |        Reading Material                                                                                                                                                                                      |
|-------------------------|---------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Introduction to Memory corruption & Assembly                 | [video](https://web.microsoftstream.com/video/480ff768-fe37-42fb-a0e4-44ab65a5f92c) [pdf](../slides/week1/SysSec-refresh.pdf) [ppt](../slides/week1/SysSec-refresh.pptx) |  Quick Intro x86-64 Intel Assembly [here](https://software.intel.com/content/www/us/en/develop/articles/introduction-to-x64-assembly.html)                                                                                                                                                                                         |
| Introduction to Stack Overflow                 | [video](https://web.microsoftstream.com/video/2f9cb60b-36f1-4da5-967f-10a7c2a345f3) [pdf](../slides/week1/2-SysSec-BoFIntro.pdf) [ppt](../slides/week1/2-SysSec-BoFIntro.pptx) | Smashing the stack for fun & profit [Classic paper here](http://phrack.org/issues/49/14.html)                                                                                                                                                                                         |
| Introduction to Format String | [video](https://web.microsoftstream.com/video/269ada32-d968-42cf-92e3-ff5dff50e119) [pdf](../slides/week1/intro-formatString-UoB.pdf) [ppt](../slides/week1/intro-formatString-UoB.pptx) | N/A                                  |
| What is a race condition?                 | [video](https://web.microsoftstream.com/video/d4077181-36ba-4d78-b45f-5d27891571f6) [pdf](../slides/week1/lecture1.pdf) [ppt](../slides/week1/lecture1.pptx) | N/A  |
| Race condition: Examples Access System Call                | [video](https://web.microsoftstream.com/video/c675aac3-ac9f-4fff-b194-31637972508d) [pdf](../slides/week1/lecture2.pdf) [ppt](../slides/week1/lecture2.pptx) | [access man page](https://man7.org/linux/man-pages/man2/access.2.html) |
| Race condition: Examples Reference Monitor                 | [video](https://web.microsoftstream.com/video/1aedcb22-7836-4be9-887b-ccfe1e92768c) [pdf](../slides/week1/lecture3.pdf) [ppt](../slides/week1/lecture3.pptx) | Timothy Fraser, N. A. I. "LOMAC: MAC you can live with." USENIX Annual Technical Conference. 2001. [pdf](https://www.usenix.org/legacy/event/usenix01/freenix01/full_papers/fraser/fraser.pdf) Watson, Robert NM. "Exploiting Concurrency Vulnerabilities in System Call Wrappers." WOOT .2007. [pdf](https://www.usenix.org/legacy/event/woot07/tech/full_papers/watson/watson.pdf) |
| Race condition: Examples Dirty COW                 | [video](https://web.microsoftstream.com/video/01be89da-3c3d-4954-b1db-69328c8cfe30) [pdf](../slides/week1/lecture4.pdf) [ppt](../slides/week1/lecture4.pptx) | [mmap man page](https://man7.org/linux/man-pages/man2/mmap.2.html) [madvise man page](https://man7.org/linux/man-pages/man2/madvise.2.html) [Dirty COW website](https://dirtycow.ninja/) |
