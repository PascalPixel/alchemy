#ifndef ALCHEMY_ABI_SHOP_SEL_SELL
#define ALCHEMY_ABI_SHOP_SEL_SELL

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b1f4c(s32, s32, s32);

#define Shop_SelSell Func_080b1bd0
#define Sys_Place Func_080b1f4c

#endif
