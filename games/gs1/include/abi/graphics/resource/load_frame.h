#ifndef ALCHEMY_ABI_GRAPHICS_RESOURCE_LOAD_FRAME
#define ALCHEMY_ABI_GRAPHICS_RESOURCE_LOAD_FRAME

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_080053e8(const void *, void *);
void Func_08009288(void *, s32);
void Func_08002dd8(s32);

#define Sys_Apply Func_080053e8
#define Sys_Apply2 Func_08009288
#define Sys_Do Func_08002dd8
#define gVal Data_000000f1

#endif
