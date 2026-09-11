#ifndef ALCHEMY_ABI_SHOP_DRAW_PARTY
#define ALCHEMY_ABI_SHOP_DRAW_PARTY

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077220(s16, s32);

#define Sys_Apply Func_08077220
#define gIw Data_03001f2c

#endif
