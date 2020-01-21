#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <switch.h>
#include "led.h"
#include "mp3.h"
#include "mainLoop.h"

using namespace std;

vector<string> musics = vector<string>();
u64 playid = 0;

Result mainLoop() 
{
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

    while(appletMainLoop()) 
	{
        svcSleepThread(1e+7L);

        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

        if ((kDown & KEY_ZR || kDown & KEY_A) && (kHeld & KEY_ZR && kHeld & KEY_A)) 
        {
            string constantDir = "sdmc:/spoty-musics/";
            string musicName = musics[playid];
            string concatenatedDirectory = constantDir + musicName;
            playMp3(concatenatedDirectory.c_str());
            if (playid >= (musics.size() - 1)) 
                playid++;
            else
                playid = 0;
        }
	}

    return 1;
}