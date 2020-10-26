# Lab 4 - ROP based Exploitation

Watch the [video](https://web.microsoftstream.com/video/2463603d-0f94-4e62-863b-f1c40918f072) for details on this lab. Concrete steps are also outlined below.

<iframe width="640" height="360" src="https://web.microsoftstream.com/embed/video/2463603d-0f94-4e62-863b-f1c40918f072?autoplay=false&amp;showinfo=true" allowfullscreen style="border:none;"></iframe>

**Aim**: open a port on the victim machine using netcat tool that returns a shell (reverse shell exploit).

Setting up the environment:  

0. download ROPGadget and install from: [URL](https://github.com/JonathanSalwan/ROPgadget). Its a github repo, so you can either clone it or (suggested) simple use "Download zip" option.  
1. Download netcat (the latest release of netcat that comes pre-installed in Ubunut has removed a particular option (-e) that we need.
Having said that, official netcat release still shipped with that option! So, we are not completely artificial ;). [URL:](https://sourceforge.net/projects/netcat/). However, the same is also avaialble [here](../code/nc071.tar.gz).
2. untar it and build-- `./configure` and `make` command (**do not do** *make install*!)
3. move `src/netcat` to `/tmp/--` `cp src/netcat /tmp/nc` (check if the binary is working as expected `/tmp/nc --help`).
4. Compile [vuln3.c](../code/vuln3.c) as `gcc -fno-stack-protector -m32 -static vuln3.c -o vuln3-32`
5. Use the same trick we saw in the lecture video to find the offsets where the input starts overwriting the saved return addr.
6. Start populating the supplied ROP exploit python script: [exploit-nc-skeleton.py](../code/exploit-rop-nc-skeleton.py). For this step, you use ROPGadget.py to find ROP chain: 

		./ROPgadget.py --binary vuln3-32 --ropchain > out-rop.txt
	\[Note: in case your ROPgadget reports that it could not find a chain on this binary, you can use [this binary](../code/vuln3-32). In worst case, if that still does not work, use VM that you used in your lab 1 -2 and repeat the whole steps\]
7. You have helper files to consult [exploit-nc.py](../code/exploit-nc.py) and [exploit-rop.py](../code/exploit-rop.py)
8. Once successful, open another terminal and type:  
```
$/tmp/nc 127.0.0.1 5678
pwd
```
