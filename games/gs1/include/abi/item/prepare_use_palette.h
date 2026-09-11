#ifndef ALCHEMY_ABI_ITEM_PREPARE_USE_PALETTE
#define ALCHEMY_ABI_ITEM_PREPARE_USE_PALETTE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080a22f4(void);
void Func_080a2144(s32);

#define Item_Do Func_080a2144
#define Item_Run Func_080a22f4

#endif
