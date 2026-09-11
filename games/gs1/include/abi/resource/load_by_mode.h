#ifndef ALCHEMY_ABI_RESOURCE_LOAD_BY_MODE
#define ALCHEMY_ABI_RESOURCE_LOAD_BY_MODE

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_08019ee4(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
extern s32 Func_0801a2a4(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_0801a32c(u32 value, s32 unused, void *destination);
extern void Func_0801a3d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

#define Sys_Place Func_0801a2a4
#define Sys_Place2 Func_0801a32c
#define Sys_SetRange Func_08019ee4
#define Sys_SetRange2 Func_0801a3d0

#endif
