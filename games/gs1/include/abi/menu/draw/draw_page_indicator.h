#ifndef ALCHEMY_ABI_MENU_DRAW_DRAW_PAGE_INDICATOR
#define ALCHEMY_ABI_MENU_DRAW_DRAW_PAGE_INDICATOR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015280(s32 window, s32 tile, s32 x, s32 y, s32 style);

#define Menu_SetRange Func_08015280

#endif
