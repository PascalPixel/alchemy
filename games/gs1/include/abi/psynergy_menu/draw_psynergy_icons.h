#ifndef ALCHEMY_ABI_PSYNERGY_MENU_DRAW_PSYNERGY_ICONS
#define ALCHEMY_ABI_PSYNERGY_MENU_DRAW_PSYNERGY_ICONS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015298(s32 style, u16 action, u8 target, s32 flags);

#define Sys_SetMode Func_08015298

#endif
