#ifndef ALCHEMY_ABI_SHOP_UNIT_COUNT
#define ALCHEMY_ABI_SHOP_UNIT_COUNT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b27b0(s32 selection, s32 variant);

#define Sys_Apply Func_080b27b0
#define gIw Data_03001f2c

#endif
