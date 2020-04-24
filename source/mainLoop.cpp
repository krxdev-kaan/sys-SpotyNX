#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <switch.h>
#include "led.h"
#include "mp3.h"
#include "mainLoop.h"
#include "server.h"

using namespace std;

vector<string> musics = vector<string>();
u64 playid = 0;

Result mainLoop() 
{
    setupIPC();
    mp3MutInit();

            musics.clear();

            DIR* dir = opendir("sdmc:/spoty-musics/");
            struct dirent* ent;
            if (dir != NULL) 
            {
                while ((ent = readdir(dir)))
                {
                   musics.push_back(ent->d_name);
                }
                closedir(dir);
            }

    string temp = "sdmc:/spoty-musics/";
    string tempName = musics[playid];
    string tempDir = temp + tempName;
    playMp3(tempDir.c_str());
            
    while(appletMainLoop()) 
	{
        svcSleepThread(1e+7L);
        SpotyIPCRamDisk* activeDisk = retrieveIPC();

        if(activeDisk->currentOperation == 1)
        {
            pauseOrPlay();
            writeToIPC(0x0, activeDisk->fileIndex);
        } 
        else if (activeDisk->currentOperation == 2) 
        {
            string constantDir = "sdmc:/spoty-musics/";
            string musicName = musics[++playid];
            string concatenatedDirectory = constantDir + musicName;
            playMp3(concatenatedDirectory.c_str());
            writeToIPC(0x0, activeDisk->fileIndex >= musics.size() ? 0x0 : activeDisk->fileIndex + 0x1);
        }
        else if (activeDisk->currentOperation == 3) 
        {
            string constantDir = "sdmc:/spoty-musics/";
            string musicName = musics[--playid];
            string concatenatedDirectory = constantDir + musicName;
            playMp3(concatenatedDirectory.c_str());
            writeToIPC(0x0, activeDisk->fileIndex <= 0 ? 0x0 : activeDisk->fileIndex - 0x1);
        }
        else 
        {
        }
	}

    return 1;
}