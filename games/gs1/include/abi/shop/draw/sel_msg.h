#ifndef ALCHEMY_ABI_SHOP_DRAW_SEL_MSG
#define ALCHEMY_ABI_SHOP_DRAW_SEL_MSG

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b2778(s32 selection, s32 variant);
s32 Func_080b27b0(s32 selection, s32 variant);
s32 Func_080b2884(s32 msg);

#define Sys_Apply Func_080b2778
#define Sys_Apply2 Func_080b27b0
#define Sys_Check Func_080b2884
#define gIw Data_03001f2c

#endif
