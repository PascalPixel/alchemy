#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

/// @file
/// The frame loop's globals: the VBlanks a frame takes, the CPU load
/// display and the VBlank flags, which wait() (system/system_frame.c) and the
/// VBlank handler share.

#include "types.h"

/// Frames in a second: the screen refreshes about 60 times a second.
#define FRAMES_PER_SECOND 60

/// The VBlank handler copies the OAM list only while it is set; scenes clear
/// it to keep their own sprites on screen.
extern u8 gOamCopyEnabled;
/// Set by wait() once it has built a frame's OAM list; the VBlank handler
/// then copies the list, the BG scroll and the queued register writes, and
/// clears it.
extern u8 gFrameRenderPending;
/// Debug: wait() measures the CPU load into gCpuLoadPeak when set.
extern u8 gCpuLoadDisplayEnabled;
/// VBlanks since wait() last started a frame: 1 when the frame took one.
extern u16 gFrameVBlanks;
/// gFrameVBlanks as the last frame left it; the text typing paces itself by
/// it.
extern u16 gLastFrameVBlanks;
extern u32 gCpuLoadTimer; ///< frames gCpuLoadPeak is still held
/// The highest CPU load in the last 30 frames: the scanline the frame's work
/// ended on, plus 256 for each VBlank it overran.
extern u32 gCpuLoadPeak;
/// Nonzero: wait() counts no idle time and ignores sleep requests.
extern volatile u8 gSleepDisabled;
/// Sleep after three idle minutes.
extern volatile u8 gIdleSleepEnabled;
/// Frames since a key was last held.
extern volatile u16 gIdleFrames;
/// 1: sleep this frame; more: count down to it.
extern volatile u8 gSleepRequested;
/// Debug builds: Start+Select pauses the frame loop.
extern u8 gDebugMode;
extern u8 gDebugPaused; ///< the debug pause is on
/// Set by the keypad interrupt; wait() performs the soft reset.
extern u8 gResetRequested;
/// Written by the soft reset just before it restarts the cartridge, and
/// cleared by System_Initialize; nothing else reads it.
extern u32 gCartridgeResetMarker;
/// Frames wait() has finished.
extern u32 gFrameCount;
extern u32 gPlayTimeFrames; ///< the play time clock, in frames

/// Stops the sound DMA and its timer.
void Sound_SuspendDirectSoundFar(void);

extern u32 gVBlankCount; ///< counted up by every VBlank
// volatile: the VBlank handler sets it while System_WaitForFrameInterrupt
// waits on it, so it is no device.
/// Bit 0 is set by the VBlank handler; System_WaitForFrameInterrupt clears it
/// and halts until it is set again.
extern volatile u16 gVBlankDone;

#endif
