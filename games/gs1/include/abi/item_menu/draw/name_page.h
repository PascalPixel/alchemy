#ifndef ALCHEMY_ABI_ITEM_MENU_DRAW_NAME_PAGE
#define ALCHEMY_ABI_ITEM_MENU_DRAW_NAME_PAGE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015070(s32 window, s32 x, s32 width, s32 height, s32 style);
void Func_080a21b0(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Func_080a2324(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);

#define Sys_SetRange Func_08015070
#define Sys_SetRange2 Func_080a2324
#define Sys_SetRange3 Func_080a21b0

#endif
