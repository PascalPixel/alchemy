#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "TYPES.H"
#include "RUNTIME_MEM.H"

/* Low runtime services every module reaches through the system call table
   at the start of ROM. */

void WaitFrames(s32 frames);
void System_WaitForFrameInterrupt(void);
void Graphics_ResetFrameState(void);
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
u32 Random16(void);
extern u32 gRandomState;

/* Set while WaitFrames holds the console in sleep, waiting for L and R to
   wake it; the key interrupt is armed again only when it is clear. */
extern volatile u16 gSleepActive;

#endif
