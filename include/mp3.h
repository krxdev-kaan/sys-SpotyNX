#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    void mp3MutInit();
    void playMp3(const char* file);
    void pauseOrPlay();
    void setBreak();
    void setInput(const char* f);

#ifdef __cplusplus
}
#endif
