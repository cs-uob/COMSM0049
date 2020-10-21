# Lab 6

## Fuzzing with AFL to detect Info Leak Attack

In this lab, we will learn to fuzz using AFL-- an open source fuzzer from google. We have already seen the technique behind AFL in our lecture video. Now, lets see it in action!

We will also explore a shortcoming of fuzzing, i.e. detection of information leak. Information leak is a type of buffer overread bug. Read [this](https://en.wikipedia.org/wiki/Buffer_over-read). The main issue is that such vulnereabilities do not result in any observable side-effects so as to enable fuzzing to detect it (WHY?). [Hearblead](https://en.wikipedia.org/wiki/Heartbleed) is a type of such vulneerability that made news in 2014. We will try to make fuzzing detect this vulnerability in this lab (seems interesting?)

Current compilers have a feature called sanitizer that are used as memory guard checks by statically instrumenting memory at different level of granularity. One of such sanitizers is called AddressSanitizer (ASan). This sanitizer is used to detected out-of-bound memory access. Which means that when we allocate memory, say on heap, it puts a "red-zone" around the allocated buffer. So, when at runtime, if we overwrite/read the buffer, we touch these red-zones, thereby aborting the program. [wiki for more refs](https://en.wikipedia.org/wiki/AddressSanitizer)

I have implemented a simple version of heartbleed vulnerability. Check [heartbleed.c](../code/heartbleed.c). Read the comments in the code to find out what is the bug and how it can be triggered. Our goal for the lab is to find the bug via fuzzing. You will need a seed input to start fuzzing. use [heartbeat_normal.bin](../code/heartbeat_normal.bin) as seed input. You can use your Ubuntu 64-bit OS.
Steps:

1. Download [AFL] (https://github.com/google/AFL). you can directly download the zip.
2. Unzip it. Enter the root directory (you can rename it as AFL).
3. Read README.md to find instruction to compile it. (note: you need not to install it.)
4. you will see new binaries being created. We are interested in 2 of them-- afl-fuzz and afl-gcc. type following to see if they are working:

		$ ./afl-fuzz --help
		$ ./afl-gcc --version

5. create these subdirectories in the root directory
		
		$ mkdir temp-in
		$ mkdir temp-out
		$ mkdir test-bins

6. Copy the above seed input into temp-in directory.
7. Compile heartbleed.c as follows:

		./afl-gcc <path to>/heartbleed.c -o test-bins/heartbleed

8. Run AFl fuzzer as follows:

		./afl-fuzz -i temp-input/ -o temp-out/ test-bins/heartbleed @@

9. you will (most probably) see certain warning/suggestions from AFL, like below (and what I did):

		sanjay@sanjay-lap:~/tools/AFL$ ./afl-fuzz -i temp-input/ -o 		temp-out test-bins/heartbleed @@
		afl-fuzz 2.56b by <lcamtuf@google.com>
		[+] You have 8 CPU cores and 3 runnable tasks (utilization: 38%).
		[+] Try parallel jobs - see /usr/local/share/doc/afl/parallel_fuzzing.txt.
		[*] Checking CPU core loadout...
		[+] Found a free CPU core, binding to #0.
		[*] Checking core_pattern...
		
		[-] Hmm, your system is configured to send core dump notifications to an
		    external utility. This will cause issues: there will be an extended delay
		    between stumbling upon a crash and having this information relayed to the
		    fuzzer via the standard waitpid() API.
		    To avoid having crashes misinterpreted as timeouts, please log in as root
		    and temporarily modify /proc/sys/kernel/core_pattern, like so:
		
		    echo core >/proc/sys/kernel/core_pattern
		
		[-] PROGRAM ABORT : Pipe at the beginning of 'core_pattern'
		         Location : check_crash_handling(), afl-fuzz.c:7316
		
		sanjay@sanjay-lap:~/tools/AFL$ echo core | sudo tee /proc/sys/kernel/core_pattern
		[sudo] password for sanjay: 
		core
		sanjay@sanjay-lap:~/tools/AFL$ ./afl-fuzz -i temp-input/ -o temp-out test-bins/heartbleed
		afl-fuzz 2.56b by <lcamtuf@google.com>
		[+] You have 8 CPU cores and 3 runnable tasks (utilization: 38%).
		[+] Try parallel jobs - see /usr/local/share/doc/afl/parallel_fuzzing.txt.
		[*] Checking CPU core loadout...
		[+] Found a free CPU core, binding to #0.
		[*] Checking core_pattern...
		[*] Checking CPU scaling governor...
		
		[-] Whoops, your system uses on-demand CPU frequency scaling, adjusted
		    between 390 and 4101 MHz. Unfortunately, the scaling algorithm in the
		    kernel is imperfect and can miss the short-lived processes spawned by
		    afl-fuzz. To keep things moving, run these commands as root:
		
		    cd /sys/devices/system/cpu
		    echo performance | tee cpu*/cpufreq/scaling_governor
		
		    You can later go back to the original state by replacing 'performance' with
		    'ondemand'. If you don't want to change the settings, set AFL_SKIP_CPUFREQ
		    to make afl-fuzz skip this check - but expect some performance drop.
		
		[-] PROGRAM ABORT : Suboptimal CPU scaling governor
		         Location : check_cpu_governor(), afl-fuzz.c:7378
		
		sanjay@sanjay-lap:~/tools/AFL$ export AFL_SKIP_CPUFREQ=1
		sanjay@sanjay-lap:~/tools/AFL$ ./afl-fuzz -i temp-input/ -o temp-out test-bins/heartbleed
		afl-fuzz 2.56b by <lcamtuf@google.com>
		[+] You have 8 CPU cores and 2 runnable tasks (utilization: 25%).
		[+] Try parallel jobs - see /usr/local/share/doc/afl/parallel_fuzzing.txt.
		[*] Checking CPU core loadout...
		[+] Found a free CPU core, binding to #0.
		[*] Checking core_pattern...
		[*] Setting up output directories...
		[*] Scanning 'temp-input/'...
		[+] No auto-generated dictionary tokens to reuse.
		[*] Creating hard links for all input files...
		[*] Validating target binary...
		[*] Attempting dry run with 'id:000000,orig:heartbeat_normal.bin'...
		[*] Spinning up the fork server...
		[+] All right - fork server is up.
		
10. At this time, you will enter into the TUI of AFL. Let you fuzzer run for 10-15 min. While it is running, read docs/status_screen.txt to know what that TUI means and how you interprete fuzzing progress. After that see if you are getting any crash. In this case, you will not find any!! Remember it is a information leak bug, which will not result in a crash.
11. Stop the fuzzer (ctrl C). Check temp-out directory to find more about fuzzing results, like queue (what inputs were generated by fuzzer) and crash (if any!).
12. Now, lets try making program crash on overread, by using sanitizer. Compile the heartbleed.c again with the following options.

		./afl-gcc -m32 -fsanitize=address <path to>/heartbleed.c -o test-bins/heartbleed-asan

13. Notice two extra options. *-fsanitize=address* to enable address sanitizer and *-m32* to make it 32-bit as ASan allocates crazy amount of VA, which creates problem at times.
14. Run fuzzer as:

		./afl-fuzz -i temp-in/ -o temp-out test-bins/heartbleed-asan @@

15. Most probably, you will again see the follwing warnings/suggestions:

		afl-fuzz 2.56b by <lcamtuf@google.com>
		[+] You have 8 CPU cores and 3 runnable tasks (utilization: 38%).
		[+] Try parallel jobs - see /usr/local/share/doc/afl/parallel_fuzzing.txt.
		[*] Checking CPU core loadout...
		[+] Found a free CPU core, binding to #0.
		[*] Checking core_pattern...
		[*] Setting up output directories...
		[+] Output directory exists but deemed OK to reuse.
		[*] Deleting old session data...
		[+] Output dir cleanup successful.
		[*] Scanning 'temp-input/'...
		[+] No auto-generated dictionary tokens to reuse.
		[*] Creating hard links for all input files...
		[*] Validating target binary...
		[*] Attempting dry run with 'id:000000,orig:heartbeat_normal.bin'...
		[*] Spinning up the fork server...
		
		[-] Whoops, the target binary crashed suddenly, before receiving any input
		    from the fuzzer! Since it seems to be built with ASAN and you have a
		    restrictive memory limit configured, this is expected; please read
		    /usr/local/share/doc/afl/notes_for_asan.txt for help.
		
		[-] PROGRAM ABORT : Fork server crashed with signal 6
		         Location : init_forkserver(), afl-fuzz.c:2213
	
16. Run the fuzzer again with an extra option *-m*:

		./afl-fuzz -m 600 -i temp-in/ -o temp-out test-bins/heartbleed-asan @@

17. fuzzer should start now..

		sanjay@sanjay-lap:~/tools/AFL$ ./afl-fuzz -m 600 -i temp-input/ -o temp-out/ test-bins/heartbleed-asan @@
		afl-fuzz 2.56b by <lcamtuf@google.com>
		[+] You have 8 CPU cores and 3 runnable tasks (utilization: 38%).
		[+] Try parallel jobs - see /usr/local/share/doc/afl/parallel_fuzzing.txt.
		[*] Checking CPU core loadout...
		[+] Found a free CPU core, binding to #0.
		[*] Checking core_pattern...
		[*] Setting up output directories...
		[+] Output directory exists but deemed OK to reuse.
		[*] Deleting old session data...
		[+] Output dir cleanup successful.
		[*] Scanning 'temp-input/'...
		[+] No auto-generated dictionary tokens to reuse.
		[*] Creating hard links for all input files...
		[*] Validating target binary...
		[*] Attempting dry run with 'id:000000,orig:heartbeat_normal.bin'...
		[*] Spinning up the fork server...
		[+] All right - fork server is up.
		    len = 10, map size = 25, exec speed = 4314 us
		[+] All test cases processed.
		
		[+] Here are some useful stats:
		
		    Test case count : 1 favored, 0 variable, 1 total
		       Bitmap range : 25 to 25 bits (average: 25.00 bits)
		        Exec timing : 4314 to 4314 us (average: 4314 us)
		
		[*] No -t option specified, so I'll use exec timeout of 40 ms.
		[+] All set and ready to roll!
		
18. Now, check the crash field in TUI. In the crash field, you will see number of crashes. Let the fuzzer run for 15-20 min. Number of unique crashes should increase (max will be ~5). One of them should be your heartbleed bug (hurray :D ).
19. Stop the fuzzer and check the temp-out/crashes directory. there should be few inputs, corresponding to crashes.
20. for each of those inputs, run the following command:

		$ ./test-bins/heartbleed-asan <input>

21. One fo such input should give you the following output:

			==28681==ERROR: AddressSanitizer: heap-buffer-overflow on address 0xf59007b7 at pc 0xf7a5b8be bp 0xffffce28 sp 0xffffc9f8
		READ of size 32512 at 0xf59007b7 thread T0
		    #0 0xf7a5b8bd  (/usr/lib32/libasan.so.4+0x778bd)
		    #1 0x565565e4 in memcpy /usr/include/bits/string_fortified.h:34
		    #2 0x565565e4 in process_heartbeet ../../temp/labs3-heartbl/heartbleed.c:54
		    #3 0x565560d1 in main ../../temp/labs3-heartbl/heartbleed.c:96
		    #4 0xf7823e90 in __libc_start_main (/lib32/libc.so.6+0x18e90)
		    #5 0x565562cf  (/home/sanjay/tools/AFL/test-bins/heartbleed-asan+0x12cf)
		    
		    
	NOTE: the way AFL names crashing inputs, sometimes the above step may not show you the same output, because the input path was not interpreted properly. in that case, either use quotes or rename the inputs to some simpler names.
	
