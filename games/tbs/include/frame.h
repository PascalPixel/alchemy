#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

#include "types.h"

extern u8 gFrameRenderPending;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it Title_ShowIntro: differ 88 in tbs-en, 438 instructions either
 * way. */
extern volatile u8 gCpuLoadDisplayEnabled;
extern u8 gOamCopyEnabled;
extern u16 gLagFrameCount;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it UiWork_StepChannelScript: differ 33 in tbs-en, 627
 * instructions for 628. */
extern volatile u16 gLagFramesShown;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it WaitFrames: differ 119 in tbs-en, 394 instructions for 403.
 */
extern volatile u16 gVBlankDone;
extern u32 gCpuLoadTimer;
extern u32 gCpuLoadPeak;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own. Measured 2026-10-12 with alchemy diff on tbs-en: without it 27 functions
 * in 17 files differ, most Ui_RunOwnerStatusScreen (529), ItemList_SelectEntry
 * (346) and SummonMenu_SelectSummon (151). */
extern volatile u32 gFrameCount;
extern u32 gFrameTick;
extern void (*gVBlankHook)(void);

/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it WaitFrames: differ 82 in tbs-en, 401 instructions for 403. */
extern volatile u8 gSleepDisabled;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it GameState_InitDefaults: differ 45 in tbs-en, 202 instructions
 * for 203. */
extern volatile u8 gIdleSleepEnabled; // sleep after three idle minutes
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it WaitFrames: differ 26 in tbs-en, 403 instructions either way.
 */
extern volatile u8 gSleepRequested; // 1: sleep this frame; more: count down
extern u8 gDebugMode;
extern u8 gDebugPaused;
extern u8 gResetRequested;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it WaitFrames: differ 89 in tbs-en, 402 instructions for 403. */
extern volatile u16 gIdleFrames; // frames since a key was last held
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it WaitFrames: differ 117 in tbs-en, 404 instructions for 403.
 */
extern volatile u16 gSleepComboFrames;
extern u32 gCartridgeResetMarker;
extern u32 gPlayTimeFrames; // the play time clock, in frames
extern u32 gSavedStackSize;
extern u8 gSavedStack[];

#endif
