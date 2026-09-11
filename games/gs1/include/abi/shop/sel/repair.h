#ifndef ALCHEMY_ABI_SHOP_SEL_REPAIR
#define ALCHEMY_ABI_SHOP_SEL_REPAIR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b2328(s32, s32);

#define Shop_SelRepair Func_080b211c
#define Sys_Apply Func_080b2328

#endif
