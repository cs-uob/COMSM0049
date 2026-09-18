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

def get_function_information():
    file_content_dict = {}
    function_list = []
    # Get the current program's function names
    function = getFirstFunction()
    while function is not None:
        monitor.checkCanceled()
        addr_view=function.getBody()
        func_min=addr_view.getMinAddress().getOffset()
        func_max=addr_view.getMaxAddress().getOffset()
        func_name=function.getName()
        #print "name %s, entry: 0x%x, Min: 0x%x, Max: 0x%x "%(function.getName(),function.getEntryPoint().getOffset(),func_min, func_max)
        function=getFunctionAfter(function)

def get_xref_information(function):
    #print("-----------------------------------------get_xref_information-------------------------------------------------")
    callers = set() #set of the callers of the function
    print "[*] Function: %s"%(function.getName(),)
    addr_view=function.getBody()
   # func_min=addr_view.getMinAddress().getOffset()
    #func_max=addr_view.getMaxAddress().getOffset()
    #print "name %s, entry: 0x%x, Min: 0x%x, Max: 0x%x "%(function.getName(),function.getEntryPoint().getOffset(),func_min, func_max)
    current_function_address = function.getEntryPoint()
    #location=PUTIL.FunctionSignatureFieldLocation(function.getProgram(),current_function_address)
    ref_to_current=ref_manager.getReferencesTo(current_function_address)
    while(ref_to_current.hasNext()):
        ref=ref_to_current.next()
        if ref.getReferenceType().isCall()==True:
            callsite_addr=ref.getFromAddress()
            caller_func=func_manager.getFunctionContaining(callsite_addr)
            if caller_func==None:
                continue
            callers.add(caller_func)
            caller_BB=sbm.getCodeBlocksContaining(callsite_addr, monitor)
            #print "\t callsite: 0x%x, caller BB len %d"%(callsite_addr.getOffset(),len(caller_BB))
            #print "\t caller: %s (0x%x), BB: 0x%x, callsite: 0x%x"%(caller_func.getName(), caller_func.getEntryPoint().getOffset(), caller_BB[0].getFirstStartAddress().getOffset(),callsite_addr.getOffset())    
            
    print "Done. Found %d callers"%(len(callers),)
    return callers.copy()

def get_call_graph():
    print("===== Compute Call Graph Slice =====")
    # Using the get-xref-information() function, start with a source function and
    # get it's callers. Iterate over this list of callers and
    # repeat the process. This will give you a backward slice starting from
    # the 1st function until main()
    # in this example, we will look for strcpy function and then start a backward callgraph slice.
    func_iter=func_manager.getFunctions(True)
    while(func_iter.hasNext() and monitor.isCancelled() != True):
        function=func_iter.next()
        fname=function.getName()
        if 'strcpy' in fname:
            print "[*] Found strcpy.."
            break
    #lets get back all the callers of the function
    caller=get_xref_information(function)
    print "[*] callers of strcpy:"
    for cfun in caller:
        print '{0:s} - 0x{1:x}'.format(cfun.getName(),cfun.getEntryPoint().getOffset())
    

if __name__ == '__main__':
    #get_func_attribute(functionAttrPath,file_name)
    
    exe_format=currentProgram.getExecutableFormat()
    #if idc.GetLongPrm(idc.INF_FILETYPE) == idc.FT_ELF:
    if 'ELF' in exe_format:
        print("Linux ELF File")
        # config all the paths
        #file_path=currentProgram.getExecutablePath()
        #base,file_name=os.path.split(file_path)
        #disassembledFileName = file_name

        print("-------- Starting --------------------")
        

        # generate xref file, ok
        #get_xref_information()

        # generate call graph file, ok
        get_call_graph()
                

        # generate function information
        #get_function_information()

        print("Analysis finished")
    else:
        print("not suitable file type")
