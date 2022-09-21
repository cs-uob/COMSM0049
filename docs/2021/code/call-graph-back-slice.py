# For Ghidra Scripting tutorial purpose.
#  Copy this file into ghidra_scripts directory (the one which is configured during installation of ghidra, where ghidra can import it.) 
#@author Sanjay Rawat 
#@category _NEW_
#@keybinding 
#@menupath 
#@toolbar 

#GHIDRA IMPORTS ###############################################################
#from ghidra.program.model.block import *
#from ghidra.program.model.listing import * 
import ghidra.program.model.block as BL
from ghidra.program.model.lang.OperandType import SCALAR, REGISTER
import generic.continues as CNT
import ghidra.app.util.bin.ByteProvider
import ghidra.app.util.bin as UBIN
import ghidra.app.util.bin.format.elf as ELF
import java.io as IO
import ghidra.program.util as PUTIL
####################################################################################
from collections import deque
import struct
import shutil
import os
import sys
import copy
###########################################################################################

######## Ghidra global definitions ##############
sbm=BL.SimpleBlockModel(currentProgram, False)
ref_manager=currentProgram.getReferenceManager()
func_manager=currentProgram.getFunctionManager()
#################################################


def get_xref_information(function):
    callers = set() #set of the callers of the function
    print "[*] Function: %s"%(function.getName(),)
    addr_view=function.getBody()
    current_function_address = function.getEntryPoint()
    ref_to_current=ref_manager.getReferencesTo(current_function_address)
    #Now you have a refernce manager. Search in the API help doc to find more about it and methods you can use. You want to do the following:
    #1. iterate over these references (Note that  it uses hasNext() 
    #2.then check if the reference type is call (you are only interested in calls)
    #3.get the address of this reference (from address: the reference is like a edge from -> to)
    #4. using func_manager, get the function that contains that address.
    #5. if that is not none, this is one of the callers of the function. add it to callers.

            
    print "Done. Found %d callers"%(len(callers),)
    return callers.copy()

def get_call_graph():
    print("===== Compute Call Graph Slice =====")
    vul_func=['strcpy', 'memcpy']# list of vulnerable functions we want to target
    # Using the get-xref-information() function, start with a source function and
    # get it's callers. Iterate over this list of callers and
    # repeat the process. This will give you a backward slice starting from
    # the 1st function until main()
    # in this example, we will look for strcpy function and then start a backward callgraph slice
    #Steps to code
    #1. get a function iterator (use func_manager). you will use this iterator to get functions.
    ##Pro Tip: when iterating with while loop, also include condition (and monitor.isCancelled() != True). This will allow you to cancel the script in case of some non-termnating loop!
    #2. for each function, get the name and check if it in vul_name (for the lab, you can just search for strcpy)
    #3. get the callers of this function. use partially implemented function get_xref_information.
    #4. print the name and address (as hex like 0xaabbccdd). This will allow you to double-click them in ghidra output window to nevigate to those functions.
    #4A. (optional for more complete slice) for each caller, repeat the above process (i.e. find its callers) untill yoo reach the main function.
    #5. Load the dnstracer binary in the ghidra by creating a new project and analyzing the new binary.
    #6. Open the Scriptmanager window and select the script. run it.
    #7. You will find that caller of strcpy is printablename function, which is where the vulnerability was found in the dnstracer.

    

if __name__ == '__main__':
    #get_func_attribute(functionAttrPath,file_name)
    
    exe_format=currentProgram.getExecutableFormat()
    #if idc.GetLongPrm(idc.INF_FILETYPE) == idc.FT_ELF:
    if 'ELF' in exe_format:
        print("Linux ELF File")

        print("-------- Starting --------------------")

        # generate call graph file, ok
        get_call_graph()
                


        print("Analysis finished")
    else:
        print("not suitable file type")
