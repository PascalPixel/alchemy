#ifndef ALCHEMY_ABI_PSYNERGY_MENU_DRAW_ACTION_PAGE
#define ALCHEMY_ABI_PSYNERGY_MENU_DRAW_ACTION_PAGE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015070(s32 window, s32 x, s32 width, s32 height, s32 style);
void Func_080a2324(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);
void Func_080a21b0(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Func_080150a8(s32 value, s32 digits, s32 layer, s32 x, s32 y);

#define PsynergyMenu_DrawActionPage Func_080a6b64
#define Sys_Run Func_08077008
#define Sys_Run2 Func_080a735c
#define Sys_SetRange Func_08015070
#define Sys_SetRange2 Func_080a2324
#define Sys_SetRange3 Func_080a21b0
#define Sys_SetRange4 Func_080150a8
#define gIw Data_03001f2c

#endif
