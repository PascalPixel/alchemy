#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "TYPES.H"
#include "RUNTIME_MEM.H"

void WaitFrames(s32 frames);
void System_WaitForFrameInterrupt(void);
void Graphics_ResetFrameState(void);
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
u32 Random16(void);
extern u32 gRandomState;

extern volatile u16 gSleepActive;

#endif
