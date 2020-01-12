
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include "switch.h"
#include "led.h"
#include "mp3.h"

using namespace std;

extern "C" {
    u32 __nx_applet_type = AppletType_None;

    #define HEAP_SIZE 0x80000
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
    }
}

void initSDLAudio(void) 
{
    SDL_Init(SDL_INIT_AUDIO);
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}

vector<string> musics = vector<string>();
u64 playid = 0;

int main()
{
    svcSleepThread(2e+9L);
    //initSDLAudio();
    /*#ifdef __APPLET__
        consoleInit(NULL);
    #endif*/

    playMp3("/switch/tinfoil/music.mp3");
    
	while(appletMainLoop()) 
	{
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

        if (kHeld & (KEY_ZR | KEY_ZL | KEY_R | KEY_A)) 
        {
            startLed(2);

            musics.clear();

            DIR* dir = opendir("sdmc:/spoty-musics/");
            struct dirent* ent;
            if (dir != NULL) 
            {
                while ((ent = readdir(dir)))
                {
                   musics.push_back(ent->d_name);
                   /*#ifdef __APPLET__
                       printf("Music: %s\n", ent->d_name);
                   #endif*/
                }
                closedir(dir);
                startLed(0);
            } 
            else 
            {
                /*#ifdef __APPLET__
                printf("DIRECTORY NOT FOUND");
                #endif*/
                startLed(3);
            }
        }



        /*#ifdef __APPLET__
            consoleUpdate(NULL);
        #else*/
            svcSleepThread(1e+7L);
        //#endif
	}

    //shutdownLed();
    /*#ifdef __APPLET__
        consoleExit(NULL);
    #endif*/
	Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
	return 0;
}