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

void reloadIPC(ios_base::openmode iosMode);
void stopIPC(void);

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
    reloadIPC(ios::out);
    if (isRamOpened) 
    {
        writeRamValues(0x0, 0x0);
        stopIPC();
    } 
    else 
    {
        fatalThrow(MAKERESULT(Module_HomebrewAbi, LibnxError_InitFail_FS));
    }
}

void reloadIPC(ios_base::openmode iosMode) 
{
    f.open("sdmc:/tempIPCServer/IPC/DONOTDELETE/serverSPOTY/ramActive.tmpdsk", iosMode);
    if (f.good()) 
    {
        isRamOpened = true;
    } 
    else 
    {
        isRamOpened = false;
    }
}

SpotyIPCRamDisk* retrieveIPC(void) 
{
    reloadIPC(ios::in);
    if(isRamOpened) 
    {
        updateRamValues();
        stopIPC();
        return &mainRamDisk;
    }
    else 
    {
        svcSleepThread(5e+7L);
        return retrieveIPC();
    }
}

void writeToIPC(int currentOP, int fileIndex) 
{
    reloadIPC(ios::out);
    if(isRamOpened) 
    {
        writeRamValues(currentOP, fileIndex);
        stopIPC();
    }
    else 
    {
        svcSleepThread(5e+7L);
        writeToIPC(currentOP, fileIndex);
    }
}

void stopIPC(void) 
{
    f.close();
    isRamOpened = false;
}

