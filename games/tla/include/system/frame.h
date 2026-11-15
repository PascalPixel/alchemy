#ifndef ALCHEMY_FRAME_H
#define ALCHEMY_FRAME_H

#include "types.h"

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
extern u8 gIdleSleepEnabled;
extern volatile u16 gIdleFrames;
extern volatile u8 gSleepRequested;
extern u8 gDebugMode;
extern u8 gDebugPaused; ///< the debug pause is on
/// Set by the keypad interrupt; wait() performs the soft reset.
extern u8 gResetRequested;
/// Written by the soft reset just before it restarts the cartridge, and
/// cleared by System_Initialize; nothing else reads it.
extern u32 gCartridgeResetMarker;
extern u32 gFrameCount;
extern u32 gPlayTimeFrames; ///< the play time clock, in frames

/// Stops the sound DMA and its timer.
void Sound_SuspendDirectSoundFar(void);

extern u32 gVBlankCount;
// volatile: the VBlank handler sets it while System_WaitForFrameInterrupt
// waits on it, so it is no device.
extern volatile u16 gVBlankDone;

#endif
