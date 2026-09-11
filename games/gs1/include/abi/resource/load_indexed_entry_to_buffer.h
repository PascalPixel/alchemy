#ifndef ALCHEMY_ABI_RESOURCE_LOAD_INDEXED_ENTRY_TO_BUFFER
#define ALCHEMY_ABI_RESOURCE_LOAD_INDEXED_ENTRY_TO_BUFFER

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002dd8(s32);
void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

#define Sys_Check Func_08002dd8
#define Sys_SetRange Func_0801a3d0

#endif
