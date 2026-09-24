#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "TYPES.H"

/* Low runtime services every module reaches through the system call table
   at the start of ROM. */

void WaitFrames(s32 frames);
u32 Random16(void);
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);

#endif
