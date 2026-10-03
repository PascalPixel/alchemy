#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

#include "TYPES.H"

extern u8 gRenderOamEnabled;
extern u8 gFrameRenderPending;
extern u8 gCpuLoadDisplayEnabled;
extern u16 gLagFrameCount;
extern u16 gLagFramesShown;
extern u32 gCpuLoadTimer;
extern u32 gCpuLoadPeak;
extern volatile u8 gSleepDisabled;
extern u8 gAutoSleepEnabled;
extern volatile u16 gIdleFrameCount;
extern volatile u8 gSleepRequested;
extern u8 gDebugMode;
extern u8 gDebugPaused;
extern u8 gResetRequested;
extern u32 gCartridgeResetMarker;
extern u32 gFrameCount;
extern u32 gFrameWaitCount;
extern u16 gSerialExchangeActive;

void AudioEngine_SuspendDirectSoundFar(void);

#endif
