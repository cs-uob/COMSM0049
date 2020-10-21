# Lab 5

## Static Analysis Lab with Ghidra
In this lab, we will develop a *trivial* static binary code analyzer that will serach for known buffer overflow prone library functions like *strcpy*, *memcpy* etc. However, we will construct a backward callgraph based slice to know which functions call such vulnerable functins (because buffer overflow will affect these functions). Most of the vulnerabilities are found in the functions which call these vulnerable functions. Even though we call it a trivial scanner, you should know that several early code scanners were actually like the one we will build [see this paper](http://www.cs.virginia.edu/~evans/pubs/ieeesoftware.pdf). 
As a proof of concept, we will analyze a utility called *dnstracer* to find a known vulnerability [CVE-2017-9430](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2017-9430). Download this tool [dnstracer](../code/dnstracer-1.9.tar.gz). Extract and build the program (from the root directory, use commands *./configure* and *make*). This will create a binary dnstracer. we will use this binary later on for analysis.

### Steps for Ghidra Scripting
1. Download Ghidra and unzip it (I assume you have already done this!).
2. In the video, I showed you how to launch Ghidra and create a project. I also showed you how to open ScriptManager. In the ScriptManager window, there is a *list* sign on the right top corner menu (script directories). Click on it. In the subsequent window, you can either see a Ghidra_script directory added already and if not, create a ghidra_script directory in your home directory and add same via ScriptManager.
3. Download [call-graph-back-slice.py](../code/call-graph-back-slice.py) sample script in that direcroty.
4. This call-graph-back-slice.py file conatains all the instructions to follow for coding.
5. After completing the code, load the newly genrated dnstracer binary in the opened project and double-click it. Ghidra will ask to analyze it. Once done, open the ScriptManager and select your scripting file. Run it and check the output.
