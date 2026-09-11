#ifndef ALCHEMY_ABI_SHOP_EFFECT_INIT
#define ALCHEMY_ABI_SHOP_EFFECT_INIT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0808a330(s32, s32);
s32 Func_0808a348(s32);

#define Effect_Apply Func_0808a330
#define Effect_Check Func_0808a348

#endif
