#ifndef ALCHEMY_ABI_SHOP_DRAW_USE_ITEM
#define ALCHEMY_ABI_SHOP_DRAW_USE_ITEM

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015270(s32);
void Func_08015080(s32, s32, s32, s32);
s32 Func_080772a8(s32, s32);
s32 Func_080b19cc(s32);
void Func_080150b0(s32, s32, s32, s32, s32);

#define Sys_Apply Func_080772a8
#define Sys_Run Func_08077008

#endif
