#ifndef ALCHEMY_ABI_GRAPHICS_PALETTE_STEP_FADE_TRANSFER
#define ALCHEMY_ABI_GRAPHICS_PALETTE_STEP_FADE_TRANSFER

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b50f0(void *, void *, s32, s32);

#define Sys_SetMode Func_080b50f0
#define gBattleWork Data_03001e74

#endif
