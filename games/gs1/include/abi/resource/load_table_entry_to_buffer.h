#ifndef ALCHEMY_ABI_RESOURCE_LOAD_TABLE_ENTRY_TO_BUFFER
#define ALCHEMY_ABI_RESOURCE_LOAD_TABLE_ENTRY_TO_BUFFER

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002dd8(s32);
void Func_0801a4c0(u32 index);

#define Sys_Check Func_08002dd8
#define Sys_Do Func_0801a4c0

#endif
