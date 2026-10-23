#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

/// @file
/// The frame loop's globals: lag counts, the CPU load display and the VBlank
/// flags.
///
/// WaitFrames (runtime/frame.c) and the VBlank handler share these cells.
/// gVBlankDone and gFrameCount are volatile because an interrupt handler uses
/// them too; the other volatiles are only ever read and written by the main
/// loop, so they are devices, and each tag names the function that differs
/// without one.

#include "types.h"

/// Set by WaitFrames once it has built a frame's OAM list; the VBlank handler
/// then copies the list, the BG scroll and the queued register writes, and
/// clears it.
extern u8 gFrameRenderPending;
/// Debug: WaitFrames measures the CPU load into gCpuLoadPeak when set.
/* FAKEMATCH(volatile): as plain u8, 88 of 438 instructions differ in tbs-en
 * (Title_ShowIntro). */
extern volatile u8 gCpuLoadDisplayEnabled;
/// The VBlank handler copies the OAM list only while it is set; maps clear
/// it to keep their own sprites on screen.
extern u8 gOamCopyEnabled;
/// VBlanks since WaitFrames last started a frame: 1 when the frame took one.
extern u16 gLagFrameCount;
/// gLagFrameCount as the last frame left it; the text typing paces itself by
/// it.
/* FAKEMATCH(volatile): as plain u16, 33 of 628 instructions differ in tbs-en
 * (TextPrinter_Step). */
extern volatile u16 gLagFramesShown;
/// Bit 0 is set by the VBlank handler; WaitFrames clears it and halts until
/// it is set again.
// volatile: the VBlank handler sets it while WaitFrames halts on it, so it is
// no device.
extern volatile u16 gVBlankDone;
extern u32 gCpuLoadTimer; ///< Frames gCpuLoadPeak is still held.
/// The highest CPU load in the last 30 frames: the scanline the frame's work
/// ended on, plus 256 for each frame it overran.
extern u32 gCpuLoadPeak;
/// Frames WaitFrames has finished.
// volatile: the VBlank pass reads it (WorldMap_ApplyAffineFrame picks
// the world map's affine page by it) while WaitFrames counts it, so it is no
// device.
extern volatile u32 gFrameCount;
extern u32 gFrameTick; ///< Counted up by every VBlank.
/// Run once by the next VBlank handler, which clears it first.
extern void (*gVBlankCallback)(void);

/// Nonzero: WaitFrames counts no idle time and ignores sleep requests.
/* FAKEMATCH(volatile): as plain u8, 82 of 403 instructions differ in tbs-en
 * (WaitFrames). */
extern volatile u8 gSleepDisabled;
/// Sleep after three idle minutes.
/* FAKEMATCH(volatile): as plain u8, 45 of 203 instructions differ in tbs-en
 * (SaveBlock_InitializeDefaults). */
extern volatile u8 gIdleSleepEnabled;
/// 1: sleep this frame; more: count down to it.
/* FAKEMATCH(volatile): as plain u8, 26 of 403 instructions differ in tbs-en
 * (WaitFrames). */
extern volatile u8 gSleepRequested;
/// Debug builds: Start+Select pauses the frame loop.
extern u8 gDebugMode;
extern u8 gDebugPaused; ///< The debug pause is on.
/// Set by the keypad interrupt; WaitFrames performs the soft reset.
extern u8 gResetRequested;
/// Frames since a key was last held.
/* FAKEMATCH(volatile): as plain u16, 89 of 403 instructions differ in tbs-en
 * (WaitFrames). */
extern volatile u16 gIdleFrames;
/// Frames L and R have been held together; three seconds request sleep.
/* FAKEMATCH(volatile): as plain u16, 117 of 403 instructions differ in tbs-en
 * (WaitFrames). */
extern volatile u16 gSleepComboFrames;
/// Written by the soft reset just before it restarts the cartridge, and
/// cleared by System_Initialize; nothing else reads it.
extern u32 gCartridgeResetMarker;
extern u32 gPlayTimeFrames; ///< The play time clock, in frames.
/// Bytes of the caller's stack WaitFrames has copied into gSavedStack, or 0.
extern u32 gSavedStackSize;
/// The caller's stack below the frame-wait stack top, while WaitFrames runs.
extern u8 gSavedStack[];

#endif
