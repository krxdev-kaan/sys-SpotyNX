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

Thread m_IPCThread; // I may have to use this since reading/writing to IPC could (potentially) freeze the switch.
Thread m_MP3Thread;
string m_argument;
bool m_thread1ShouldDispose = false;
bool m_thread1IsRunning = false;

vector<string> musics = vector<string>();

void startMP3Thread();

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
    string tempName = musics[0];
    string tempDir = temp + tempName;
    m_argument = tempDir;
    startMP3Thread();
            
    while(appletMainLoop()) 
	{
        svcSleepThread(1e+7L);

        if(m_thread1ShouldDispose) 
        {
            threadClose(&m_MP3Thread);
            m_thread1ShouldDispose = false;
            m_thread1IsRunning = false;
        }

        SpotyIPCRamDisk* activeDisk = retrieveIPC();

        if(activeDisk->currentOperation == 1)
        {
            pauseOrPlay();
            writeToIPC(0x0, activeDisk->fileIndex);
        } 
        else if (activeDisk->currentOperation == 2) 
        {
            string constantDir = "sdmc:/spoty-musics/";
            string musicName = musics[activeDisk->fileIndex >= musics.size() ? 0x0 : activeDisk->fileIndex + 0x1];
            string concatenatedDirectory = constantDir + musicName;
            m_argument = concatenatedDirectory;
            setInput(m_argument.c_str());
            writeToIPC(0x0, activeDisk->fileIndex >= musics.size() ? 0x0 : activeDisk->fileIndex + 0x1);
        }
        else if (activeDisk->currentOperation == 3) 
        {
            string constantDir = "sdmc:/spoty-musics/";
            string musicName = musics[activeDisk->fileIndex <= 0 ? 0x0 : activeDisk->fileIndex - 0x1];
            string concatenatedDirectory = constantDir + musicName;
            m_argument = concatenatedDirectory;
            setInput(m_argument.c_str());
            writeToIPC(0x0, activeDisk->fileIndex <= 0 ? 0x0 : activeDisk->fileIndex - 0x1);
        }
        else 
        {
        }
	}

    return 1;
}

void startMP3T1(void* arg) 
{
    m_thread1IsRunning = true;
    playMp3(m_argument.c_str());
    m_thread1IsRunning = false;
    m_thread1ShouldDispose = true;
}

void startMP3Thread() 
{
    threadCreate(&m_MP3Thread, startMP3T1, NULL, NULL, 0x10000, 0x2C, -2);
    threadStart(&m_MP3Thread);
}