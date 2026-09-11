#ifndef ALCHEMY_ABI_SHOP_EFFECT_RESET
#define ALCHEMY_ABI_SHOP_EFFECT_RESET

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0808a500(s32);
void Func_08009248(s32, u32);

#define Effect_Apply Func_08009248
#define Effect_Do Func_0808a500

#endif
