#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

#include "TYPES.H"

/* Rendering and VBlank state. These are separate IWRAM cells. */
extern u8 gFrameRenderPending; /* Display and OAM work pending. */
extern u8 gCpuLoadDisplayEnabled; /* CPU load display enabled. */
extern u8 gOamCopyEnabled;
extern u16 gLagFrameCount; /* VBlanks since the last frame wait. */
extern u16 gLagFramesShown;
extern volatile u16 gVBlankDone; /* Set by the VBlank handler. */
extern u32 gCpuLoadTimer;
extern u32 gCpuLoadPeak;
extern volatile u32 gFrameCount;
extern s32 gFrameTick;
extern void (*Data_03001cfc)(void); /* One-shot VBlank callback. */

/* Sleep, reset and frame-wait state. */
extern volatile u8 gSleepDisabled;
extern volatile u8 gOptionMirror; /* Auto-sleep setting. */
extern volatile u8 gSleepRequested;
extern u8 gDebugMode;
extern u8 gDebugPaused;
extern u8 gResetRequested;
extern volatile u16 gPostLoadCounter; /* Consecutive idle frames; also reset on load. */
extern volatile u16 gSleepComboFrames;
extern u32 gCartridgeResetMarker;
extern u32 gLoadedStateWord; /* Play-time frame counter, saved and restored. */
extern u32 gSavedStackSize;
extern u8 gSavedStack[];

#endif
