# Exercises sheet 2

1. Define the principle of least privileges.
2. Why Linux basic discretionary access control scheme does not adhere to this principle?
3. Describe the difference between DAC and MAC.
4. Explain the Bell-LaPadula Model and the Biba Model.
5. Why a reference monitor is required?
6. Research how basic access control is implemented in Windows.
7. Explain why a system such as SELinux is necessary.
8. In normal Linux DAC access check are only performed on file open operation.
However, in SELinux, they are checked on any operation to the file.
Explain why this change was made.
9. Look at the [Yama LSM documentation](https://www.kernel.org/doc/html/v4.15/admin-guide/LSM/Yama.html) and its [code](https://github.com/torvalds/linux/blob/master/security/yama/yama_lsm.c). Try to explain how it works.
10. Read [the paper](https://tfjmp.org/files/publications/2020-ndss.pdf) associated with the [4th video](https://web.microsoftstream.com/video/f368875b-2fd4-4894-89af-7a4641e57503).
Slide number 8 introduces a number of requirements an IDS should fulfil.
Discuss how well you think the solution proposed in the paper fulfil those requirements.
Do you have any suggestion to improve the system?
