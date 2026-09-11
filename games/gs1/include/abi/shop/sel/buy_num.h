#ifndef ALCHEMY_ABI_SHOP_SEL_BUY_NUM
#define ALCHEMY_ABI_SHOP_SEL_BUY_NUM

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_080b04dc(s32);
extern s32 Func_080022f4(s32, s32);
extern s32 Func_080b1614(s32, s32, s32);

#define Sys_Apply Func_080022f4
#define Sys_Do Func_080b04dc
#define Sys_Place Func_080b1614

#endif
