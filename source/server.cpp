#include <stdio.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <string>
#include <unistd.h>
#include <switch.h>
#include "server.h"
#include "mainLoop.h"

using namespace std;

SpotyIPCRamDisk mainRamDisk;
fstream f;

bool isRamOpened = false;

void updateRamValues() 
{
    f.seekp(0);
    char c = f.get();
    char fi = f.get();
    mainRamDisk = {c, fi};
}

void writeRamValues(int c = -1, int fi = -1) 
{
    if(c != -1) 
    {
        f.seekp(0);
        f.put(c);
    }
    if(fi != -1) 
    {
        f.seekp(1);
        f.put(fi);
    }
}

void setupIPC(void) 
{
    f.open("sdmc:/tempIPCServer/IPC/DONOTDELETE/serverSPOTY/ramActive.tmpdsk", ios::in | ios::out);
    if (f.good()) 
    {
        writeRamValues(0x0, 0x0);
        isRamOpened = true;
    } 
    else 
    {
        fatalThrow(MAKERESULT(Module_HomebrewAbi, LibnxError_InitFail_FS));
        isRamOpened = false;
    }
}

SpotyIPCRamDisk* retrieveIPC(void) 
{
    updateRamValues();
    return &mainRamDisk;
}

void writeToIPC(int currentOP, int fileIndex) 
{
    if(isRamOpened) 
    {
        writeRamValues(currentOP, fileIndex);
    }
}

void stopIPC(void) 
{
    f.close();
}

