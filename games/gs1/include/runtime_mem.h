#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "types.h"

u32 Runtime_BumpAllocate(s32 size);
/* Legacy register-call veneer declaration; new code must call the typed target. */

#define Sys_Alloc Runtime_BumpAllocate

void Sys_Free(void *buffer);
void Mem_Copy(void *dst, void *src, s32 size, void *desc);

#endif
