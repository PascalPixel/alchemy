#ifndef ALCHEMY_ABI_ITEM_MENU_DRAW_STAT_DELTA
#define ALCHEMY_ABI_ITEM_MENU_DRAW_STAT_DELTA

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015098(u8 *, s32, s32, s32);

#define ItemMenu_DecreaseGlyph Data_080af228
#define ItemMenu_IncreaseGlyph Data_080af224
#define Sys_SetMode Func_08015098

#endif
