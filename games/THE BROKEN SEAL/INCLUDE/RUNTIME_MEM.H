#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "TYPES.H"

u32 Runtime_BumpAllocate(s32 size);
/* Legacy register-call veneer declaration; new code must call the typed target. */

void Sys_Free(void *buffer);
void _call_via_r3(void *dst, void *src, s32 size, void *desc);

#endif
