#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "types.h"

u32 Runtime_BumpAllocate(s32 size); /* ワークをバンプ確保 */
void Func_08002df0(void *buffer); /* Sys_Alloc の解放 */
/* Legacy register-call veneer declaration; new code must call the typed target. */
void Func_080072f0(void *dst, void *src, s32 size, void *desc);

#define Sys_Alloc Runtime_BumpAllocate
#define Sys_Free Func_08002df0
#define Mem_Copy Func_080072f0

#endif
