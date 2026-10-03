#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

#include "TYPES.H"

/* Rendering and VBlank state. These are separate IWRAM cells. */
extern u8 Data_03001e44; /* Display and OAM work pending. */
extern u8 Data_03001f58; /* CPU load display enabled. */
extern u8 gOamCopyEnabled;
extern u16 Data_03001ccc; /* VBlanks since the last frame wait. */
extern u16 gLagFramesShown;
extern u16 Data_03001d28; /* VBlank completion latch. */
extern u32 gCpuLoadTimer;
extern u32 gCpuLoadPeak;
extern u32 gFrameCount;
extern s32 gFrameTick;
extern void (*Data_03001cfc)(void); /* One-shot VBlank callback. */

/* Sleep, reset and frame-wait state. */
extern u8 Data_03001ca0; /* Sleep disabled. */
extern u8 gOptionMirror; /* Auto-sleep setting. */
extern u8 gSleepRequested;
extern u8 gDebugMode;
extern u8 gDebugPaused;
extern u8 gResetRequested;
extern u16 gPostLoadCounter; /* Consecutive idle frames; also reset on load. */
extern u16 gSleepComboFrames;
extern u32 Data_03007800; /* Cartridge reset marker. */
extern u32 gLoadedStateWord; /* Play-time frame counter, saved and restored. */
extern u32 gSavedStackSize;
extern u8 gSavedStack[];

/* IRQ dispatch table and BIOS vector installed with the resident runtime. */
extern void (*Data_030000e0[14])(void);
extern void *Data_03007ffc;

#endif
