
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <switch.h>
#include "mainLoop.H"

using namespace std;

extern "C" {
    u32 __nx_applet_type = AppletType_None;

    #define HEAP_SIZE 0xA0000
    size_t nx_inner_heap_size = HEAP_SIZE;
    char   nx_inner_heap[HEAP_SIZE];

    void __libnx_initheap(void)
    {
    	void*  addr = nx_inner_heap;
    	size_t size = nx_inner_heap_size;

    	extern char* fake_heap_start;
    	extern char* fake_heap_end;

    	fake_heap_start = (char*)addr;
    	fake_heap_end   = (char*)addr + size;
    }

    void __attribute__((weak)) __appInit(void)
    {
        svcSleepThread(5e+8L);
        Result rc;

        rc = smInitialize();
        if (R_FAILED(rc))
           fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));

        rc = hidInitialize();
        if (R_FAILED(rc))
          fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));

	    rc = hidsysInitialize();
	    if (R_FAILED(rc))
           fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));

        rc = timeInitialize();
        if (R_FAILED(rc))
            fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_Time));

        rc = fsInitialize();
        if (R_FAILED(rc))
            fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));

        //SDL_Init(SDL_INIT_AUDIO);

        fsdevMountSdmc();
    }

    void __attribute__((weak)) __appExit(void)
    {
        fsdevUnmountAll();
        fsExit();
        timeExit();
        hidExit();
    	hidsysExit();
        smExit();
        socketExit();
    }
}

int main()
{
    svcSleepThread(9e+8L);
    
    Result rc = mainLoop();
    
	return rc;
}