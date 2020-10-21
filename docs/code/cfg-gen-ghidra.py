
#TODO Add User Code Here
#from ghidra.program.model.block import *
#from ghidra.program.model.listing import * 
import ghidra.program.model.block as BL
from ghidra.program.model.lang.OperandType import SCALAR, REGISTER
import ghidra.app.script as SCR #import println, printf
from collections import deque
import timeit
import sys
import struct
import pickle
import os
import gc
## global variables ##
backedges=list()# list of the backedges, preseorented as tuple (srcBB,dstBB)
edges=dict() # dictionary to keep edges weights key=(srcBB,destBB), value=weight
weights=dict() # dictionary to keep weights of the BBs. key=BBaddress, value=weight. Weight is calculated as w=1.0/reaching prob.
#where reaching prob(B) = Sum_i \in Pred(B){ weight_edge(i,B)*weight(i)}
# All of the backedges have 0 weights, i.e. they have no influence on the weight of the target BB.
#GS=SCR.GhidraScript()
sbm=BL.BasicBlockModel(currentProgram)
image_base=currentProgram.getImageBase().getOffset()
def dead(msg):
    #print "[*] %s"% msg
    printf("[*] %s\n", msg)
    sys.exit(0)


def computeCFG(function):
    ''' 
    Note: Ghidra memory model creates references for the edges. In doing so, it creates references for CALL also, which means that if we use getDestinations() or getSources(), we also get edges
    corresponding to calls and indirection (jumping to jumptable e.g.). In particular, within a function, even if we try to get the incoming edges for root node, we get references to all the callers
    that call this fuunction. So, we neglect such calls by using getFlowType().isCall() and getFlowType().isIndirect().

    '''
    total=0 #counts the total blocks in this function
    BBIterator=sbm.getCodeBlocksContaining(function.getBody(),monitor)
    while BBIterator.hasNext():
        block=BBIterator.next()
        total = total+1
        print "BB: 0x%x" % block.getFirstStartAddress().getOffset()
        dest=block.getDestinations(monitor)
        while (dest.hasNext()):
	    dbb = dest.next();
            if dbb.getFlowType().isCall()== True or dbb.getFlowType().isIndirect()==True:
                continue
            print "\t[*] 0x%x type: %s" % (dbb.getDestinationAddress().getOffset(),dbb.getFlowType().getName())
             

def getLength(function):
    '''
    given a function, the function returns the number of basic blocks in it.
    
    '''
    count=0
    blocks=sbm.getCodeBlocksContaining(function.getBody(), monitor)
    while (blocks.hasNext()):
        bb = blocks.next();
	count +=1
    return count


def main():
    ''' main function
    '''
    gc.enable()
    start=timeit.default_timer()
    clist=currentProgram.getListing()
    fweight=dict()
    total_bb=0
    total_func=0
    print "started analysis..."
    func_manager=currentProgram.getFunctionManager()

    #let's iterate over all the functions
    func_iter=clist.getFunctions(True)
    while (func_iter.hasNext() and monitor.isCancelled() != True):
        function=func_iter.next()
        if function.isThunk()==True or function.isExternal()==True:
            continue
        bb_count=getLength(function)
        if bb_count <= 1:
            continue
        total_bb= total_bb + bb_count
        total_func += 1
        #print "In: %s"%function.getName()
        printf("In: %s\n",function.getName())
        computeCFG(function)
        print "[*] done...."
    stop=timeit.default_timer()
    printf("[*] Total time: %5.5f", stop-start)
    printf("total functions analysed: %d\n total Basic blocks analysed: %d\n", total_func,total_bb)
    #print "[*] Total time: ", stop-start
    #print " total functions analysed: %d"% total_func
    #print " total Basic blocks analysed: %d"% total_bb

if __name__ =="__main__":
    main()
