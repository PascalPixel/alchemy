#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

#include "types.h"

extern u8 gOamCopyEnabled;
extern u8 gFrameRenderPending;
extern u8 gCpuLoadDisplayEnabled;
extern u16 gFrameVBlanks;
extern u16 gLastFrameVBlanks;
extern u32 gCpuLoadTimer;
extern u32 gCpuLoadPeak;
extern volatile u8 gSleepDisabled;
extern u8 gIdleSleepEnabled;
extern volatile u16 gIdleFrames;
extern volatile u8 gSleepRequested;
extern u8 gDebugMode;
extern u8 gDebugPaused;
extern u8 gResetRequested;
extern u32 gCartridgeResetMarker;
extern u32 gFrameCount;
extern u32 gPlayTimeFrames;

void Sound_SuspendDirectSoundFar(void);

extern u32 gVBlankCount;
// volatile: the VBlank handler sets it while System_WaitForFrameInterrupt
// waits on it, so it is no device.
extern volatile u16 gVBlankDone;

#endif
