#pragma once

/**
 * currentOperation Values-> 
 *                        ->  0 = No Active Operation
 *                        ->  1 = Lock/Unlock State
 *                        ->  2 = Skip Value (May Become Deprecated)
 *                        ->  3 = Revert Value (May Become Deprecated)
 *                        --  4 = Seek Value (Not Implemented Currently)
 *                        --  5 = Emergency Killer (Not Implemented Currently)
 */ 
typedef struct {
    int currentOperation;
    int fileIndex;
} SpotyIPCRamDisk;

void setupIPC(void);
SpotyIPCRamDisk* retrieveIPC(void);
void writeToIPC(int currentOP, int fileIndex);
void stopIPC(void);