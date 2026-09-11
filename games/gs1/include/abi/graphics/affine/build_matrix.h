#ifndef ALCHEMY_ABI_GRAPHICS_AFFINE_BUILD_MATRIX
#define ALCHEMY_ABI_GRAPHICS_AFFINE_BUILD_MATRIX

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);

#define Sys_Check Func_08002322
#define Sys_Check2 Func_0800231c
#define gIw Data_03001d00
#define gIw2 Data_03001d40

#endif
