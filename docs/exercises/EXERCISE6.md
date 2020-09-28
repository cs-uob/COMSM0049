# Exercises sheet 6

1. Check the slide 4 of Week 6 - video 1. Based on past lectures, design conceptually the steps necessary to deploy a rootkit.
2. Explain how TPM could be used to detect rootkits.
3. Explain how [LoadPin](https://www.kernel.org/doc/html/latest/admin-guide/LSM/LoadPin.html) and [lockdown](https://thenewstack.io/linux-kernel-finally-gets-its-lockdown/) LSMs may help prevent rootkit installation.
4. Discuss how the LSM framework (see previous lecture) could be used to prevent the installation of a kernel rootkit (e.g., see [kernel_read_file hook](https://github.com/torvalds/linux/blob/master/include/linux/lsm_hooks.h#L639) and [kernel_post_read_file hook](https://elixir.bootlin.com/linux/latest/source/include/linux/lsm_hooks.h#L645)). In your opinion would this represent a perfect solution?
5. What is the **threat model** for a single sign-on service within Trustzone on an Android device?
6. What is the **Trusted Computing Base** for a single sign-on service within Trustzone on an Android device?
7. Compare and contract Intel SGX and ARM Trustzone.
