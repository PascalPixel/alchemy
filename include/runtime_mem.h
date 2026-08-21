#ifndef ALCHEMY_RUNTIME_MEM_H
#define ALCHEMY_RUNTIME_MEM_H

#include "types.h"

u32 Func_08004938(s32 size); /* ワークをバンプ確保 */
void Func_08002df0(void *buffer); /* Sys_Alloc の解放 */
/* 記述子付きコピー */
void Func_080072f0(void *dst, void *src, s32 size, void *desc);

#define Sys_Alloc Func_08004938
#define Sys_Free Func_08002df0
#define Mem_Copy Func_080072f0

#endif
