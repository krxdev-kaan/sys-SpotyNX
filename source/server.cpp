#include <switch.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <switch.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#include <netdb.h>
#include "server.h"
#include "mainLoop.h"

#define PORT 78184

void serverThreadFunc(void* arg) 
{
    //SOCKET INITALIZATION KILLS THE SWITCH (FIND THE REASON)
    Result rc = socketInitializeDefault();
    if (R_FAILED(rc)) {
        fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_AM));
    }
    
    int server_fd, sock, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    char buffer[128] = {0};
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        threadExit();
        return;
    }
    
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        threadExit();
        return;
    }
    
    if (listen(server_fd, 3) < 0)
    {
        threadExit();
        return;
    }
    
    if ((sock = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        threadExit();
        return;
    }
    
    while (true)
    {
        memset(buffer, 0, 128);
        valread = read(sock, buffer, 128);
        
        if(buffer[0] == '\0')
        {
            if ((sock = accept(server_fd, (struct sockaddr *)&address,
                                     (socklen_t*)&addrlen))<0)
            {
                threadExit();
                return;
            }
        }
        else 
        {
            //
            //
            // CHECK IF BUFFER IS PLAY or PAUSE or QUEUE or SONGCHANGE or SKIP
            //
            //

            if (buffer == "play") 
            {
                eventUp = true;
            }
        }
    }
}

