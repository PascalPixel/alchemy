#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

#include "types.h"

extern u8 gFrameRenderPending;
extern u8 gCpuLoadDisplayEnabled;
extern u8 gOamCopyEnabled;
extern u16 gLagFrameCount;
extern u16 gLagFramesShown;
extern volatile u16 gVBlankDone;
extern u32 gCpuLoadTimer;
extern u32 gCpuLoadPeak;
extern volatile u32 gFrameCount;
extern s32 gFrameTick;
extern void (*gVBlankHook)(void);

extern volatile u8 gSleepDisabled;
extern volatile u8 gOptionMirror;
extern volatile u8 gSleepRequested;
extern u8 gDebugMode;
extern u8 gDebugPaused;
extern u8 gResetRequested;
extern volatile u16 gPostLoadCounter;
extern volatile u16 gSleepComboFrames;
extern u32 gCartridgeResetMarker;
extern u32 gLoadedStateWord;
extern u32 gSavedStackSize;
extern u8 gSavedStack[];

#endif
