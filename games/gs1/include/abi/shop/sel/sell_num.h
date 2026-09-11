#ifndef ALCHEMY_ABI_SHOP_SEL_SELL_NUM
#define ALCHEMY_ABI_SHOP_SEL_SELL_NUM

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b04dc(s32);
s32 Func_080b1614(s32, s32, s32);
void Func_080a1038(void *);

#define Sys_Do Func_080b04dc
#define Sys_Do2 Func_080a1038
#define Sys_Place Func_080b1614

#endif
