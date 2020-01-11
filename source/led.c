//All of the codes inspiration comes from jakibaki/sys-ftpd's led system on sysmodule.
//And of course ELY3M because of great explanation fo the LED system.

#include <string.h>
#include <switch.h>
#include "led.h"

/**
 * ledMode 0=Breath, 1=Strobing, 2=Heartbeat, 3=Flash, 3 < x = 0
 */
void startLed(uint8_t ledMode)
{
    HidsysNotificationLedPattern pattern;
    memset(&pattern, 0, sizeof(pattern));

    if (ledMode == 0) 
    {
        pattern.baseMiniCycleDuration = 0x8;
        pattern.totalMiniCycles = 0x2;
        pattern.totalFullCycles = 0x0;
        pattern.startIntensity = 0x1;

        pattern.miniCycles[0].ledIntensity = 0xF;
        pattern.miniCycles[0].transitionSteps = 0xF;
        pattern.miniCycles[0].finalStepDuration = 0x0;
        pattern.miniCycles[1].ledIntensity = 0x1;
        pattern.miniCycles[1].transitionSteps = 0xF;
        pattern.miniCycles[1].finalStepDuration = 0x0;
    }
    else if (ledMode == 1) 
    {
            pattern.baseMiniCycleDuration = 0x1;             // 12.5 ms
            pattern.totalMiniCycles = 0x2;                   // 3 mini cycles. Last one 12.5ms.
            pattern.totalFullCycles = 0x0;                   // Repeat forever.
            pattern.startIntensity = 0xF;                    // 100%.
			
            pattern.miniCycles[0].ledIntensity = 0xF;        // 100%.
            pattern.miniCycles[0].transitionSteps = 0x0;     // Forced 12.5ms.
            pattern.miniCycles[0].finalStepDuration = 0x0;   // Forced 12.5ms.
            pattern.miniCycles[1].ledIntensity = 0x0;        // 0%.
            pattern.miniCycles[1].transitionSteps = 0x0;     // Forced 12.5ms.
            pattern.miniCycles[1].finalStepDuration = 0x0;   // Forced 12.5ms.

            pattern.miniCycles[2].ledIntensity = 0xF; 		  // 100%
            pattern.miniCycles[2].transitionSteps = 0x0;	  // Forced 12.5ms.
            pattern.miniCycles[2].finalStepDuration = 0x0;	  // Forced 12.5ms.
            pattern.miniCycles[3].ledIntensity = 0x0; 		  // 0%
            pattern.miniCycles[3].transitionSteps = 0x0;     // Forced 12.5ms.
            pattern.miniCycles[3].finalStepDuration = 0x0;   // Forced 12.5ms.
    }
    else if (ledMode == 2) 
    {
            pattern.baseMiniCycleDuration = 0x1;             // 12.5ms.
            pattern.totalMiniCycles = 0xF;                   // 16 mini cycles.
            pattern.totalFullCycles = 0x0;                   // Repeat forever.
            pattern.startIntensity = 0x0;                    // 0%.

            // First beat.
            pattern.miniCycles[0].ledIntensity = 0xF;        // 100%.
            pattern.miniCycles[0].transitionSteps = 0xF;     // 15 steps. Total 187.5ms.
            pattern.miniCycles[0].finalStepDuration = 0x0;   // Forced 12.5ms.
            pattern.miniCycles[1].ledIntensity = 0x0;        // 0%.
            pattern.miniCycles[1].transitionSteps = 0xF;     // 15 steps. Total 187.5ms.
            pattern.miniCycles[1].finalStepDuration = 0x0;   // Forced 12.5ms.

            // Second beat.
            pattern.miniCycles[2].ledIntensity = 0xF;
            pattern.miniCycles[2].transitionSteps = 0xF;
            pattern.miniCycles[2].finalStepDuration = 0x0;
            pattern.miniCycles[3].ledIntensity = 0x0;
            pattern.miniCycles[3].transitionSteps = 0xF;
            pattern.miniCycles[3].finalStepDuration = 0x0;

            // Led off wait time.
            for(int i = 2; i < 15; i++) {
                pattern.miniCycles[i].ledIntensity = 0x0;        // 0%.
                pattern.miniCycles[i].transitionSteps = 0xF;     // 15 steps. Total 187.5ms.
                pattern.miniCycles[i].finalStepDuration = 0xF;   // 187.5ms.
            }
    } 
    else if (ledMode == 3) 
    {
            pattern.baseMiniCycleDuration = 0x1;             // 12.5 ms
            pattern.totalMiniCycles = 0x2;                   // 3 mini cycles. Last one 12.5ms.
            pattern.totalFullCycles = 0x0;                   // Repeat forever.
            pattern.startIntensity = 0xF;                    // 100%.


            pattern.miniCycles[0].ledIntensity = 0xF;        // 100%.
            pattern.miniCycles[0].transitionSteps = 0xF;     // 15 steps. Total 187.5ms.
            pattern.miniCycles[0].finalStepDuration = 0x0;   // Forced 12.5ms.
            pattern.miniCycles[1].ledIntensity = 0x0;        // 0%.
            pattern.miniCycles[1].transitionSteps = 0xF;     // 15 steps. Total 187.5ms.
            pattern.miniCycles[1].finalStepDuration = 0x0;   // Forced 12.5ms.

            pattern.miniCycles[2].ledIntensity = 0xF; 		  // 100%
            pattern.miniCycles[2].transitionSteps = 0xF;	  // 15 steps. Total 187.5ms.
            pattern.miniCycles[2].finalStepDuration = 0x0;	  // Forced 12.5ms.
            pattern.miniCycles[3].ledIntensity = 0x0; 		  // 0%
            pattern.miniCycles[3].transitionSteps = 0xF;     // 15 steps. Total 187.5ms.
            pattern.miniCycles[3].finalStepDuration = 0x0;   // Forced 12.5ms.
    } 
    else 
    {
        pattern.baseMiniCycleDuration = 0x8;
        pattern.totalMiniCycles = 0x2;
        pattern.totalFullCycles = 0x0;
        pattern.startIntensity = 0x1;

        pattern.miniCycles[0].ledIntensity = 0xF;
        pattern.miniCycles[0].transitionSteps = 0xF;
        pattern.miniCycles[0].finalStepDuration = 0x0;
        pattern.miniCycles[1].ledIntensity = 0x1;
        pattern.miniCycles[1].transitionSteps = 0xF;
        pattern.miniCycles[1].finalStepDuration = 0x0;
    }

    u64 UniquePadIds[5] = {0};
    memset(UniquePadIds, 0, sizeof(UniquePadIds));

    size_t total_entries = 0;

    Result rc = hidsysGetUniquePadIds(UniquePadIds, 5, &total_entries);

    if (R_SUCCEEDED(rc)) 
    {
        for(int i = 0; i < total_entries; i++) 
        {
            rc = hidsysSetNotificationLedPattern(&pattern, UniquePadIds[i]);
        }
    }
}

void shutdownLed(void)
{
    HidsysNotificationLedPattern pattern;
    memset(&pattern, 0, sizeof(pattern));

    u64 UniquePadIds[5] = {0};
    memset(UniquePadIds, 0, sizeof(UniquePadIds));

    size_t total_entries = 0;

    Result rc = hidsysGetUniquePadIds(UniquePadIds, 5, &total_entries);

    if (R_SUCCEEDED(rc)) 
    {
        for (int i = 0; i < total_entries; i++)
            hidsysSetNotificationLedPattern(&pattern, UniquePadIds[i]);
    }
}
